//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "AcyclicBroker.h"
#include "Client.h"

Define_Module(AcyclicBroker);

AcyclicBroker::AcyclicBroker() {
	subscriptionMonitor = new SubscriptionMonitor(NR_TOPICS);
}
AcyclicBroker::~AcyclicBroker() {}

void AcyclicBroker::sleep(){	//TODO what if in the same time 2 connected brokers decide to disconnect? Hell will break loose, we need to use a sort of "locking" mechanism in this
	//step0: decide if I can sleep (if I'm the only broker present, means I cannot sleep) Thus, I check if I have any connected Brokers
	//OBS: I could instead unregister and bring the whole network to an "unavailable state" but I believe it really doesn't make sense.
	LinkedList<NeighbourEntry>* brokersList = neighboursMap.getBrokersList();
	if (brokersList->size<=0) { //it means we cannot sleep, we reschedule the sleep
		scheduleAt(simTime() + par("SleepDelay"), sleepDelayMsg);
		EV << "I cannot sleep, I'm alone";
		return;
	}
	//step1: we need to redirect every other Broker, in order NOT to break the chain
	//TODO Once we implement the "network conditions", this will become much more complicated (we'll try to link the Brokers in such a way as to minimize the latency between them)
	if (brokersList->size>1){ //cause if we're connected with only 1 broker we're not breaking any chain
		NeighbourEntry* prevEntry = brokersList->removeFromFront();
		NeighbourEntry* entry = brokersList->removeFromFront();
		while (entry!=NULL){
			send(new ConnectionRequestMessage(prevEntry->getNeighbour()),entry->getOutGate());
			send(new ConnectionRequestMessage(entry->getNeighbour()),prevEntry->getOutGate());
			prevEntry = entry;
			entry = brokersList->removeFromFront();
		}
	}
	delete(brokersList);
	//step2: unregister from NameServ through a Disconnect message, such that we do not receive any more connection requests from either brokers or clients
	sendDirect(new DisconnectionRequestMessage(this), getNSGate());
	//step3: send disconnection requests to all neighbors
	LinkedList<NeighbourEntry>* nList = neighboursMap.getNeighboursList();
	for (NeighbourEntry* ne = nList->removeFromFront();ne!=NULL;ne = nList->removeFromFront()){
		send(new DisconnectionRequestMessage(this), ne->getOutGate());
		ne->getOutGate()->disconnect();
		neighboursMap.removeMapping(ne->getNeighbour()); //this entry will become null
	}
	delete (nList);
	//step4: schedule a rewake
	scheduleAt(simTime() + par("WakeUpDelay"), wakeUpDelayMsg);
}

void AcyclicBroker::handleDisconnectionRequest(DisconnectionRequestMessage* drm){//primarily is the same the basic broker, just that I must also check any possible unsubscriptions
	std::vector<int> hisSubscriptions = neighboursMap.getSubscriptions(drm->getRequesterNode());
	Broker::handleDisconnectionRequest(drm); //This is the call to super class (Broker) to disconnect the requesting side, and remove entry
	//now decide if we must unsubscribe to any particular topics (since we have no client subscribed to a given topic, if thats the case)
	for (unsigned int i=0;i<hisSubscriptions.size();i++){
		LinkedList<NeighbourEntry>* sList = neighboursMap.getSubscribers(hisSubscriptions[i]);
		if (sList->size<=0 && subscriptionMonitor->isSubscribed(hisSubscriptions[i])){ //means we have no subscriber left for that topic, so we must unsubcribe definitely
			subscriptionMonitor->unsubscribe(hisSubscriptions[i]);
			LinkedList<NeighbourEntry>* bList = neighboursMap.getBrokersList();
			for (NeighbourEntry* ne = bList->removeFromFront();ne!=NULL;ne = bList->removeFromFront()){
				send(new UnsubscriptionMessage(this,hisSubscriptions[i]),ne->getOutGate());
			}
			delete(bList);
		}
		delete (sList);
	}
}

