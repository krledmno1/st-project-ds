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

#ifndef LISTNODE_H_
#define LISTNODE_H_

#include "Broker.h"

class ListNode {
public:
	ListNode(Broker* b);
	virtual ~ListNode();

	Broker* getBroker();

	void setNextNode(ListNode* ln);
	ListNode* getNextNode();
private:
	Broker* broker;
	ListNode* node;
};

#endif /* LISTNODE_H_ */
