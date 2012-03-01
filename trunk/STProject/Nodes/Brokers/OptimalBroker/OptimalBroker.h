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

#ifndef OPTIMALBROKER_H_
#define OPTIMALBROKER_H_

#include "AcyclicBroker.h"
#include "ConnectMessage.h"
#include "MWOEMessage.h"
#include "ExpandMessage.h"
#include "UpdateMessage.h"
#include "Map.h"
#include "GenericQueue.h"
#include "JoinMessage.h"
#include "Client.h"

class OptimalBroker: public AcyclicBroker {
public:
	OptimalBroker();
	virtual ~OptimalBroker();

private:

	long id;
	long componentId;
	bool converged;
	long currentConvergence;
	LinkedList<long> members;
	LinkedList<Broker> availableBrokers;
	cGate* leaderHop;
	Map<Broker,ConnectMessage*> sentConnects;
	Map<Broker,ConnectMessage*> receivedConnects;
	LinkedList<MWOEMessage> receivedMWOEs;
	GenericQueue<JoinMessage> awaitingNSMsgs;
	bool initial;

	GenericQueue<SubscriptionMessage> bufferedSubs;



	void wakeUp();
	void sleep();
protected:
	virtual void handleMessage(cMessage* msg);
	virtual void handleNameServerMessage(NSMessage* nsm);
	virtual void handleSubscription(SubscriptionMessage* sm);


	//GHS algorithm
	void initiateProtocol();

	void handleConnectionMessage(ConnectMessage* msg);
	void handleUpdateMessage(UpdateMessage* msg);
	void handleMWOEMessage(MWOEMessage* msg);
	void handleExpandMessage(ExpandMessage* msg);
	void handleJoinMessage(JoinMessage* msg);



	void sendConnection(Broker* broker);
	void findAndSendMWOE();
	void sendUpdate();
	void connect(ConnectMessage* msg);
	void findBestMWOE();

	long calculateNewLeader(LinkedList<long>* m);
	bool isLeader();
	bool allMWOEReceived();
	bool isConverged();
	double finishTime();
	void memberUnion(LinkedList<long>* newMembers);
	void memberCopy(LinkedList<long>* newMembers);
	ConnectMessage* getReceivedConnectsForCurrentConvergence(Broker* broker, long convergence);

};

#endif /* OPTIMALBROKER_H_ */
