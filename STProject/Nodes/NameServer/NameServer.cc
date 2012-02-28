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

		//create condition table
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

//NameServer only uses NSMessage*. Using type constants would only clutter the code
void NameServer::handleMessage(cMessage* msg)
{
	STMessage* stm = dynamic_cast<STMessage*>(msg);
	if (stm == NULL)
	{
		EV << "NameServer: unrecognized message";
		return;
	}

	if (stm->getType() == stm->NAME_SERVER_MSG)
	{
		NSMessage* ns = dynamic_cast<NSMessage*>(msg);
		if (dynamic_cast<Broker*>(ns->getRequester()) != NULL)
		{
			handleBrokerRequest(ns);
		}
		else
		{
			if (dynamic_cast<Client*>(ns->getRequester()) != NULL)
			{
				handleClientRequest(ns);
			}
		}
	}
	else
	{
		if (stm->getType() == stm->DISCONNECTION_REQUEST_MSG)
		{
			handleUnregisterRequest(dynamic_cast<DisconnectionRequestMessage*>(msg));
		}
	}
}


//now there is also a linked list of brokers in the message there are no active brokers
//this will be false and message will contain no brokers; recipient has to check for that
void NameServer::handleBrokerRequest(NSMessage* msg) {

	if (!brokerList.isEmpty())
	{
		Node<Broker>* traverse = brokerList.start;
		while(traverse!=brokerList.end)
		{
			msg->addRequestedNode(traverse->getContent());
			traverse= traverse->getNext();
		}
		msg->addRequestedNode(traverse->getContent());
	}
	Broker* b = dynamic_cast<Broker*>(msg->getRequester());
	sendDirect(msg, b->gate("updIn"));
	registerBroker(b);

	////////////////////////////////////////////////////
	//OLD CODE with vector
	/*

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

	 */
	///////////////////////////////////////////////////////////

}



//same goes with the client...
void NameServer::handleClientRequest(NSMessage* msg) {

	if (!brokerList.isEmpty())
	{
		Node<Broker>* traverse = brokerList.start;
		while(traverse!=brokerList.end)
		{
			msg->addRequestedNode(traverse->getContent());
			traverse= traverse->getNext();
		}
		msg->addRequestedNode(traverse->getContent());
	}

	Client* c = dynamic_cast<Client*>(msg->getRequester());
	sendDirect(msg, c->gate("updIn"));

	////////////////////////////////////////////////////////////
	//OLD CODE with vector
	/*

	 Broker* b = NULL;
        if (brokersVector.size() == 0) {
                b = NULL;
        } else {
                if (brokersVector.size() > 1) { //we pick a random one
                        while (b == NULL) { //because Brokers unregistered just became NULL, the vector did not shrink, (a LinkedList is needed)
                                b = brokersVector[rand() % brokersVector.size()];       //possible infinite loop if all brokers unregister and client tries to register
                        }
                } else {
                        b = brokersVector[0];
                }
        }
        msg->setRequestedNode(b);
        Client* c = dynamic_cast<Client*>(msg->getRequester());
        sendDirect(msg, c->gate("updIn"));

	 */
	///////////////////////////////////////////////////////////////
}


//Does nothing if the broker is not in the list or the request is not form a broker
//otherwise, it removes the broker from the list
void NameServer::handleUnregisterRequest(DisconnectionRequestMessage* msg) {
	Broker* b = dynamic_cast<Broker*>(msg->getRequesterNode());
	if (b != NULL) {

		//we must remove him from the vector
		if(brokerList.removeNode(b)==NULL)
		{
			EV << "NameServer: I didn't find the broker that I had to unregister!!!";
		}
		else
		{
			cancelAndDelete(msg);
		}
	}


	////////////////////////////////////////////////////////////
	//OLD CODE with vector
	/*

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

	 */
	////////////////////////////////////////////////////////////////
}

cGate* NameServer::getFreeInputGate() {
	//should't be called by anybody
	return NULL;
}


//its more then just appending the broker to the vector. In case vector has null entries corrisponding to removed Brokers,
//it shouldn't extend the vector but simply insert it in a null position (clearly we should use LinkedLists, not vectors)
//EDIT: brokerList is Linked List now
void NameServer::registerBroker(Broker* b) {
	if(brokerList.find(b)==NULL)
	{
		brokerList.addToBack(b);
	}
	else
	{
		EV << "NameServer: Broker subscribed under my radar somehow!";
	}


	////////////////////////////////////////////////////////////
	//OLD CODE with vector
	/*

	   for (unsigned int i = 0; i < brokersVector.size(); i++) {
                if (brokersVector[i] == NULL) {
                        brokersVector[i] = b;
                        return;
                }
        } //here means we have no NULL entry, no unsubscribed broker...
        brokersVector.push_back(b);

	 */
	//////////////////////////////////////////////////////////////
}
