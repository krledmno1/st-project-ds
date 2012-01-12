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

class STNode: public cSimpleModule {
public:
	STNode();
	virtual ~STNode();

	virtual cGate* getFreeInputGate();
	void setNSGate(cGate *nsGate);

	bool isAlive();
protected:
	bool alive;
	cMessage *wakeUpDelayMsg;
	cMessage *sleepDelayMsg;

	cGate* getNSGate();

	static const int NR_TOPICS = 4; //�[0;3] and No negative topics!
};

#endif /* STNODE_H_ */
