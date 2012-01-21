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

NetworkConditionTable* STNode::conditionTable;

//____________Construction
NameServer::NameServer() {
	srand(time(0));



}
NameServer::~NameServer() {}

void NameServer::initialize() {
	setNSGate(gate("updIn"));

	int bNum = simulation.getSystemModule()->par("nrBrokers");
		int cNum = simulation.getSystemModule()->par("nrClients");
		double min = par("minDelay");
		double max = par("maxDelay");

		STNode::conditionTable = new NetworkConditionTable();

		for(int i = 0;i<cNum+bNum;i++)
		{
			STNode* n1 = NULL;
			if(i<cNum)
			{
				n1 = dynamic_cast<STNode*>(simulation.getSystemModule()->getSubmodule("clients",i));
			}
			else
			{
				n1 = dynamic_cast<STNode*>(simulation.getSystemModule()->getSubmodule("borkers",i-cNum));
			}
			Map<STNode,double>* temp = new Map<STNode,double>();
			for(int j = 0;j<bNum;j++)
			{
				STNode* n2;
				n2 = dynamic_cast<STNode*>(simulation.getSystemModule()->getSubmodule("borkers",j));
				if(n1!=n2)
				{
					double delay = min+(((double)rand()/(double)RAND_MAX)*(max-min));

					temp->setMapping(n2,delay);
				}
				else
				{
					temp->setMapping(n2,0.0);
				}
			}
			STNode::conditionTable->getTable()->setMapping(n1,temp);
		}

}
void NameServer::handleMessage(cMessage* msg) { //NameServer only uses NSMessage*. Using type constants would only clutter the code
	STMessage* stm = dynamic_cast<STMessage*>(msg);
	if (stm == NULL) {
		EV << "NameServer: unrecognized message";
		return;
	}

	if (stm->getType() == stm->NAME_SERVER_MSG) {
		NSMessage* ns = dynamic_cast<NSMessage*>(msg);
		if (dynamic_cast<Broker*>(ns->getRequester()) != NULL) {
			handleBrokerRequest(ns);
		} else if (dynamic_cast<Client*>(ns->getRequester()) != NULL) {
			handleClientRequest(ns);
		}
	} else if (stm->getType() == stm->DISCONNECTION_REQUEST_MSG) {
		handleUnregisterRequest(dynamic_cast<DisconnectionRequestMessage*>(msg));
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
			while (b == NULL) {
				b = brokersVector[rand() % brokersVector.size()];
			}
		}
	}
	msg->setRequestedNode(b);
	b = dynamic_cast<Broker*>(msg->getRequester());
	sendDirect(msg, b->gate("updIn"));
	registerBroker(b);
}




void NameServer::handleClientRequest(NSMessage* msg) {
	Broker* b = NULL;
	if (brokersVector.size() == 0) {
		b = NULL;
	} else {
		if (brokersVector.size() > 1) { //we pick a random one
			while (b == NULL) { //because Brokers unregistered just became NULL, the vector did not shrink, (a LinkedList is needed)
				b = brokersVector[rand() % brokersVector.size()];	//possible infinite loop if all brokers unregister and client tries to register
			}
		} else {
			b = brokersVector[0];
		}
	}
	msg->setRequestedNode(b);
	Client* c = dynamic_cast<Client*>(msg->getRequester());
	sendDirect(msg, c->gate("updIn"));
}

void NameServer::handleUnregisterRequest(DisconnectionRequestMessage* msg) {
	Broker* b = dynamic_cast<Broker*>(msg->getRequesterNode());
	if (b == NULL) {
		return;
	}
	//we must remove him from the vector
	for (unsigned int i = 0; i < brokersVector.size(); i++) {
		if (brokersVector[i] == b) {
			brokersVector[i] = NULL;
			cancelAndDelete(msg);
			return;
		}
	}
	EV << "NameServer: I didnt find the broker that I had to unregister!!!";
}

cGate* NameServer::getFreeInputGate() {
	//should't be called by anybody
	return NULL;
}

void NameServer::registerBroker(Broker* b) { //its more then just appending the broker to the vector. In case vector has null entries corrisponding to removed Brokers, it shouldn't extend the vector but simply insert it in a null position (clearly we should use LinkedLists, not vectors)
	for (unsigned int i = 0; i < brokersVector.size(); i++) {
		if (brokersVector[i] == NULL) {
			brokersVector[i] = b;
			return;
		}
	} //here means we have no NULL entry, no unsubscribed broker...
	brokersVector.push_back(b);
}
