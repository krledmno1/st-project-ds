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

	timeStamp = new VectorClock();
	Pair *pair = new Pair(this,0);
	timeStamp->getTimeStamp()->addToBack(pair);

//	ts.addToBack(pair);
//	timeStamp.setTimeStamp(ts);


}
Client::~Client() {
	delete timeStamp;
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
		EV << stm->getType();
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
	subscriptionMonitor->unsubscribeAll();
	send(new DisconnectionRequestMessage(this), gate("out"));
	gate("out")->disconnect();
	scheduleAt(simTime() + par("WakeUpDelay"), wakeUpDelayMsg);
}

void Client::subscribe() {
	int topicChosen = subscriptionMonitor->getRandomUnsubscribedTopic();
	if (topicChosen < 0) {
		EV << "I got all subscriptions. Can't subscribe to anything anymore";
		return;
	}
	subscriptionMonitor->subscribe(topicChosen);
	EV << "Subscribing " << topicChosen;
	send(new SubscriptionMessage(this, topicChosen), gate("out"));
	scheduleAt(simTime() + par("SubscriptionPeriod"), subscribeDelayMsg);
}
void Client::unsubscribe() {
	int topicChosen = subscriptionMonitor->getRandomSubscribedTopic();
	if (topicChosen < 0) {
		EV << "I'm not subscribed to anything yet";
		return;
	}
	subscriptionMonitor->unsubscribe(topicChosen);
	EV << "Unsubscribing " << topicChosen;
	send(new UnsubscriptionMessage(this, topicChosen), gate("out"));
	scheduleAt(simTime() + par("UnSubscriptionPeriod"), unsubscribeDelayMsg);
}
void Client::publish() {
	int topicChosen = rand() % (NR_TOPICS);
	EV << "Publishing " << topicChosen;
	Pair *p = timeStamp->search(this);
	if (p == NULL) {
		EV << "I cannot find myself\n";
		return;
	}
	p->setValue(p->getValue() + 1);
	send(new PublishMessage(this, topicChosen, timeStamp), gate("out"));
	scheduleAt(simTime() + par("PublishPeriod"), publishDelayMsg);
}

void Client::handlePublishMessage(PublishMessage* nsm) {

	if (checkReceiveCondition(nsm) == true) {
		EV << "Received msg \n"; //<< nsm->

		checkPostponed();
	} else {
		EV << "postponed \n";
		postponedMessages.addToBack(nsm);
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
			scheduleAt(simTime() + par("WakeUpDelay"), wakeUpDelayMsg);
			return;
		}
	} else {
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

bool Client::checkReceiveCondition(PublishMessage *msg) {

	//first cont
	Pair *internalTimestamp = timeStamp->search(msg->getSender());
	Pair *msgTimestamp = msg->getTimeStamp()->search(msg->getSender());

	if (internalTimestamp == NULL) {
		timeStamp->update(msg->getTimeStamp());
		checkPostponed();
		return true;
	}

	if (msgTimestamp == NULL) {
		EV << "error publish";
		return false;
	}

	if (msgTimestamp->getValue() != internalTimestamp->getValue() + 1)
		return false;

	Node<Pair> *externalPair = msg->getTimeStamp()->getTimeStamp()->start;

	if (externalPair == NULL) {
		EV << "ERROR empty message \n";
		return false;
	}

	while (externalPair->getNext() != NULL) {
		if (externalPair->getContent() == internalTimestamp) {
			externalPair = externalPair->getNext();
			continue;
		}

		STNode *otherowner = externalPair->getContent()->getNode(); //not sender
		Pair *internal = timeStamp->search(otherowner);
		if (internal->getValue() > externalPair->getContent()->getValue()) {

			return false;
		}
		externalPair = externalPair->getNext();
	}

	return true;
}

void Client::checkPostponed() {

	Node<PublishMessage> *pm = postponedMessages.start;
	EV << "check postpones \n";
	while (pm != NULL) {
		if (checkReceiveCondition(pm->getContent()))
			handlePublishMessage(pm->getContent());
		pm = pm->getNext();
	}

}

