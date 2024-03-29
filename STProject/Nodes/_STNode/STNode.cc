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

#include "STNode.h"
#include "cmessage.h"

static cGate* nsGate;
void STNode::setNSGate(cGate *nsg){
		nsGate = nsg;
}
cGate* STNode::getNSGate(){
	return nsGate;
}

//_____________Constructing
STNode::STNode() {
	wakeUpDelayMsg = new cMessage("wake up");
	sleepDelayMsg = new cMessage("go to sleep");
}
STNode::~STNode() {
	cancelAndDelete(wakeUpDelayMsg);
	cancelAndDelete(sleepDelayMsg);
}

//_____________Front Panel
cGate* STNode::getFreeInputGate(){
	EV << "The abstract function got called. This C++ thing....";
	return NULL;
}

double STNode::ping(STNode* target)
{
	return conditionTable->getDelay(this,target);
}



