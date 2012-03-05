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

#include "Client.h"
#include "Broker.h"
#include "ConnectionRequestMessage.h"
#include "DisconnectionRequestMessage.h"
#include "SubscriptionMessage.h"
#include "PublishMessage.h"
#include "UnsubscriptionMessage.h"
#include <map>

Define_Module(Client);

Client::Client() {
	subscriptionMonitor = new SubscriptionMonitor(NR_TOPICS);
	publishDelayMsg = new cMessage("Publish");
	subscribeDelayMsg = new cMessage("Subscribe");
	unsubscribeDelayMsg = new cMessage("Unsubscribe");
	currentPing = 0;

	vectors.resize(NR_TOPICS);
	for (int i=0;i<NR_TOPICS;i++){
		vectors[i] = new VectorClock();
		vectors[i]->getTimeStamp()->addToBack(new Pair(this,0));
	}
}
Client::~Client() {
	cancelAndDelete(publishDelayMsg);
	cancelAndDelete(subscribeDelayMsg);
	cancelAndDelete(unsubscribeDelayMsg);
}

cGate* Client::getFreeInputGate() {
	int nr = par("nrInputGates");
	for (int i = 0; i < nr; i++) {
		cGate* g = gate("in", i);
		if (!g->isConnected())
			return g;
	}
	return NULL;
}

void Client::initialize() {
	scheduleAt(simTime() + par("WakeUpDelay"), wakeUpDelayMsg);
	delaySignal = registerSignal("PublishDelay");
}
void Client::handleMessage(cMessage* msg) {
	if (msg == wakeUpDelayMsg) {
		wakeUp();
	} else if (msg == sleepDelayMsg) {
		goSleep();
	} else if (msg == subscribeDelayMsg) {
		subscribe();
	} else if (msg == unsubscribeDelayMsg) {
		unsubscribe();
	} else if (msg == publishDelayMsg) {
		publish();
	} else if (dynamic_cast<STMessage*>(msg) != NULL) { //we handle here STMessages
		STMessage* stm = dynamic_cast<STMessage*>(msg);
		if (stm->getType() == stm->NAME_SERVER_MSG) {
			handleNameServerMessage(dynamic_cast<NSMessage*>(msg));
		} else if (stm->getType() == stm->DISCONNECTION_REQUEST_MSG) {
			handleBrokerDisconnectionRequest();
			cancelAndDelete(msg);
		} else if (stm->getType() == stm->NEW_BROKER_NOTIFICATION) {
			handleNewBrokerNotification(
					dynamic_cast<NewBrokerNotificationMessage*>(msg));
		} else if (stm->getType() == stm->PUBLISH_MESSAGE) {
			//EV<< "handling publish message";
			handlePublishMessage(dynamic_cast<PublishMessage*>(msg));
		} else if (stm->getType()==stm->PUBLISH_MESSAGE){
			handlePublishMessage(dynamic_cast<PublishMessage*>(msg));
		} else {
			EV << "Client: Unrecognized STMessage type \n";
			cancelAndDelete(msg);
		}
	} else {
		EV << "Client: Unrecognized message type \n";
		cancelAndDelete(msg);
	}
}

void Client::wakeUp() {
	sendDirect(new NSMessage(dynamic_cast<STNode*>(this)), getNSGate());
}

void Client::goSleep() {
	//disconnect, and wait until he wakes up again
	//we first send through the existing channel the request to the other party to disconnect his "way". Equivalent to gate("in")->disconnect(); which unfortunately doesn't work. Seems that only outPut gates can disconnect
	cancelEvent(subscribeDelayMsg);
	cancelEvent(unsubscribeDelayMsg);
	cancelEvent(publishDelayMsg);
	cancelEvent(wakeUpDelayMsg);
	subscriptionMonitor->unsubscribeAll();
	releaseMessages();
	send(new DisconnectionRequestMessage(this), gate("out"));
	gate("out")->disconnect();
	scheduleAt(simTime() + par("WakeUpDelay"), wakeUpDelayMsg);
}

void Client::subscribe() {
	int topicChosen = subscriptionMonitor->getRandomUnsubscribedTopic();
	if (topicChosen < 0) {
		//EV << "I got all subscriptions. Can't subscribe to anything anymore";
		return;
	}
	subscriptionMonitor->subscribe(topicChosen);
	//EV << " Subscribing To " << topicChosen << " (" << this << ")\n";
	send(new SubscriptionMessage(this, topicChosen), gate("out"));
	scheduleAt(simTime() + par("SubscriptionPeriod"), subscribeDelayMsg);
}
void Client::unsubscribe() {
	int topicChosen = subscriptionMonitor->getRandomSubscribedTopic();
	if (topicChosen < 0) {
		//EV << "I'm not subscribed to anything yet";
		return;
	}
	subscriptionMonitor->unsubscribe(topicChosen);
	//WE MUST ALSO DELETE THE VECTORCLOCK (and create another one for further use)
	VectorClock* toBeDeleted = vectors[topicChosen];
	int value = toBeDeleted ->search(this)->getValue();
	delete(toBeDeleted);
	VectorClock* newVectorClock = new VectorClock();
	newVectorClock->timeStamp->addToBack(new Pair(this,value));
	vectors[topicChosen] = newVectorClock;

	//EV << this << " Unsubscribing to" << topicChosen;
	send(new UnsubscriptionMessage(this, topicChosen), gate("out"));
	scheduleAt(simTime() + par("UnSubscriptionPeriod"), unsubscribeDelayMsg);
}

