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

Define_Module(Client);

Client::Client() {
	subscriptionMonitor = new SubscriptionMonitor(NR_TOPICS);
	publishDelayMsg = new cMessage("Publish");
	subscribeDelayMsg = new cMessage("Subscribe");
	unsubscribeDelayMsg = new cMessage("Unsubscribe");
}
Client::~Client() {
	cancelAndDelete(publishDelayMsg);
	cancelAndDelete(subscribeDelayMsg);
	cancelAndDelete(unsubscribeDelayMsg);
}

cGate* Client::getFreeInputGate(){
	return gate("in");
}

void Client::initialize() {
	scheduleAt(simTime() + par("WakeUpDelay"), wakeUpDelayMsg);
}
void Client::handleMessage(cMessage *msg) {
	if (msg == wakeUpDelayMsg) {
		wakeUp();
	} else if (msg == sleepDelayMsg) {
		goSleep();
	} else if (msg == subscribeDelayMsg){
		subscribe();
	} else if (msg == unsubscribeDelayMsg){
		unsubscribe();
	} else if (msg == publishDelayMsg){
		publish();
	} else if (dynamic_cast<STMessage*>(msg) != NULL) { //we handle here STMessages
		STMessage* stm = dynamic_cast<STMessage*>(msg);
		if (stm->getType()==stm->NAME_SERVER_MSG){
			handleNameServerMessage(dynamic_cast<NSMessage*>(msg));
		} else if (stm->getType()==stm->DISCONNECTION_REQUEST_MSG){
			handleBrokerDisconnectionRequest();
			cancelAndDelete(msg);
		} else {
			EV << "Client: Unrecognized STMessage type \n";
		}
	} else {
		EV << "Client: Unrecognized message type \n";
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

void Client::subscribe(){
	int topicChosen = subscriptionMonitor->getRandomUnsubscribedTopic();
	if (topicChosen<0){
		EV << "I got all subscriptions. Can't subscribe to anything anymore";
		return;
	}
	subscriptionMonitor->subscribe(topicChosen);
	EV << "Subscribing " << topicChosen;
	send(new SubscriptionMessage(this,topicChosen), gate("out"));
	scheduleAt(simTime() + par("SubscriptionPeriod"),subscribeDelayMsg);
}
void Client::unsubscribe(){
	int topicChosen = subscriptionMonitor->getRandomSubscribedTopic();
	if (topicChosen<0){
		EV << "I'm not subscribed to anything yet";
		return;
	}
	subscriptionMonitor->unsubscribe(topicChosen);
	EV << "Unsubscribing " << topicChosen;
	send(new UnsubscriptionMessage(this,topicChosen),gate("out"));
	scheduleAt(simTime() + par("UnSubscriptionPeriod"),unsubscribeDelayMsg);
}
void Client::publish(){
	int topicChosen = rand() % (NR_TOPICS);
	EV << "Publishing " << topicChosen;
	send(new PublishMessage(this, topicChosen),gate("out"));
	scheduleAt(simTime() + par("PublishPeriod"),publishDelayMsg);
}
/* This is also the procedure which activates the Node. Wakeup only contacts the NS */
void Client::handleNameServerMessage(NSMessage* nsm) { //this is the reply we get from NS when we ask for a broker
	STNode* stn = nsm->getRequestedNode();
	Broker* requestedNode = dynamic_cast<Broker*>(stn);
	if (requestedNode == NULL) { //try later
		scheduleAt(simTime() + par("WakeUpDelay"), wakeUpDelayMsg);
		return;
	}
	cGate* myGate = gate("out");
	cGate* hisGate = requestedNode->getFreeInputGate();
	if (myGate == NULL || hisGate == NULL) { //try later
		scheduleAt(simTime() + par("WakeUpDelay"), wakeUpDelayMsg);
		return;
	}
	myGate->connectTo(hisGate);
	send(new ConnectionRequestMessage(this), myGate);
	cancelAndDelete(nsm);
	//if I was redirected, I should resubscribe to all my topics
	for (int i=0;i<NR_TOPICS;i++){
		if (subscriptionMonitor->isSubscribed(i)){
			send(new SubscriptionMessage(this,i), gate("out"));
		}
	}
	//now "decide" for how long it will run
	scheduleAt(simTime() + par("SubscriptionPeriod"),subscribeDelayMsg);
	scheduleAt(simTime() + par("UnSubscriptionPeriod"),unsubscribeDelayMsg);
	scheduleAt(simTime() + par("PublishPeriod"),publishDelayMsg);
	scheduleAt(simTime() + par("SleepDelay"), sleepDelayMsg);
}

void Client::handleBrokerDisconnectionRequest(){ //if a broker wishes to disconnect, it is client's task to find another broker
	//this will be a sort of goSleep() method, only that it will try to wake up much sooner
	cancelEvent(subscribeDelayMsg);
	cancelEvent(unsubscribeDelayMsg);
	cancelEvent(publishDelayMsg);
	cancelEvent(sleepDelayMsg); //because if it comes before the Reconnection wake up, we end up wanting to disconnect when we are already disconnected
	gate("out")->disconnect();
	scheduleAt(simTime() + par("ReconnectDelay"), wakeUpDelayMsg);
}
