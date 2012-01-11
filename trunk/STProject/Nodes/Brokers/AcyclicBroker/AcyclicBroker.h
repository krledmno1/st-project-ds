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

#ifndef ACYCLICBROKER_H_
#define ACYCLICBROKER_H_

#include "Broker.h"

/**
 * This Broker will implement all the behavior needed for an Acyclic Broker. That means, that every single broker will try do its best in order to maintain an acyclic topology (particularly this issue
 * arises when a Broker decides to disconnect and leaves various clusters of Brokers unconnected unless he does something about it)
 */

class AcyclicBroker: public Broker {
public:
	AcyclicBroker();
	virtual ~AcyclicBroker();

protected:
	virtual void sleep();
	virtual void handleSubscription(SubscriptionMessage* sm);
	virtual void handleConnectionRequest(ConnectionRequestMessage* crm);
	virtual void handlePublish(PublishMessage* pm);
	virtual void handleUnsubscription(UnsubscriptionMessage* um);
};

#endif /* ACYCLICBROKER_H_ */
