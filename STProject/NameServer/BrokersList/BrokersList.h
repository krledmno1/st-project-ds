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

#ifndef BROKERSLIST_H_
#define BROKERSLIST_H_

#include"Broker.h"
#include "ListNode.h"

class BrokersList {
public:
	BrokersList();
	virtual ~BrokersList();

	void addBroker(Broker* b);
	void removeBroker(Broker* b);
	int getSize();
	Broker* getBroker(int index); //this method facilitates "random picking"
private:
	int size;
	ListNode* rootNode; //the one with index 0 He will get successors recursively
};

#endif /* BROKERSLIST_H_ */
