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

#ifndef CCNODE_H_
#define CCNODE_H_
#include "cgate.h"

class CCNode {
public:
	CCNode(cGate* brokerOutputGate, cGate* clientInputGate);
	virtual ~CCNode();

	cGate* getBrokerOutputGate();
	cGate* getClientInputGate();

	CCNode* getNextNode();
	void setNextNode(CCNode* ccn);

private:
	cGate* brokerOutputGate;
	cGate* clientInputGate;
	CCNode* nextNode;
};

#endif /* CCNODE_H_ */
