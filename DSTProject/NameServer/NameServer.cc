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
#include "BrokersList.h"
#include "Client.h"

Define_Module(NameServer);

//____________Construction
NameServer::NameServer() {}
NameServer::~NameServer() {}

void NameServer::initialize(){
	setNSGate(gate("updIn"));
	bList = new BrokersList();
	srand(time(0));
}
void NameServer::handleMessage(cMessage* msg){
	NSMessage* ns = dynamic_cast<NSMessage*>(msg);
	if (ns==NULL){return;}
	if (dynamic_cast<Broker*>(ns->getRequester())!=NULL){
		EV << "Going on broker side";
		handleBrokerRequest(ns);
	} else if (dynamic_cast<Client*>(ns->getRequester())!=NULL){
		EV << "Going on client side";
		handleClientRequest(ns);
	}
}

void NameServer::handleBrokerRequest(NSMessage* msg){
	int index;
	if (bList->getSize()>1){
		index = rand()%bList->getSize();
	} else {
		index = 0;
	}
	//char str[10];
	//bubble(itoa(index, str, 10));
	msg->setRequestedNode(bList->getBroker(index));
	//msg->setRequestedNode(bList->getBroker(0));
	Broker* b = dynamic_cast<Broker*>(msg->getRequester());
	sendDirect(msg,b->gate("updIn"));
	bList->addBroker(dynamic_cast<Broker*>(msg->getRequester()));
}

void NameServer::handleClientRequest(NSMessage* msg){
	int index;
	if (bList->getSize()>1){
			index = rand()%bList->getSize();
		} else {
			index = 0;
		}
	msg->setRequestedNode(bList->getBroker(index));
	Client* c = dynamic_cast<Client*>(msg->getRequester());
	sendDirect(msg,c->gate("updIn"));
}
