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

#ifndef NAMESERVER_H_
#define NAMESERVER_H_

#include "STNode.h"
#include "NSMessage.h"
#include <vector>
#include "Broker.h"
#include "DisconnectionRequestMessage.h"

/*
 * Looking at the implementation, you'll notice that the NameServer depends upon the fact that once more than 1 broker
 * has been registered, it can never drop bellow 1 (otherwise it would lock in cycles) TODO prevent this happening in case
 * we cannot rely on broker presence
 */
class NameServer: public STNode {
public:
	NameServer();
	virtual ~NameServer();

	virtual cGate* getFreeInputGate();

protected:
	virtual void handleMessage(cMessage* msg);
	virtual void initialize();

private:
	void handleBrokerRequest(NSMessage* msg);
	void handleClientRequest(NSMessage* msg);
	void handleUnregisterRequest(DisconnectionRequestMessage* msg);
	std::vector<Broker*> brokersVector;

	//try to refactor with linked list
	//LinkedList<Broker> brokerList;

	void registerBroker(Broker* b); //its more then just appending the broker to the vector. In case vector has null entries corrisponding to removed Brokers, it shouldn't extend the vector but simply insert it in a null position (clearly we should use LinkedLists, not vectors)
};

#endif /* NAMESERVER_H_ */
