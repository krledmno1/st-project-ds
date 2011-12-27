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

#include "NeighboursMap.h"

NeighboursMap::NeighboursMap() {}
NeighboursMap::~NeighboursMap() {}

void NeighboursMap::addMapping(STNode* stn, cGate* outGate){
	//firstly I check if there isn't already this node registered
	for (unsigned int i=0;i<neighboursVector.size();i++){
		if (neighboursVector[i]->getNeighbour()==stn){
			neighboursVector[i]->setOutGate(outGate);
			return;
		}
	}
	//here it means we scanned the entire list
	NeighbourEntry* ne = new NeighbourEntry(stn,outGate);
	neighboursVector.push_back(ne);
}

cGate* NeighboursMap::getOutputGate(STNode* stn){
	for (unsigned int i=0; i<neighboursVector.size();i++){
		if (neighboursVector[i]->getNeighbour()==stn){
			return neighboursVector[i]->getOutGate();
		}
	}
	return NULL;
}