void AcyclicBroker::handlePublish(PublishMessage* pm){
	STNode* stn = pm->getSender();
	LinkedList<NeighbourEntry>* sList = neighboursMap.getSubscribers(pm->getTopic());
	for (NeighbourEntry* ne = sList->removeFromFront();ne!=NULL;ne = sList->removeFromFront()){
		if (ne->getNeighbour()!=stn){ //we dont send back messages
			//send (new PublishMessage(this,topic,pm->getTimeStamp()),ne->getOutGate());
			send (pm->clone(this),ne->getOutGate());
		}
	}
	delete (sList);
	cancelAndDelete(pm);
}

void AcyclicBroker::handleSubscription(SubscriptionMessage* sm){
	STNode* stn = sm->getSubscriber();
	int topic = sm->getTopic();
	//step1: if I'm not already subscribed to the topic, subscribe to this topic to my neighbouring brokers
	if (!neighboursMap.isSubscribed(stn, topic)){//otherwise no use to repropagate
		LinkedList<NeighbourEntry>* bList = neighboursMap.getBrokersList();
		for (NeighbourEntry* ne = bList->removeFromFront();ne!=NULL;ne = bList->removeFromFront()){
			if (ne->getNeighbour()!=stn){ //we dont want to subscribe to the requester, even if he's a broker
				send(new SubscriptionMessage(this,topic),ne->getOutGate());
				subscriptionMonitor->subscribe(topic); //if I send it twice, I will set a boolean value twice to true, not a big deal, it just avoid using a flag to indicate that it sent to at least one broker the request
			}
		}
		delete(bList);
	}
	//step 2: finally add the subscription also in our DB
	neighboursMap.addSubscription(stn,topic);
	cancelAndDelete(sm);
}

void AcyclicBroker::handleUnsubscription(UnsubscriptionMessage* um){
	//first and foremost, remove the subscriber from our list
	neighboursMap.removeSubscription(um->getUnsubscriber(),um->getTopic());

	LinkedList<NeighbourEntry>* sList = neighboursMap.getSubscribers(um->getTopic());
	if (sList->size<=0 && subscriptionMonitor->isSubscribed(um->getTopic())){ //means we have no subscriber left for that topic, so we must unsubcribe definitely
		subscriptionMonitor->unsubscribe(um->getTopic());
		LinkedList<NeighbourEntry>* bList = neighboursMap.getBrokersList();
		for (NeighbourEntry* ne = bList->removeFromFront();ne!=NULL;ne = bList->removeFromFront()){
			send(new UnsubscriptionMessage(this,um->getTopic()),ne->getOutGate());
		}
		delete(bList);
	} else if (sList->size==1){ //particular case, CHECK THE PAPER (second conditions checks if the one hanging is a broker, and not a client
		NeighbourEntry* ne = sList->removeFromFront();
		if (dynamic_cast<Client*>(ne->getNeighbour())==NULL){
			send(new UnsubscriptionMessage(this,um->getTopic()),ne->getOutGate());
		}
	}
	delete(sList);
	cancelAndDelete(um);
}

void AcyclicBroker::handleConnectionRequest(ConnectionRequestMessage* crm) {
	//TODO it should be handled the case in which the broker is out of Free Gates, in which case he should return an error Message (not available, something like this)
	STNode* stn = crm->getRequesterNode();
	Broker::handleConnectionRequest(crm);
	cGate* outGate = neighboursMap.getOutputGate(stn);

	//If the connection node is a broker, we must fill him up with our subscriptions
	if (dynamic_cast<Broker*>(stn)!=NULL){
		std::vector<int> subscriptions = neighboursMap.getSubscriptions();
		for (unsigned int i=0;i<subscriptions.size();i++){
			send(new SubscriptionMessage(this,subscriptions[i]),outGate);
		}
	}
}
