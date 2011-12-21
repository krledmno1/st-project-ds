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

#include "ClientsMap.h"
#include "omnetpp.h"

ClientsMap::ClientsMap() {
}
ClientsMap::~ClientsMap() {
}

void ClientsMap::addMapping(cGate* brokerOutputGate, cGate* clientInputGate) {
	if (rootNode == NULL) {
		rootNode = new CCNode(brokerOutputGate, clientInputGate);
	} else {
		CCNode* newRoot = new CCNode(brokerOutputGate, clientInputGate);
		newRoot->setNextNode(rootNode);
		rootNode = newRoot;
	}
}
void ClientsMap::removeMapping(cGate* brokerOutputGate) { //no need to specify to which it was connected previously
	CCNode* prevNode = NULL;
	CCNode* i = rootNode;
	while (i != NULL) {
		if (i->getBrokerOutputGate() == brokerOutputGate) { //we found it
			if (prevNode == NULL) { //we're dealing with the root
				rootNode = i->getNextNode();
			} else { //we're somewhere in the list
				prevNode->setNextNode(i->getNextNode());
			}
			return;
		}
		prevNode = i;
		i = i->getNextNode();
	}
	//If I ever get here, I'm screwed. It means I didnt find the broker I was looking to remove (throw Exception? :D)
	EV << "ERROR: ClientsMap did not find the gate that had to be removed!";
}
cGate* ClientsMap::getClientInputGate(cGate* brokerOutputGate) {
	CCNode* i = rootNode;
	while (i != NULL) {
		if (i->getBrokerOutputGate() == brokerOutputGate) { //we found it
			return i->getClientInputGate();
		}
		i = i->getNextNode();
	}
	//If I ever get here, I'm screwed. It means I didnt find the broker I was looking to remove (throw Exception? :D)
	EV << "ERROR: ClientsMap did not find the gate that was searched for!";
	return NULL;
}
cGate* ClientsMap::getBrokerOutPutGate(cGate* clientInputGate){
	CCNode* i = rootNode;
		while (i != NULL) {
			if (i->getClientInputGate() == clientInputGate) { //we found it
				return i->getBrokerOutputGate();
			}
			i = i->getNextNode();
		}
		//If I ever get here, I'm screwed. It means I didnt find the broker I was looking to remove (throw Exception? :D)
		EV << "ERROR: ClientsMap did not find the gate that was searched for!";
		return NULL;
}
