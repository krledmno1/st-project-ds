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

#ifndef PUBLISHMESSAGE_H_
#define PUBLISHMESSAGE_H_

#include "VectorClock.h"

#include "STMessage.h"
#include "STNode.h"

class PublishMessage: public STMessage {
public:
	PublishMessage(STNode* sender, int topic, VectorClock *ts);
	virtual ~PublishMessage();

	int getTopic();
	STNode* getSender();
	STNode* getOriginalSender();
    VectorClock* getTimeStamp();
    void setTimeStamp(VectorClock *timeStamp);
	PublishMessage* clone(STNode* newSender);
	simtime_t getCreationTime();
private:
	PublishMessage();
	int topic;
	STNode* sender;
	STNode* originalSender;
	VectorClock *timeStamp;
	static int nextID;
	int id;
	simtime_t creationTime;
};

#endif /* PUBLISHMESSAGE_H_ */
