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

#ifndef NEWBROKERNOTIFICATIONMESSAGE_H_
#define NEWBROKERNOTIFICATIONMESSAGE_H_

#include "STMessage.h"
#include "Broker.h"

/*
 * This message is being pushed by a new broker when it joins the network, and it will be broadcasted to every client of the network, to test its ping to the new server and join him in case it has a good connection
 */
class NewBrokerNotificationMessage: public STMessage {
public:
	NewBrokerNotificationMessage(Broker* newBroker, Broker* previousHop);
	virtual ~NewBrokerNotificationMessage();

	Broker* getJoiningBroker();
	Broker* getPreviousHop();
private:
	Broker* newBroker;
	Broker* previousHop;
};

#endif /* NEWBROKERNOTIFICATIONMESSAGE_H_ */
