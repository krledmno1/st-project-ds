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

PublishMessage::PublishMessage(STNode* stn, int t, VectorClock *ts) {
	messageType = PUBLISH_MESSAGE;
	topic = t;
	sender = stn;
	timeStamp = new VectorClock();
	timeStamp->setTimeStamp(ts->getTimeStamp());
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

VectorClock* PublishMessage::getTimeStamp()
{
    return timeStamp;
}

void PublishMessage::setTimeStamp(VectorClock *timeStamp)
{
    this->timeStamp = timeStamp;
}


