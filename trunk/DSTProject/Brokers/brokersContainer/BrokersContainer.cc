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

#include "BrokersContainer.h"
#include "BCNode.h"

int BrokersList::getSize(){return size;}

BrokersContainer::BrokersContainer() {
	size = 0;
}

BrokersContainer::~BrokersContainer() {}

void BrokersContainer::addBrokerGate(cGate* b){
	if (rootNode==NULL){
		rootNode = new BCNode(b);
		size = 1;
	} else { //we already have a root node. Anyway, we add this new Broker to the head of the list
		//if (rootNode->getBrokerGate()==b) {
		//	EV << "I already have this broker";
		//}
		BCNode* newRoot = new BCNode(b);
		//if (newRoot->getBrokerGate()==rootNode->getBrokerGate()){
		//	EV << "Si aici avem acelasi broker";
		//}
		newRoot->setNextNode(rootNode);
		rootNode = newRoot;
		size++;
	}
}

void BrokersContainer::removeBrokerGate(cGate* b){
	BCNode* prevNode = 0;
	BCNode* i = rootNode;
	while(i!=0){
		if (i->getBrokerGate()==b) { //we found it
			if (prevNode==0){//we're dealing with the root
				rootNode = i->getNextNode();
				size--;
			} else { //we're somewhere in the list
				prevNode->setNextNode(i->getNextNode());
				size--;
			}
			return;
		}
		prevNode = i;
		i = i->getNextNode();
	}
	//If I ever get here, I'm screwed. It means I didnt find the broker I was looking to remove (throw Exception? :D)
}
cGate* BrokersContainer::getBrokerGate(int index){
	if (size==0) return NULL;
	BCNode* in = rootNode;
	int i = 0;
	while (i<index && in->getNextNode()!=NULL){
		EV << "Going inner\n";
		in = in->getNextNode();
		i++;
	}
	if (in==rootNode){
		EV << "I'm returning the root \n";
	}
	if (in->getBrokerGate()==rootNode->getBrokerGate()){
		EV << "Target got the same broker as the root \n";
	}
	return in->getBrokerGate();
}
