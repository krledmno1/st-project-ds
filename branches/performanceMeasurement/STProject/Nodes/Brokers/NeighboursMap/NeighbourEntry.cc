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

#include "NeighbourEntry.h"

NeighbourEntry::NeighbourEntry(STNode* n, cGate* mog) {
	neighbourNode = n;
	myOutGate = mog;
}

NeighbourEntry::~NeighbourEntry() {}

STNode* NeighbourEntry::getNeighbour(){
	return neighbourNode;
}
cGate* NeighbourEntry::getOutGate(){
	return myOutGate;
}
void NeighbourEntry::setOutGate(cGate* og){
	myOutGate = og;
}

//_________Subscription Management
void NeighbourEntry::addSubscription(int topic){
	//first scan and check if we already have it
	int freeSpace = -1;
	for (unsigned int i=0;i<subscriptions.size();i++){
		if (subscriptions[i]==topic) return;
		if (subscriptions[i]<0) freeSpace = i;
	}
	//at this point we're sure we don't have that topic already
	if (freeSpace>=0){ //we have a free slot
		subscriptions[freeSpace] = topic;
	} else { //we don't have space, we enlarge the vector
		subscriptions.push_back(topic);
	}
}
void NeighbourEntry::removeSubscription(int topic){
	for (unsigned int i=0;i<subscriptions.size();i++){
		if (subscriptions[i]==topic){
			subscriptions[i] = -1; //free space
			return;
		}
	}
}
std::vector<int> NeighbourEntry::getSubscriptions(){
	std::vector<int> cleanSubscriptions = std::vector<int>(subscriptions.size());
	for (unsigned int i=0;i<subscriptions.size();i++){
		if (subscriptions[i]>=0){
			cleanSubscriptions.push_back(subscriptions[i]);
		}
	}
	return cleanSubscriptions;
}

bool NeighbourEntry::isSubscribed(int topic){//true IFF is registered to topic
	for (unsigned int i=0;i<subscriptions.size();i++){
		if (subscriptions[i]==topic) return true;
	}
	return false;
}
