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

#include "PublishMessage.h"

int PublishMessage::nextID = 0;
PublishMessage::PublishMessage(STNode* stn, int t, VectorClock *ts) {
	messageType = PUBLISH_MESSAGE;
	topic = t;
	sender = stn;
	originalSender = stn;
	timeStamp = new VectorClock();
	//we import the timestamp creating pair by pair
	Node<Pair>* pairIterator = ts->getTimeStamp()->start;
	while (pairIterator!=NULL){
		if (pairIterator->getContent()!=NULL){
			timeStamp->timeStamp->addToBack(new Pair(pairIterator->getContent()->getNode(),pairIterator->getContent()->getValue()));
		} else {
			EV << "PublishMessage NULL";
		}
		pairIterator = pairIterator->getNext();
	}
	//timeStamp->setTimeStamp(ts->getTimeStamp());
	creationTime = simTime();
}
simtime_t PublishMessage::getCreationTime() {
	return creationTime;
}

STNode *PublishMessage::getOriginalSender() {
	return originalSender;
}

PublishMessage::PublishMessage(){
	messageType = PUBLISH_MESSAGE; //and nothing else
	timeStamp = new VectorClock();
}
PublishMessage::~PublishMessage() {
	delete timeStamp;
}

int PublishMessage::getTopic(){
	return topic;
}

STNode* PublishMessage::getSender(){
	return sender;
}

PublishMessage* PublishMessage::clone(STNode* newSender){
	PublishMessage* clone = new PublishMessage();
	clone->sender = newSender;
	clone->topic = topic;
	clone->id = id;
	clone->creationTime = creationTime;
	clone->originalSender = originalSender;
	Node<Pair>* pairIterator = timeStamp->getTimeStamp()->start;
	while (pairIterator!=NULL){
		if (pairIterator->getContent()!=NULL){
			clone->timeStamp->timeStamp->addToBack(pairIterator->getContent());
		} else {
			EV << "PublishMessage NULL";
		}
		pairIterator = pairIterator->getNext();
	}
	return clone;
}
VectorClock* PublishMessage::getTimeStamp() {
    return timeStamp;
}

void PublishMessage::setTimeStamp(VectorClock *timeStamp) {
    this->timeStamp = timeStamp;
}