void Client::flushVectorClocks() {
	for (int i=0;i<NR_TOPICS;i++){
		VectorClock* toBeDeleted = vectors[i];
		int value = toBeDeleted ->search(this)->getValue();
		delete(toBeDeleted);
		VectorClock* newVectorClock = new VectorClock();
		newVectorClock->timeStamp->addToBack(new Pair(this,value));
		vectors[i] = newVectorClock;
	}
}

/* This is also the procedure which activates the Node. Wakeup only contacts the NS */
void Client::handleNameServerMessage(NSMessage* nsm) {
	//this is the reply we get from NS when we ask for a broker
	LinkedList<STNode>* requestedNodes =
			dynamic_cast<LinkedList<STNode>*>(nsm->getRequestedNodes());
	if (!requestedNodes->isEmpty()) {
		Node<STNode>* requestedNode = requestedNodes->start;
		Broker* best = NULL;
		double bestDelay = simulation.getSystemModule()->getSubmodule(
				"nameServer", 0)->par("maxDelay");
		bestDelay++;
		for (int i = 0; i < requestedNodes->getSize(); i++) {
			Broker* candidate =
					dynamic_cast<Broker*>(requestedNode->getContent());
			double candidateDelay = this->ping(candidate);
			if (candidateDelay < bestDelay) {
				best = candidate;
				bestDelay = candidateDelay;
			}
			requestedNode = requestedNode->getNext();
		}
		if (best != NULL) //if there exists best one
		{

			cGate* myGate = gate("out");
			cGate* hisGate = best->getFreeInputGate();
			if (myGate == NULL || hisGate == NULL) {
				//try later
				cancelEvent(wakeUpDelayMsg);
				scheduleAt(simTime() + par("WakeUpDelay"), wakeUpDelayMsg);
				return;
			}

			cDelayChannel* cha = cDelayChannel::create("DelayChannel");
			cha->setDelay(bestDelay);
			currentPing = bestDelay;

			myGate->connectTo(hisGate, cha);

			send(new ConnectionRequestMessage(this), myGate);
			cancelAndDelete(nsm);
			//if I was redirected, I should resubscribe to all my topics
			for (int i = 0; i < NR_TOPICS; i++) {
				if (subscriptionMonitor->isSubscribed(i)) {
					send(new SubscriptionMessage(this, i), gate("out"));
				}
			}
			//now "decide" for how long it will run
			scheduleAt(simTime() + par("SubscriptionPeriod"),
					subscribeDelayMsg);
			scheduleAt(simTime() + par("UnSubscriptionPeriod"),
					unsubscribeDelayMsg);
			scheduleAt(simTime() + par("PublishPeriod"), publishDelayMsg);
			scheduleAt(simTime() + par("SleepDelay"), sleepDelayMsg);
			return;

		} else {
			cancelEvent(wakeUpDelayMsg);
			scheduleAt(simTime() + par("WakeUpDelay"), wakeUpDelayMsg);
			return;
		}
	} else {
		cancelEvent(wakeUpDelayMsg);
		scheduleAt(simTime() + par("WakeUpDelay"), wakeUpDelayMsg);
		return;
	}

}

void Client::handleBrokerDisconnectionRequest() { //if a broker wishes to disconnect, it is client's task to find another broker
	//this will be a sort of goSleep() method, only that it will try to wake up much sooner
	cancelEvent(subscribeDelayMsg);
	cancelEvent(unsubscribeDelayMsg);
	cancelEvent(publishDelayMsg);
	cancelEvent(sleepDelayMsg); //because if it comes before the Reconnection wake up, we end up wanting to disconnect when we are already disconnected
	gate("out")->disconnect();
	releaseMessages();
	cancelEvent(wakeUpDelayMsg);
	scheduleAt(simTime() + par("ReconnectDelay"), wakeUpDelayMsg);
}

void Client::handleNewBrokerNotification(NewBrokerNotificationMessage* m) {
	double newPing = ping(m->getJoiningBroker());
	if (newPing < currentPing) {
		cGate* hisGate = m->getJoiningBroker()->getFreeInputGate();
		if (hisGate == NULL) { //he's full, we cancel reconnection attempt
			cancelAndDelete(m);
			return;
		}
		send(new DisconnectionRequestMessage(this), gate("out"));
		gate("out")->disconnect();

		cDelayChannel* cha = cDelayChannel::create("DelayChannel");
		cha->setDelay(newPing);
		currentPing = newPing;

		cGate* myGate = gate("out");
		myGate->connectTo(hisGate, cha);

		send(new ConnectionRequestMessage(this), myGate);
		for (int i = 0; i < NR_TOPICS; i++) {
			if (subscriptionMonitor->isSubscribed(i)) {
				send(new SubscriptionMessage(this, i), gate("out"));
			}
		}
	}
	cancelAndDelete(m);
}

