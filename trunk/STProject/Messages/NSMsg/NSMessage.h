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

#include "STMessage.h"
#include "STNode.h"
#include "LinkedList.h"

class NSMessage: public STMessage {
public:
	NSMessage();
	NSMessage(STNode* requester);
	virtual ~NSMessage();

	void setRequesterNode(STNode* r);
	STNode* getRequester();

	void addRequestedNode(STNode* r);

	LinkedList<STNode>* getRequestedNodes();
private:
	STNode* requester;
	LinkedList<STNode> requested;
};

#endif /* NSMESSAGE_H_ */
