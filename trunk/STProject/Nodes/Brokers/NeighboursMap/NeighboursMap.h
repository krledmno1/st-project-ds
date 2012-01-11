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

#ifndef NEIGHBOURSMAP_H_
#define NEIGHBOURSMAP_H_

#include "STNode.h"
#include "NeighbourEntry.h"
#include <vector>

/*
 * Obs: The mapping being a vector[], i cannot remove entries
 */
class NeighboursMap {
public:
	NeighboursMap();
	virtual ~NeighboursMap();

	void addMapping(STNode* stn, cGate* outGate); //if a mapping exists, gets overwritten, otherwise a new one
	void removeMapping(STNode* stn);

	cGate* getOutputGate(STNode* stn);

	bool hasBrokers(); //it means, it has a broker, regardless of clients

	std::vector<NeighbourEntry*> getNeighboursVector();

	std::vector<NeighbourEntry*> getBrokersVector();

private:
	std::vector<NeighbourEntry*> neighboursVector; /*Note, the vector MAY HAVE NULL ENTRIES */
};

#endif /* NEIGHBOURSMAP_H_ */
