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

#ifndef BROKER_H_
#define BROKER_H_

#include "STNode.h"
#include "NSMessage.h"
#include "ConnectionRequestMessage.h"
#include "DisconnectionRequestMessage.h"
#include "NeighboursMap.h"
#include "SubscriptionMessage.h"
#include "UnsubscriptionMessage.h"
#include "PublishMessage.h"


class NewBrokerNotificationMessage;

/*
 * The Broker, in this form, provides the basic facility of handling connections. It has nothing to do with passing messages around (Publish/Subcribe), or maintaining any topology of the network,
 * in fact, once the broker wishes to disconnect, it will break/fragment the entire network. This will be taken care of by further extensions (Acyclic/Cyclic Brokers)
 */

class Broker: public STNode {
public:
	Broker();
	virtual ~Broker();

	virtual cGate* getFreeInputGate();
	cGate* getFreeOutputGate();
protected:
	virtual void handleMessage(cMessage *msg);
	virtual void initialize();

	virtual void handleConnectionRequest(ConnectionRequestMessage* crm);
	virtual void sleep();
	virtual void handleSubscription(SubscriptionMessage* sm);
	virtual void handleUnsubscription(UnsubscriptionMessage* um);
	virtual void handlePublish(PublishMessage* pm);
	virtual void handleDisconnectionRequest(DisconnectionRequestMessage* drm);
	NeighboursMap neighboursMap;
private:
	void wakeUp();
	void handleNameServerMessage(NSMessage* nsm);
	void handleNewBrokerNotification(NewBrokerNotificationMessage* nbnm);
};

#endif /* BROKER_H_ */
