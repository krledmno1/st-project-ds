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

#include "OptimalBroker.h"

Define_Module(OptimalBroker);

OptimalBroker::OptimalBroker() {
	// TODO Auto-generated constructor stub
converged = true;

}
void OptimalBroker::wakeUp() {
	sendDirect(new NSMessage(dynamic_cast<STNode*>(this)), getNSGate());
}

void OptimalBroker::handleMessage(cMessage* msg)
{
	if (msg == wakeUpDelayMsg)
	{
		wakeUp();
	}
	else if (msg == sleepDelayMsg)
	{
		sleep();

	} else
		if (dynamic_cast<STMessage*>(msg) != NULL)
		{
			STMessage* stm = dynamic_cast<STMessage*>(msg);
			if (stm->getType() == stm->NAME_SERVER_MSG) {
				handleNameServerMessage(dynamic_cast<NSMessage*>(msg));
			} else if (stm->getType() == stm->CONNECTION_REQUEST_MSG) {
				handleConnectionRequest(dynamic_cast<ConnectionRequestMessage*>(msg));
			} else if (stm->getType() == stm->DISCONNECTION_REQUEST_MSG) {
				handleDisconnectionRequest(dynamic_cast<DisconnectionRequestMessage*>(msg));
			} else if (stm->getType() == stm->SUBSCRIPTION_MESSAGE) {
				handleSubscription(dynamic_cast<SubscriptionMessage*>(msg));
			} else if (stm->getType() == stm->UNSUBSCRIPTION_MESSAGE) {
				handleUnsubscription(dynamic_cast<UnsubscriptionMessage*>(msg));
			} else if (stm->getType() == stm->PUBLISH_MESSAGE) {
				handlePublish(dynamic_cast<PublishMessage*>(msg));
			} else if(stm->getType() == stm->CONNECTION_MESSAGE){
				handleConnectionMessage(dynamic_cast<ConnectMessage*>(msg));
			} else if(stm->getType() == stm->MWOE_MESSAGE){
				handleMWOEMessage(dynamic_cast<MWOEMessage*>(msg));
			} else if(stm->getType() == stm->EXPAND_MESSAGE){
				handleExpandMessage(dynamic_cast<ExpandMessage*>(msg));
			} else if(stm->getType() == stm->UPDATE_MESSAGE){
				handleUpdateMessage(dynamic_cast<UpdateMessage*>(msg));
			} else {
				EV << "Broker: Unknown STMessage type \n";
			}
		} else {
			EV << "Broker: Unknown message type \n";
		}
}
void OptimalBroker::handleNameServerMessage(NSMessage* nsm)
{


	if(converged)
	{

			LinkedList<STNode>* list = dynamic_cast<LinkedList<STNode>*>(nsm->getRequestedNodes());

			availableBrokers.removeAll();

			LinkedList<NeighbourEntry>* adjecentBrokers =  neighboursMap.getBrokersList();
			for(Node<NeighbourEntry>* ne = adjecentBrokers->start;(ne!=NULL); ne=ne->getNext())
			{

				ne->getContent()->getOutGate()->disconnect();
				neighboursMap.removeMapping(ne->getContent()->getNeighbour());
				//receivedConnects.removeMapping(dynamic_cast<Broker*>(ne->getContent()->getNeighbour()));
			}

			for(int i = 0; list->getSize();i++)
			{
				availableBrokers.addToBack(dynamic_cast<Broker*>(list->removeFromFront()));
			}
			if (!availableBrokers.isEmpty())
			{
				initiateProtocol();
			}

			cancelAndDelete(nsm);


	}
	else
	{
		scheduleAt(simTime()+par("ConverganceDelay").doubleValue()*(availableBrokers.getSize()+1),nsm);
	}
}


void OptimalBroker::initiateProtocol()
{
			id = getId();
			converged = false;
			componentId = getId();	//trivial leader election
			leaderHop = NULL; 		//if leader hop is null than don't send but schedule
			members.removeAll();
			members.addToBack(new long(id));
			sentConnects.removeAll();
			receivedMWOEs.removeAll();

			findAndSendMWOE();

}

