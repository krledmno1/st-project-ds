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

#ifndef CLIENTSMAP_H_
#define CLIENTSMAP_H_

#include "CCNode.h"

/* ___Linked List Implementation
 * The original intention was to have a mapping between the output gates of the Broker, and the clients' input gates to which they are connected
 *	This sort of mapping suits well also the need of a mapping between the clients and their subscriptions to various topics (TODO)
 */
class ClientsMap {
public:
	ClientsMap();
	virtual ~ClientsMap();

	void addMapping(cGate* brokerOutputGate, cGate* clientInputGate);
	void removeMapping(cGate* brokerOutputGate); //no need to specify to which it was connected previously
	cGate* getClientInputGate(cGate* brokerOutputGate); //actually this isn't used, I just didn't think it through :)
	cGate* getBrokerOutPutGate(cGate* clientInputGate);

private:
	CCNode* rootNode;
};

#endif /* CLIENTSMAP_H_ */
