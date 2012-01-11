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

AcyclicBroker::AcyclicBroker() {}
AcyclicBroker::~AcyclicBroker() {}

void AcyclicBroker::sleep(){
	//EV << "AcyclicBroker: Method Sleep"; //check if virtualization works.. it does...
	//broker disconnection is a lot messier than client disconnection.
	//step0: decide if I can sleep (if I'm the only broker present, means I cannot sleep) Thus, I check if I have any connected Brokers
	//TODO what if in the same time 2 connected brokers decide to disconnect? Hell will break loose, we need to use a sort of "locking" mechanism in this
	//std::vector<NeighbourEntry*>* brokersVectorPointer = ;
	std::vector<NeighbourEntry*> brokersVector = neighboursMap.getBrokersVector();
	if (brokersVector.size()<=0) { //it means we cannot sleep, we reschedule the sleep
		scheduleAt(simTime() + par("SleepDelay"), sleepDelayMsg);
		EV << "Cannot sleep, I'm alone";
		//delete(brokersVectorPointer); //stupid garbage collection
		return;
	}
	//step1: we need to redirect every other Broker, in order not to break the chain
	if (brokersVector.size()>1){ //if we're connected with only 1 broker we're not breaking any chain
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

void AcyclicBroker::handlePublish(PublishMessage* pm){
	STNode* stn = pm->getSender();
	int topic = pm->getTopic();
	std::vector<NeighbourEntry*> subscribers = neighboursMap.getSubscribers(pm->getTopic());
	//if (subscribers.size()==0 && dynamic_cast<Client*>(stn)==NULL){
	//	EV << "FATAL ERROR, NO SUBSCRIBERS.FATAL ERROR, NO SUBSCRIBERS.FATAL ERROR, NO SUBSCRIBERS.FATAL ERROR, NO SUBSCRIBERS.FATAL ERROR, NO SUBSCRIBERS.FATAL ERROR, NO SUBSCRIBERS.FATAL ERROR, NO SUBSCRIBERS.FATAL ERROR, NO SUBSCRIBERS.FATAL ERROR, NO SUBSCRIBERS.FATAL ERROR, NO SUBSCRIBERS.FATAL ERROR, NO SUBSCRIBERS.FATAL ERROR, NO SUBSCRIBERS.FATAL ERROR, NO SUBSCRIBERS.FATAL ERROR, NO SUBSCRIBERS.FATAL ERROR, NO SUBSCRIBERS.FATAL ERROR, NO SUBSCRIBERS.FATAL ERROR, NO SUBSCRIBERS.FATAL ERROR, NO SUBSCRIBERS.FATAL ERROR, NO SUBSCRIBERS.FATAL ERROR, NO SUBSCRIBERS.FATAL ERROR, NO SUBSCRIBERS.FATAL ERROR, NO SUBSCRIBERS.";
	//}
	for (unsigned int i=0; i<subscribers.size();i++){
		if (subscribers[i]->getNeighbour()!=stn){
			send (new PublishMessage(this,topic),subscribers[i]->getOutGate());
		}
	}
	cancelAndDelete(pm);
}

/* COMMENTS FOR THE ELSE PART:
 * There is a trick in propagating Subscriptions
 * 1. Once i get a subscription, I propagate the subscriptions in the name of my subscriptor.
 * BUT IF I RECEIVE SUBSCRIPTION FROM A BROKER, it doesnt mean that I will receive the topic from that broker.
 * Imagine the scenario where a brokers asks me to look after topic 0, I propagate the subscription to all the other brokers, and then a client asks me to look after topic 0.
 * If I check and see that I got already subscribers on topic 0 I stay calm and say: ok, its gonna come. WRONG
 * The broker who sent me that subscription in the first place WONT send me publications on topic 0 UNLESS I ask him for that.
 * Solution: the ELSE PART. If I have no client asking already for topic 0, I'm going to repropagate the subscription.
 */
void AcyclicBroker::handleSubscription(SubscriptionMessage* sm){
	STNode* stn = sm->getSubscriber();
	int topic = sm->getTopic();
	//step 0: if someone asks me to look after a topic, and I wasn't looking already after that topic, I become interested in that topic so I notify my neighbours
	if (!neighboursMap.hasClientSubscribers(topic)){
		std::vector<NeighbourEntry*> brokers = neighboursMap.getBrokersVector();
		for (unsigned int i=0; i<brokers.size();i++){
			if (brokers[i]->getNeighbour()!=stn){ //we dont want to send it back to where it came from
				send(new SubscriptionMessage(this,topic),brokers[i]->getOutGate());
			}
		}
	}
	//step 1: add the subscription also in our DB
	neighboursMap.addSubscription(stn,topic);
	cancelAndDelete(sm);
}

void AcyclicBroker::handleUnsubscription(UnsubscriptionMessage* um){
	neighboursMap.removeSubscription(um->getUnsubscriber(),um->getTopic());
	std::vector<NeighbourEntry*> subscribers = neighboursMap.getSubscribers(um->getTopic());
	if (subscribers.size()<=0){ //means we have no subscriber left for that
		std::vector<NeighbourEntry*> brokers = neighboursMap.getBrokersVector();
			for (unsigned int i=0; i<brokers.size();i++){
				if (brokers[i]->getNeighbour()!=um->getUnsubscriber()){ //we dont want to send it back to where it came from
				send(new UnsubscriptionMessage(this,um->getTopic()),brokers[i]->getOutGate());
			}
		}
	//} else if (subscribers.size()==1 && dynamic_cast<Client*>(subscribers[0])==NULL){
		//means we have a hanging broker. We should unsubscribe from him
	//	send (new UnsubscriptionMessage(this,um->getTopic()),subscribers[0]->getOutGate());
	}
	cancelAndDelete(um);
}

void AcyclicBroker::handleConnectionRequest(ConnectionRequestMessage* crm) {
	//TODO here you should separate between client gates, and broker gates (and produce corrisponding mappings, with subscribings, etc.. )
	//TODO also, it should be handled the case in which the broker is out of Free Gates, in which case he should return an error Message (not available, something like this)
	STNode* stn = crm->getRequesterNode();
	cGate* outGate = getFreeOutputGate();
	outGate->connectTo(stn->getFreeInputGate());
	neighboursMap.addMapping(stn, outGate);

	if (dynamic_cast<Broker*>(stn)!=NULL){//means we must fill him up with our subscriptions
		std::vector<int> subscriptions = neighboursMap.getSubscriptions();
		for (unsigned int i=0;i<subscriptions.size();i++){
			send(new SubscriptionMessage(this,subscriptions[i]),outGate);
		}
	}
	cancelAndDelete(crm);
}
