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

PublishMessage::PublishMessage(STNode* stn, int t) {
	messageType = PUBLISH_MESSAGE;
	topic = t;
	sender = stn;
	id = nextID;
	nextID++;
	creationTime = simTime();
}
simtime_t PublishMessage::getCreationTime() {
	return creationTime;
}

PublishMessage::PublishMessage(){
	messageType = PUBLISH_MESSAGE; //and nothing else
}

PublishMessage::~PublishMessage() {}

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
	return clone;
}