bool OptimalBroker::isConverged()
{
	if(members.getSize()==availableBrokers.getSize()+1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void OptimalBroker::findAndSendMWOE()
{

	converged = isConverged();
	if(!converged)
	{

		//find best MWOE
		Node<Broker>* requestedNode = availableBrokers.start;
		Broker* best = NULL;
		double bestDelay = simulation.getSystemModule()->getSubmodule("nameServer",0)->par("maxDelay");
		bestDelay++;

		for(int i=0;i<availableBrokers.getSize();i++)
		{
			Broker* candidate = requestedNode->getContent();
			double candidateDelay = this->ping(candidate);
			if(candidate!=this && candidateDelay<bestDelay)
			{
				bool cond1 = true;
				Node<long>* nod = members.start;
				for(int j=0;j<members.getSize();j++)
				{
					if(candidate->getId()==*(nod->getContent()))
					{
						cond1 = false;
						break;
					}
					nod=nod->getNext();
				}


				if(cond1 && neighboursMap.getOutputGate(dynamic_cast<STNode*>(candidate))==NULL)	//look only for adecent links that are not connected
				{
					best = candidate;
					bestDelay = candidateDelay;
				}
			}
			requestedNode = requestedNode->getNext();
		}
		MWOEMessage* m = new MWOEMessage();
		if(best!=NULL)
		{
			//exists the best mwoe

			m->broker=best;
			m->delay = bestDelay;


			if(!isLeader())
			{
				//send MWOE to leader
				m->piggyback.push(this);
				send(m,leaderHop);
			}
			else
			{
				//add to the list

				scheduleAt(simTime(),m);
			}

		}
		else
		{
			//no MWOE
			m->broker=NULL;
			m->delay = 0;


			if(!isLeader())
			{
				//send MWOE to leader
				m->piggyback.push(this);
				send(m,leaderHop);
			}
			else
			{
				//add to the list
				receivedMWOEs.addToBack(m);
				scheduleAt(simTime(),m);
			}
		}
	}
	else
	{
		//algorithm converged
		sentConnects.removeAll();


		LinkedList<NeighbourEntry>* adjecentBrokers =  neighboursMap.getBrokersList();
		for(Node<NeighbourEntry>* ne = adjecentBrokers->start;(ne!=NULL); ne=ne->getNext())
		{
			receivedConnects.removeMapping(dynamic_cast<Broker*>(ne->getContent()->getNeighbour()));
		}
	}

}

void OptimalBroker::handleMWOEMessage(MWOEMessage* msg)
{
	if(!isLeader())
	{

		msg->piggyback.push(this);
		send(msg,leaderHop);
	}
	else
	{
		receivedMWOEs.addToBack(msg);
		if(allMWOEReceived())
		{
			findBestMWOE();
			receivedMWOEs.removeAll();
		}


	}
}

void OptimalBroker::handleExpandMessage(ExpandMessage* msg)
{
	if(msg->piggyback.isEmpty())
	{

		sendConnection(msg->broker);
		cancelAndDelete(msg);

	}
	else
	{
		cGate* forward = neighboursMap.getOutputGate(msg->piggyback.pop());
		send(msg,forward);
	}
}

void OptimalBroker::handleUpdateMessage(UpdateMessage* msg)
{
		Node<long>* node = msg->members.start;
		for(int i=0;i<msg->members.getSize();i++)
		{
			bool cond = true;
			for(Node<long>* n=members.start;n!=NULL;n=n->getNext())
			{
				if(*(n->getContent())==*(node->getContent()))
				{
					cond=false;
				}
			}


			if(cond)
			{
				members.addToBack(new long(*(node->getContent())));
			}
			node = node->getNext();
		}

		long newLeader = calculateNewLeader(&members);
		if(newLeader!=componentId)
		{
			leaderHop = neighboursMap.getOutputGate(msg->sender);
		}

		Broker* sender = msg->sender;
		componentId=newLeader;



		LinkedList<NeighbourEntry>* bList = neighboursMap.getBrokersList();
		for (NeighbourEntry* ne = bList->removeFromFront();ne!=NULL;ne = bList->removeFromFront())
		{
			if(dynamic_cast<Broker*>(ne->getNeighbour())!=sender)
			{
				UpdateMessage* u = new UpdateMessage();
				u->componentId=msg->componentId;
				u->sender=this;
				Node<long>* node = msg->members.start;
				for(int i=0;i<msg->members.getSize();i++)
				{
					u->members.addToBack(new long(*(node->getContent())));
					node = node->getNext();
				}
				send(u,ne->getOutGate());
			}


		}
		findAndSendMWOE();

		cancelAndDelete(msg);

}

void OptimalBroker::handleConnectionMessage(ConnectMessage* msg)
{

	ConnectMessage* con = sentConnects.getValue(msg->broker);
	if(con!=NULL)
	{
		connect(msg);
		findAndSendMWOE();
	}
	else
	{
		receivedConnects.setMapping(msg->broker,msg);
	}

}


void OptimalBroker::sendConnection(Broker* broker)
{
	ConnectMessage* con = new ConnectMessage();

	for(Node<long>* node = members.start; node !=NULL ;node = node->getNext())
	{
		if(node !=NULL)
		{
			if(node->getContent() !=NULL)
			{
				long* b = new long;
				*b=*(node->getContent());
				con->members.addToBack(b);
			}
		}
	}
	con->broker=this;

	ConnectMessage* msg = receivedConnects.getValue(broker);
	if(msg!=NULL)
	{
		connect(msg);
		findAndSendMWOE();
	}
	else
	{
		sentConnects.setMapping(broker,con);
	}
	cGate* his = broker->getFreeInputGate();
	sendDirect(con,his);

}

void OptimalBroker::connect(ConnectMessage* msg)
{

	for(Node<long>* node = msg->members.start;node !=NULL;node = node->getNext())
	{
		bool cond = true;
		for(Node<long>* n=members.start;n!=NULL;n=n->getNext())
		{
			//there is a straaange derefferencing problem here!!!
			if(node!=NULL && n!=NULL)
			{
				if(node->getContent()!=NULL&&n->getContent()!=NULL)
				{
					if(*(n->getContent())==*(node->getContent()))
					{
						cond=false;
					}
				}
			}
		}

		if(cond)
		{
			members.addToBack(new long(*(node->getContent())));
		}

	}

	long newLeader = calculateNewLeader(&members);

	cGate* myGate = getFreeOutputGate();
	cGate* hisGate = msg->broker->getFreeInputGate();
	if (myGate == NULL || hisGate == NULL)
	{
		//to do
	}

	cDelayChannel* cha = cDelayChannel::create("DelayChannel");
	cha->setDelay(this->ping(msg->broker));

	myGate->connectTo(hisGate,cha);


	if(newLeader!=componentId)
	{
		leaderHop = myGate;
	}
	componentId=newLeader;

	LinkedList<NeighbourEntry>* bList = neighboursMap.getBrokersList();
	for (NeighbourEntry* ne = bList->removeFromFront();ne!=NULL;ne = bList->removeFromFront())
	{
		UpdateMessage* u = new UpdateMessage();
		u->componentId=componentId;
		u->sender=this;
		Node<long>* node = members.start;
		for(int i=0;i<members.getSize();i++)
		{
			u->members.addToBack(new long(*(node->getContent())));
			node = node->getNext();
		}
		send(u,ne->getOutGate());
	}


	neighboursMap.addMapping(msg->broker, myGate);
	cancelAndDelete(msg);
}


double OptimalBroker::finishTime()
{

	double time = simTime().dbl();
	LinkedList<NeighbourEntry>* adjecentBrokers =  neighboursMap.getBrokersList();
	for(Node<NeighbourEntry>* ne = adjecentBrokers->start;(ne!=NULL); ne=ne->getNext())
	{

					if(time < ne->getContent()->getOutGate()->getChannel()->getTransmissionFinishTime().dbl())
					{
						time = ne->getContent()->getOutGate()->getChannel()->getTransmissionFinishTime().dbl();
					}

	}
	return time;
}

long OptimalBroker::calculateNewLeader(LinkedList<long>* m)
{
	long min = componentId;
	Node<long>* node = m->start;
	for(int i=0;i<m->getSize();i++)
	{
		if(*(node->getContent())<min)
		{
			min=*(node->getContent());
		}
		node = node->getNext();
	}
	return min;
}

void OptimalBroker::findBestMWOE()
{
	MWOEMessage* bestMsg = NULL;
	Broker* best = NULL;
	double bestDelay = simulation.getSystemModule()->getSubmodule("nameServer",0)->par("maxDelay");
	bestDelay++;
	Node<MWOEMessage>* node = receivedMWOEs.start;
	for(int i=0;i<receivedMWOEs.getSize();i++)
	{
		if(node->getContent()->delay<bestDelay && node->getContent()->broker!=NULL)
		{
			best = node->getContent()->broker;
			bestDelay = node->getContent()->delay;
			bestMsg = node->getContent();
		}
		node = node->getNext();
	}
	if(best!=NULL)
	{
		ExpandMessage* e = new ExpandMessage();
		e->broker = bestMsg->broker;
		Stack<Broker> temp;
		while(!bestMsg->piggyback.isEmpty())
		{

			temp.push(bestMsg->piggyback.pop());
		}
		while(!temp.isEmpty())
		{
			e->piggyback.push(temp.pop());
		}

		if(e->piggyback.top()==NULL)
		{
			scheduleAt(simTime(),e);
		}
		else
		{
			cGate* sendTo = neighboursMap.getOutputGate(e->piggyback.pop());
			send(e,sendTo);
		}

	}
	else
	{
		//bubble: Converged!
		//TO DO: Some maintanance of data structures...
	}
	Node<MWOEMessage>* n = receivedMWOEs.start;
	for(int i=0;i<receivedMWOEs.getSize();i++)
	{
		cancelAndDelete(n->getContent());
		n=n->getNext();
	}
}

bool OptimalBroker::allMWOEReceived()
{
	if(receivedMWOEs.getSize()==members.getSize())
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool OptimalBroker::isLeader()
{
	if(id == componentId)
		return true;
	else return false;
}

OptimalBroker::~OptimalBroker() {
	// TODO Auto-generated destructor stub
}
