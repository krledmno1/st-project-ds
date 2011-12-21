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

#include "CCNode.h"

CCNode::CCNode(cGate* bog, cGate* cig) {
	brokerOutputGate = bog;
	clientInputGate = cig;
}
CCNode::~CCNode() {}

cGate* CCNode::getBrokerOutputGate(){return brokerOutputGate;}
cGate* CCNode::getClientInputGate(){return clientInputGate;}

CCNode* CCNode::getNextNode(){return nextNode;}
void CCNode::setNextNode(CCNode* ccn){nextNode = ccn;}
