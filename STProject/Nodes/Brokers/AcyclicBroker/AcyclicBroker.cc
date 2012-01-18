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
	std::vector<NeighbourEntry*> brokersVector = neighboursMap.getBrokersVector();
	if (brokersVector.size()<=0) { //it means we cannot sleep, we reschedule the sleep
		scheduleAt(simTime() + par("SleepDelay"), sleepDelayMsg);
		EV << "I cannot sleep, I'm alone";
		return;
	}
	//step1: we need to redirect every other Broker, in order NOT to break the chain
	//TODO Once we implement the "network conditions", this will become much more complicated (we'll try to link the Brokers in such a way as to minimize the latency between them)
	if (brokersVector.size()>1){ //cause if we're connected with only 1 broker we're not breaking any chain
		for (unsigned int i=1;i<brokersVector.size();i++){
			if (brokersVector[i]!=NULL && brokersVector[i-1]!=NULL){
				send(new ConnectionRequestMessage(brokersVector[i]->getNeighbour()),brokersVector[i-1]->getOutGate());
				send(new ConnectionRequestMessage(brokersVector[i-1]->getNeighbour()),brokersVector[i]->getOutGate());
			} else {
				EV << "AcyclicBroker: brokersVector got NULL entries";
			}

		}
	}
	//step2: unregister from NameServ through a Disconnect message, such that we do not receive any more connection requests from either brokers or clients
	sendDirect(new DisconnectionRequestMessage(this), getNSGate());
	//step3: send disconnection requests to all neighbors
	std::vector<NeighbourEntry*> neighbours = neighboursMap.getNeighboursVector();
	for (unsigned int i = 0; i < neighbours.size(); i++) {
		if (neighbours[i] != NULL) {
			send(new DisconnectionRequestMessage(this), neighbours[i]->getOutGate());
			neighbours[i]->getOutGate()->disconnect();
			neighboursMap.removeMapping(neighbours[i]->getNeighbour()); //this entry will become null
		}
	}
	//step4: schedule a rewake
	scheduleAt(simTime() + par("WakeUpDelay"), wakeUpDelayMsg);
}

void AcyclicBroker::handleDisconnectionRequest(DisconnectionRequestMessage* drm){//primarily is the same the basic broker, just that I must also check any possible unsubscriptions
	std::vector<int> hisSubscriptions = neighboursMap.getSubscriptions(drm->getRequesterNode());
	Broker::handleDisconnectionRequest(drm); //This is the call to super class (Broker) to disconnect the requesting side, and remove entry
	//now decide if we must unsubscribe to any particular topics (since we have no client subscribed to a given topic, if thats the case)
	for (unsigned int i=0;i<hisSubscriptions.size();i++){
		std::vector<NeighbourEntry*> subscribers = neighboursMap.getSubscribers(hisSubscriptions[i]);
		if (subscribers.size()<=0 && subscriptionMonitor->isSubscribed(hisSubscriptions[i])){ //means we have no subscriber left for that topic, so we must unsubcribe definitely
			subscriptionMonitor->unsubscribe(hisSubscriptions[i]);
			std::vector<NeighbourEntry*> brokers = neighboursMap.getBrokersVector();
			for (unsigned int i=0; i<brokers.size();i++){
				send(new UnsubscriptionMessage(this,hisSubscriptions[i]),brokers[i]->getOutGate());
			}
		}
	}
}

void AcyclicBroker::handlePublish(PublishMessage* pm){
	STNode* stn = pm->getSender();
	int topic = pm->getTopic();
	std::vector<NeighbourEntry*> subscribers = neighboursMap.getSubscribers(pm->getTopic());
	for (unsigned int i=0; i<subscribers.size();i++){
		if (subscribers[i]->getNeighbour()!=stn){ //we dont send back messages
			send (new PublishMessage(this,topic),subscribers[i]->getOutGate());
		}
	}
	cancelAndDelete(pm);
}

void AcyclicBroker::handleSubscription(SubscriptionMessage* sm){
	STNode* stn = sm->getSubscriber();
	int topic = sm->getTopic();
	//step1: if I'm not already subscribed to the topic, subscribe to this topic to my neighbouring brokers
	if (!neighboursMap.isSubscribed(stn, topic)){//otherwise no use to repropagate
		std::vector<NeighbourEntry*> brokers = neighboursMap.getBrokersVector();
		for (unsigned int i=0; i<brokers.size();i++){
			if (brokers[i]->getNeighbour()!=stn){ //we dont want to subscribe to the requester, even if he's a broker
				send(new SubscriptionMessage(this,topic),brokers[i]->getOutGate());
				subscriptionMonitor->subscribe(topic); //if I send it twice, I will set a boolean value twice to true, not a big deal, it just avoid using a flag to indicate that it sent to at least one broker the request
			}
		}
	}
	//step 2: finally add the subscription also in our DB
	neighboursMap.addSubscription(stn,topic);
	cancelAndDelete(sm);
}

void AcyclicBroker::handleUnsubscription(UnsubscriptionMessage* um){
	//first and foremost, remove the subscriber from our list
	neighboursMap.removeSubscription(um->getUnsubscriber(),um->getTopic());

	std::vector<NeighbourEntry*> subscribers = neighboursMap.getSubscribers(um->getTopic());
	if (subscribers.size()<=0 && subscriptionMonitor->isSubscribed(um->getTopic())){ //means we have no subscriber left for that topic, so we must unsubcribe definitely
		subscriptionMonitor->unsubscribe(um->getTopic());
		std::vector<NeighbourEntry*> brokers = neighboursMap.getBrokersVector();
		for (unsigned int i=0; i<brokers.size();i++){
			send(new UnsubscriptionMessage(this,um->getTopic()),brokers[i]->getOutGate());
		}
	} else if (subscribers.size()==1 && dynamic_cast<Client*>(subscribers[0]->getNeighbour())==NULL) { //particular case, CHECK THE PAPER (second conditions checks if the one hanging is a broker, and not a client
		send(new UnsubscriptionMessage(this,um->getTopic()),subscribers[0]->getOutGate());
	}
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
