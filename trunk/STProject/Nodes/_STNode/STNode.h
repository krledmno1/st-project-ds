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

#ifndef STNODE_H_
#define STNODE_H_

#include <csimplemodule.h>

/*
 * This is the superclass of all the nodes of our project.
 * Mainly it has been created such that it hardcodes the address of the Naming Service (through a static pointer).
 * It also provides the facility of freeInputGates, accessible to any other node that wishes to communicate.
 */

class STNode: public cSimpleModule {
public:
	STNode();
	virtual ~STNode();

	virtual cGate* getFreeInputGate();
	void setNSGate(cGate *nsGate);

protected:
	cMessage *wakeUpDelayMsg;
	cMessage *sleepDelayMsg;

	cGate* getNSGate();

	static const int NR_TOPICS = 4; //€[0;3] and No negative topics!
};

#endif /* STNODE_H_ */
