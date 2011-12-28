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

#ifndef CLIENT_H_
#define CLIENT_H_

#include "STNode.h"
#include "NSMessage.h"
#include <omnetpp.h>
#include "Broker.h"
#include "cgate.h"

class Client: public STNode {
public:
	Client();
	virtual ~Client();

	virtual cGate* getFreeInputGate();

protected:
	virtual void handleMessage(cMessage *msg);
	virtual void initialize();
private:
	void wakeUp();
	void goSleep();
	void handleNameServerMessage(NSMessage* nsm); //this is the reply we get from NS when we ask for a broker
	void handleBrokerDisconnectionRequest(); //if a broker wishes to disconnect, it is client's task to find another broker
};

#endif /* CLIENT_H_ */
