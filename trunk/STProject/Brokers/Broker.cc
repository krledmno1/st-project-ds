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

#include "Broker.h"
#include "NSMessage.h"
#include "ConnectionRequestMessage.h"
#include "DisconnectionRequestMessage.h"

Define_Module(Broker);

Broker::Broker() {
	clientsMap = new ClientsMap();
}
Broker::~Broker() {
	cancelAndDelete(wakeUpMsg);
}

void Broker::initialize() {
	scheduleAt(par("WakeUpDelay"), wakeUpMsg);
}
void Broker::handleMessage(cMessage *msg) {
	if (msg == wakeUpMsg) {
		wakeUp();
	} else if (dynamic_cast<NSMessage*>(msg) != NULL) {
		handleNameServerMessage(dynamic_cast<NSMessage*>(msg));
	} else if (dynamic_cast<ConnectionRequestMessage*>(msg) != NULL) {
		handleConnectionRequest(dynamic_cast<ConnectionRequestMessage*>(msg));
	} else if (dynamic_cast<DisconnectionRequestMessage*>(msg) != NULL) {
		handleDisconnectionRequest(dynamic_cast<DisconnectionRequestMessage*>(msg));
	} else {
		EV << "Broker: Unknown message type \n";
	}
}

void Broker::wakeUp() {
	sendDirect(new NSMessage(dynamic_cast<STNode*>(this)), getNSGate());
}

void Broker::handleNameServerMessage(NSMessage* nsm) {
	Broker* requestedNode = dynamic_cast<Broker*>(nsm->getRequestedNode());
	if (requestedNode != NULL && requestedNode != this) {
		cGate* myGate = getFreeOutputGate();
		cGate* hisGate = requestedNode->getFreeInputGate();
		if (myGate == NULL || hisGate == NULL) { //retry later
			scheduleAt(par("WakeUpDelay"), wakeUpMsg);
			return;
		}
		myGate->connectTo(hisGate);
		//TODO handle the case in which you have no free InputGate
		send(new ConnectionRequestMessage(getFreeInputGate(), true), myGate);
		cancelAndDelete(nsm);
	}
}

void Broker::handleConnectionRequest(ConnectionRequestMessage* crm) {
	//TODO here you should separate between client gates, and broker gates (and produce corrisponding mappings, with subscribings, etc.. )
	//TODO also, it should be handled the case in which the broker is out of Free Gates, in which case he should return an error Message (not available, something like this)
	cGate* outGate = getFreeOutputGate();
	outGate->connectTo(crm->getRequesterGate());
	if (crm->isClient()) {
		clientsMap->addMapping(outGate, crm->getRequesterGate());
	}
	cancelAndDelete(crm);
}

void Broker::handleDisconnectionRequest(DisconnectionRequestMessage* drm) {
	cGate* myOutputGate = clientsMap->getBrokerOutPutGate(drm->getRequesterInputGate());
	myOutputGate->disconnect();
	clientsMap->removeMapping(myOutputGate);
	cancelAndDelete(drm);
}

cGate* Broker::getFreeInputGate() {
	int nr = par("nrInputGates");
	for (int i = 0; i < nr; i++) {
		cGate* g = gate("in", i);
		if (!g->isConnected())
			return g;
	}
	return NULL;
}

cGate* Broker::getFreeOutputGate() {
	int nr = par("nrOutputGates");
	for (int i = 0; i < nr; i++) {
		cGate* g = gate("out", i);
		if (!g->isConnected())
			return g;
	}
	return NULL;
}
