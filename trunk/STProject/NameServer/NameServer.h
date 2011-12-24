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

class NameServer: public STNode {
public:
	NameServer();
	virtual ~NameServer();
protected:
	virtual void handleMessage(cMessage* msg);
	virtual void initialize();
private:
	//BrokersList* bList;
	void handleBrokerRequest(NSMessage* msg);
	void handleClientRequest(NSMessage* msg);

	std::vector<Broker*> brokersVector;
};

#endif /* NAMESERVER_H_ */
