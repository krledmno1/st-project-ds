

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
#include "Broker.h"
#include "Client.h"

NeighboursMap::NeighboursMap() {}
NeighboursMap::~NeighboursMap() {}

void NeighboursMap::addMapping(STNode* stn, cGate* outGate) {
	//firstly I check if there isn't already this node registered
	NeighbourEntry* ne = new NeighbourEntry(stn, outGate);
	for (unsigned int i = 0; i < neighboursVector.size(); i++) {
		if (neighboursVector[i] == NULL) {
			neighboursVector[i] = ne;
			return;
		}
	}
	neighboursVector.push_back(ne);
}

cGate* NeighboursMap::getOutputGate(STNode* stn) {
	for (unsigned int i = 0; i < neighboursVector.size(); i++) {
		if (neighboursVector[i] != NULL){
			if (neighboursVector[i]->getNeighbour() == stn) {
				return neighboursVector[i]->getOutGate();
			}
		}
	}
	return NULL;
}

LinkedList<NeighbourEntry>* NeighboursMap::getNeighboursList(){
	LinkedList<NeighbourEntry>* list = new LinkedList<NeighbourEntry>;
	for (unsigned int i=0;i<neighboursVector.size();i++){
		if (neighboursVector[i]!=NULL){
			list->addToBack(neighboursVector[i]);
		}
	}
	return list;
}

LinkedList<NeighbourEntry>* NeighboursMap::getBrokersList(){
	LinkedList<NeighbourEntry>* list = new LinkedList<NeighbourEntry>;
	for (unsigned int i=0;i<neighboursVector.size();i++){
		if (neighboursVector[i]!=NULL){
			Broker* b = dynamic_cast<Broker*>(neighboursVector[i]->getNeighbour());
			if (b!=NULL){ //we put it
				list->addToBack(neighboursVector[i]);
			}
		}
	}
	return list;
}

bool NeighboursMap::hasBrokers() { //it means, it has a broker, regardless of clients
	for (unsigned int i = 0; i < neighboursVector.size(); i++) {
		if (neighboursVector[i] != NULL) {
			if (dynamic_cast<Broker*>(neighboursVector[i]->getNeighbour()) != NULL) {
				if (neighboursVector[i]->getOutGate()->isConnected()) {
					return true;
				}
			}
		}
	}
	return false;
}

void NeighboursMap::removeMapping(STNode* stn) {
	//firstly I check if there isn't already this node registered
	for (unsigned int i = 0; i < neighboursVector.size(); i++) {
		if (neighboursVector[i] != NULL) {
			if (neighboursVector[i]->getNeighbour() == stn) { //found it
				neighboursVector[i] = NULL;
				return;
			}
		}
	}
	EV	<< "NeighboursMap: ERROR The node to be removed from mappings wasn't found!!! (This shouldn't happen)";
}

int maxTopic = 0; //very hackish, but quick to implement getSubcriptions();
void NeighboursMap::addSubscription(STNode* stn, int topic){
	NeighbourEntry* ne = getEntry(stn);
	if (ne==NULL){
		EV << "The node which subscribed has been removed";
		return;
	}
	ne->addSubscription(topic);
	if (topic>maxTopic) maxTopic = topic;
}
void NeighboursMap::removeSubscription(STNode* stn, int topic){
	NeighbourEntry* ne = getEntry(stn);
		if (ne==NULL){
			EV << "The node which unsubscribed has been removed";
			return;
		}
		ne->removeSubscription(topic);
}

LinkedList<NeighbourEntry>* NeighboursMap::getSubscribers(int topic){
	LinkedList<NeighbourEntry>* list = new LinkedList<NeighbourEntry>;
	for (unsigned int i=0;i<neighboursVector.size();i++){
		if (neighboursVector[i]!=NULL){
			if (neighboursVector[i]->isSubscribed(topic)){
				list->addToBack(neighboursVector[i]);
			}
		}
	}
	return list;
}

bool NeighboursMap::hasClientSubscribers(int topic){
	for (unsigned int i=0;i<neighboursVector.size();i++){
		if (neighboursVector[i]!=NULL){
			if (dynamic_cast<Client*>(neighboursVector[i]->getNeighbour())!=NULL && neighboursVector[i]->isSubscribed(topic)){
				return true;
			}
		}
	}
	return false;
}

bool NeighboursMap::isSubscribed(STNode* stn, int topic){
	NeighbourEntry* ne = getEntry(stn);
	if (ne==NULL) return false;
	return ne->isSubscribed(topic);
}
std::vector<int> NeighboursMap::getSubscriptions(){
	std::vector<int> subscriptions;
	//TODO this is totally inefficient, but quick to implement. To optimize, just like the client we should keep a boolean array with all the subscriptions, and return easily the existing subscriptions. The challenge would be to make efficient the unsubscriptions also (instead of bool, use int, and count them?)
	for (int i=0;i<=maxTopic;i++){
		LinkedList<NeighbourEntry>* list = getSubscribers(i);
		if (list->size>0){
			subscriptions.push_back(i);
		}
		delete(list);
	}
	return subscriptions;
}

std::vector<int> NeighboursMap::getSubscriptions(STNode* stnode){
	std::vector<int> defaultResponse(0); //to avoid returning pointers, damn C++, I will send an empty structure
	NeighbourEntry* ne = getEntry(stnode);
	if (ne==NULL){
		return defaultResponse;
	}
	return ne->getSubscriptions();
}

NeighbourEntry* NeighboursMap::getEntry(STNode* stn){
	for (unsigned int i=0; i<neighboursVector.size();i++){
		if (neighboursVector[i]!=NULL){
			if (neighboursVector[i]->getNeighbour() == stn){ //found it
				return neighboursVector[i];
			}
		}
	}
	return NULL;
}

bool NeighboursMap::hasNode(STNode *n)
{
	if(getEntry(n)!=NULL)
		return true;
	else
		return false;
}
