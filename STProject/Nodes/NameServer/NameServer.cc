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

#include "NameServer.h"
#include "STNode.h"
#include "Client.h"
#include <vector>

Define_Module(NameServer);

//____________Construction
NameServer::NameServer() {}
NameServer::~NameServer() {}

void NameServer::initialize() {
	setNSGate(gate("updIn"));
	srand(time(0));
}
void NameServer::handleMessage(cMessage* msg) { //NameServer only uses NSMessage*. Using type constants would only clutter the code
	NSMessage* ns = dynamic_cast<NSMessage*>(msg);
	if (ns == NULL) {
		return;
	}
	if (dynamic_cast<Broker*>(ns->getRequester()) != NULL) {
		handleBrokerRequest(ns);
	} else if (dynamic_cast<Client*>(ns->getRequester()) != NULL) {
		handleClientRequest(ns);
	}
}

void NameServer::handleBrokerRequest(NSMessage* msg) {
	Broker* b = NULL;
	if (brokersVector.size() == 0) {
		b = NULL;
	} else {
		if (brokersVector.size() == 1) {
			b = brokersVector[0];
		} else { //size > 1, which means we pick one random
			b = brokersVector[rand() % brokersVector.size()]; // - 1
		}
		EV << "Its greater";
	}
//if (bList->getSize()>1){
//
//} else {
//	index = 0;
//}
//char str[10]; ->>> diagnostics
//bubble(itoa(index, str, 10));
//	b = bList->getBroker(index);
	msg->setRequestedNode(b);
	b = dynamic_cast<Broker*>(msg->getRequester());
	sendDirect(msg, b->gate("updIn"));
	//bList->addBroker(b);
	brokersVector.push_back(b);
}

void NameServer::handleClientRequest(NSMessage* msg) {
	Broker* b;
	if (brokersVector.size()==0){
		b = NULL;
	} else {
		if (brokersVector.size() > 1) {
			b = brokersVector[rand() % brokersVector.size()];
		} else {
			b = brokersVector[0];
		}
	}

	//Broker* b = bList->getBroker(index);
	msg->setRequestedNode(b);
	Client* c = dynamic_cast<Client*>(msg->getRequester());
	sendDirect(msg, c->gate("updIn"));
}

cGate* NameServer::getFreeInputGate(){
	//should't be called by anybody
	return NULL;
}
