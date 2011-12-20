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

#include "BrokersList.h"
#include "ListNode.h"

	int BrokersList::getSize(){return size;}
//__________Construct
BrokersList::BrokersList() {
	size = 0;
}
BrokersList::~BrokersList() {}
//__________Front Panel
void BrokersList::addBroker(Broker* b){
	if (rootNode==NULL){
		rootNode = new ListNode(b);
		size = 1;
	} else { //we already have a root node. Anyway, we add this new Broker to the head of the list
		ListNode* newRoot = new ListNode(b);
		newRoot->setNextNode(rootNode);
		rootNode = newRoot;
		size++;
	}
}
void BrokersList::removeBroker(Broker* b){
	ListNode* prevNode = 0;
	ListNode* i = rootNode;
	while(i!=0){
		if (i->getBroker()==b) { //we found it
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
Broker* BrokersList::getBroker(int index){
	if (size==0) return NULL;
	ListNode* in = rootNode;
	int i = 0;
	while (i<index && in->getNextNode()!=NULL){
		in = in->getNextNode();
		i++;
	}
	return in->getBroker();
}
