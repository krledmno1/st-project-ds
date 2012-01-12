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
 * Obs: The mapping being a vector[], i cannot remove entries. I instead nullify the entries that have to be removed. Thus watchout for NULL bombs...
 *Note: the map will also keep track of all the client's topic subscriptions. I wonder if I can unify the subscription mapping between clients and brokers
 */
class NeighboursMap {
public:
	NeighboursMap();
	virtual ~NeighboursMap();
//__Connections Management
	void addMapping(STNode* stn, cGate* outGate); //if a mapping exists, gets overwritten, otherwise a new one
	void removeMapping(STNode* stn);
	cGate* getOutputGate(STNode* stn);

//__Subscriptions
	void addSubscription(STNode* stn, int topic);
	void removeSubscription(STNode* stn, int topic);
	std::vector<NeighbourEntry*> getSubscribers(int topic);
	bool hasClientSubscribers(int topic);
	bool isSubscribed(STNode* stn, int topic);
	std::vector<int> getSubscriptions(); //returns all the topics to which this Broker is subscribed (by his clients, or neighbouring brokers)

//__General purpose tasks
	bool hasBrokers(); //it means, it has a broker, regardless of clients
	std::vector<NeighbourEntry*> getNeighboursVector();
	std::vector<NeighbourEntry*> getBrokersVector();
	std::vector<int> getSubscriptions(STNode* stnode);
private:
	std::vector<NeighbourEntry*> neighboursVector; /*Note, the vector MAY HAVE NULL ENTRIES */
	NeighbourEntry* getEntry(STNode* stn);
};

#endif /* NEIGHBOURSMAP_H_ */