void Client::publish() {
	//int topicChosen = rand() % (NR_TOPICS);
	//We allow publishing only on subscribed channels
	int topicChosen = subscriptionMonitor->getRandomSubscribedTopic();
	if (topicChosen<0){
		//we're not subscribed anywhere, retry later
		scheduleAt(simTime() + par("PublishPeriod"), publishDelayMsg);
		return;
	}
	//EV << "Publishing " << topicChosen << "\n";

	VectorClock* timeStamp = vectors[topicChosen];
	if (timeStamp==NULL){
		EV << "Client: timestamp not found for a given topic";
		return;
	}
	Pair *p = timeStamp->search(this);
	if (p == NULL) {
		EV << "I cannot find myself\n";
		return;
	}
	p->setValue(p->getValue() + 1);
	timeStamp->printClock();
	send(new PublishMessage(this, topicChosen, timeStamp), gate("out"));
	scheduleAt(simTime() + par("PublishPeriod"), publishDelayMsg);
}

void Client::handlePublishMessage(PublishMessage *pm) {
	if (checkReceiveCondition(pm) == true) {
		exportMessage(pm);
		checkPostponed();
	} else {
		EV << "Client: Postponed message \n";
		postponedMessages.addToBack(pm);
	}
}

void Client::exportMessage(PublishMessage *pm) {
	//EV << "Received msg \n"; //<< nsm->
	simtime_t messageTime = pm->getCreationTime();
	simtime_t now = simTime();
	//EV << now-messageTime;
	emit(delaySignal,now-messageTime);
	cancelAndDelete(pm);
}

bool Client::checkReceiveCondition(PublishMessage *msg) {
	VectorClock* timeStamp = vectors[msg->getTopic()];
	if (timeStamp==NULL || msg->getTimeStamp()==NULL){
		EV << "timeStamp NULL for a given topic in receiveCondition";
		return true;
	}
	Pair *oldSenderPair = timeStamp->search(msg->getOriginalSender());

	Pair *newSenderPair = msg->getTimeStamp()->search(msg->getOriginalSender());

	if (oldSenderPair != NULL) {EV << "Checking received condition, with sender " << oldSenderPair->getNode() << "\n";}
	else {EV << this << ": Checking received condition, with new sender " << "\n";}
	EV << "My timestamp: ";timeStamp->printClock();
	EV << "His timestamp: " ; msg->getTimeStamp()->printClock();

	if (oldSenderPair == NULL) {
		timeStamp->update(msg->getTimeStamp());
		EV << "Outcome: merged "; timeStamp->printClock();
		return true;
	}

	/*Debug*/if (newSenderPair == NULL) {EV << "Client: Error publish";return false;}

	if (newSenderPair->getValue() != oldSenderPair->getValue() + 1){
		EV << "CheckReceive: not direct sender followup";
		return false;
	}

	Node<Pair>* externalPairIterator = msg->getTimeStamp()->getTimeStamp()->start;

	/*Debug*/if (externalPairIterator == NULL) {EV << "ERROR empty message \n";return false;}

	while (externalPairIterator != NULL) {
		/*Sender skip*/if (externalPairIterator->getContent()->getNode() == oldSenderPair->getNode()) {externalPairIterator = externalPairIterator->getNext(); continue; }

		/*debug*/if (externalPairIterator->getContent() == NULL){EV << "Client: Entry with null content\n"; continue;}
		STNode *node = externalPairIterator->getContent()->getNode(); //not sender
		Pair *nodeMatch = timeStamp->search(node);
		if (nodeMatch == NULL){
			EV << "Client: NEW NODE DETECTED, ACCEPTING THE MESSAGE"; //yet we dont merge it
			return true;
		}
		if (nodeMatch->getValue() < externalPairIterator->getContent()->getValue()) {
			EV << "In iteration condition broke\n";
			return false;
		}
		externalPairIterator = externalPairIterator->getNext();
	}
	//we merge it
	timeStamp->update(msg->getTimeStamp());
	EV << "Outcome: true: "; timeStamp->printClock();
	return true;
}

void Client::checkPostponed() {
	Node<PublishMessage> *pmIterator = postponedMessages.start;
	LinkedList<PublishMessage> exportedMessages;
	while (pmIterator != NULL) {
		if (checkReceiveCondition(pmIterator->getContent())){
			exportedMessages.addToBack(pmIterator->getContent());
		}
		pmIterator = pmIterator->getNext();
	}
	int counter = 0;
	Node<PublishMessage>* exportedMessagesIterator = exportedMessages.start;
	while (exportedMessagesIterator!=NULL){
		counter++;
		PublishMessage* pm = exportedMessagesIterator->getContent();
		postponedMessages.removeNode(pm);
		exportedMessagesIterator = exportedMessagesIterator->getNext();
		exportMessage(pm);
	}
	if (counter>0){
		EV << "***********UNLOCKED " << counter << " messages!!";
		checkPostponed();
	}
}

void Client::releaseMessages() {
	while (postponedMessages.size>0){
		checkPostponed();
		flushVectorClocks();
	}
}
