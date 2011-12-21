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

#ifndef BROKERSCONTAINER_H_
#define BROKERSCONTAINER_H_
#include "cgate.h"
#include "BCNode.h"
#include "omnetpp.h"


/*
 * ______Linked List implementation
 * This is a mapping between the owner brokers' output gates to the connected brokers' input gates
 * This mapping will also manage the subscriptions incoming from other brokers.
 * TODO UNFINISHED, and UNITILIZED (actually I dont think its even compiled...)
 */

class BrokersContainer {
public:
	BrokersContainer();
	virtual ~BrokersContainer();

	void addBrokerGate(cGate* b);
	void removeBrokerGate(cGate* b);
	int getSize();
	cGate* getBrokerGate(int index); //this method facilitates "random picking"
private:
	int size;
	BCNode* rootNode; //the one with index 0 He will get successors recursively
};

#endif /* BROKERSCONTAINER_H_ */