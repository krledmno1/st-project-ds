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

#ifndef NSMESSAGE_H_
#define NSMESSAGE_H_

#include <cmessage.h>
#include "STNode.h"

class NSMessage: public cMessage {
public:
	NSMessage();
	NSMessage(STNode* requester);
	virtual ~NSMessage();

	void setRequesterNode(STNode* r);
	STNode* getRequester();

	void setRequestedNode(STNode* r);
	STNode* getRequestedNode();
private:
	STNode* requester;
	STNode* requested;
};

#endif /* NSMESSAGE_H_ */
