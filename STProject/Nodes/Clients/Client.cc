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

Define_Module(Client);

Client::Client() {
}

Client::~Client() {
	cancelAndDelete(wakeUpMsg);
	cancelAndDelete(sleepMsg);
}

cGate* Client::getFreeInputGate(){
	return gate("in");
}

void Client::initialize() {
	scheduleAt(simTime() + par("WakeUpDelay"), wakeUpMsg);
}
void Client::handleMessage(cMessage *msg) {
	if (msg == wakeUpMsg) {
		wakeUp();
	} else if (msg == sleepMsg) {
		goSleep();
	} else if (dynamic_cast<STMessage*>(msg) != NULL) { //we handle here STMessages
		STMessage* stm = dynamic_cast<STMessage*>(msg);
		if (stm->getType()==stm->NAME_SERVER_MSG){
			handleNameServerMessage(dynamic_cast<NSMessage*>(msg));
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
	send(new DisconnectionRequestMessage(this), gate("out"));
	gate("out")->disconnect();
	scheduleAt(simTime() + par("WakeUpDelay"), wakeUpMsg);
}

void Client::handleNameServerMessage(NSMessage* nsm) { //this is the reply we get from NS when we ask for a broker
	Broker* requestedNode = dynamic_cast<Broker*>(nsm->getRequestedNode()); //instead of casting the message I use directly request object
	if (requestedNode == NULL) { //try later
		scheduleAt(simTime() + par("WakeUpDelay"), wakeUpMsg);
		return;
	}
	cGate* myGate = gate("out");
	cGate* hisGate = requestedNode->getFreeInputGate();
	if (myGate == NULL || hisGate == NULL) { //try later
		scheduleAt(simTime() + par("WakeUpDelay"), wakeUpMsg);
		return;
	}
	myGate->connectTo(hisGate);
	send(new ConnectionRequestMessage(this), myGate);
	cancelAndDelete(nsm);
	//now "decide" for how long it will run
	scheduleAt(simTime() + par("SleepDelay"), sleepMsg);
}