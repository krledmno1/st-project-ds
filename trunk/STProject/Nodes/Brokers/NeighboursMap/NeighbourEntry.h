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

#ifndef NEIGHBOURENTRY_H_
#define NEIGHBOURENTRY_H_

#include "STNode.h"
#include "cgate.h"

class NeighbourEntry {
public:
	NeighbourEntry(STNode* neighbour, cGate* myOutGate);
	virtual ~NeighbourEntry();

	STNode* getNeighbour();
	cGate* getOutGate();
	void setOutGate(cGate* og);

private:
	STNode* neighbourNode;
	cGate* myOutGate;
};

#endif /* NEIGHBOURENTRY_H_ */
