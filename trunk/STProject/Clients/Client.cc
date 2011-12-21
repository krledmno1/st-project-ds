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

void Client::initialize(){
	scheduleAt(simTime()+par("WakeUpDelay"),wakeUpMsg);
}
void Client::handleMessage(cMessage *msg){
	if (msg==wakeUpMsg){
		wakeUp();
	} else if (msg==sleepMsg){
		//disconnect, and wait until he wakes up again
		//we first send through the existing channel the request to the other party to disconnect his "way". Equivalent to gate("in")->disconnect(); which unfortunately doesn't work. Seems that only outPut gates can disconnect
		send(new DisconnectionRequestMessage(gate("in")),gate("out"));
		gate("out")->disconnect();
		//gate("in")->disconnect();
		scheduleAt(simTime()+par("WakeUpDelay"),wakeUpMsg);
	} else if (dynamic_cast<NSMessage*>(msg)!=NULL){
		NSMessage* replyMsg = dynamic_cast<NSMessage*>(msg);
		Broker* requestedNode = dynamic_cast<Broker*>(replyMsg->getRequestedNode());//instead of casting the message I use directly request object
		if (requestedNode==NULL){//try later
			scheduleAt(simTime()+par("WakeUpDelay"),wakeUpMsg);
			return;
		}
		cGate* myGate = gate("out");
		cGate* hisGate = requestedNode->getFreeInputGate();
		if (myGate == NULL || hisGate == NULL){ //try later
			scheduleAt(simTime()+par("WakeUpDelay"),wakeUpMsg);
			return;
		}
		myGate->connectTo(hisGate);
		send(new ConnectionRequestMessage(gate("in"),true),myGate);
		EV << "I should have sent a new Message \n";
		cancelAndDelete(replyMsg);
		//now "decide" for how long it will run
		scheduleAt(simTime()+par("SleepDelay"),sleepMsg);
	} else {
		EV << "Unrecognized message type \n";
	}
}

void Client::wakeUp(){
	sendDirect(new NSMessage(dynamic_cast<STNode*>(this)),getNSGate());
}
