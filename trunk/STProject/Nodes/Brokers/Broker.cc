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

#include "Broker.h"
#include "Client.h"
#include "NSMessage.h"
#include "ConnectionRequestMessage.h"
#include "DisconnectionRequestMessage.h"

Define_Module(Broker);

Broker::Broker() {}
Broker::~Broker() {}

void Broker::initialize() {
	scheduleAt(par("WakeUpDelay"), wakeUpDelayMsg);
}
void Broker::handleMessage(cMessage *msg) {
	if (msg == wakeUpDelayMsg) {
		wakeUp();
	} else if (msg == sleepDelayMsg) {
		sleep();
	} else if (dynamic_cast<STMessage*>(msg) != NULL) {
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
		} else {
			EV << "Broker: Unknown STMessage type \n";
		}
	} else {
		EV << "Broker: Unknown message type \n";
	}
}

void Broker::wakeUp() {
	sendDirect(new NSMessage(dynamic_cast<STNode*>(this)), getNSGate());
}

void Broker::sleep() {
	//broker disconnection is a lot messier than client disconnection.
	//step0: decide if I can sleep (if I'm the only broker present, means I cannot sleep) Thus, I check if I have any connected Brokers
	//TODO what if in the same time 2 connected brokers decide to disconnect? Hell will break loose, we need to use a sort of "locking" mechanism in this
	if (neighboursMap.hasBrokers() == false) { //it means we cannot sleep, we reschedule the sleep
		scheduleAt(simTime() + par("SleepDelay"), sleepDelayMsg);
		EV << "Cannot sleep, I'm alone";
		return;
	}
	//step1: unregister from NameServ through a Disconnect message, such that we do not receive any more connection requests from either brokers or clients
	sendDirect(new DisconnectionRequestMessage(this), getNSGate());
	//step2: send disconnection requests to all neighbors
	std::vector<NeighbourEntry*> neighbours = neighboursMap.getNeighboursVector();
	for (unsigned int i = 0; i < neighbours.size(); i++) {
		if (neighbours[i] != NULL) {
			send(new DisconnectionRequestMessage(this), neighbours[i]->getOutGate());
			neighbours[i]->getOutGate()->disconnect();
			neighboursMap.removeMapping(neighbours[i]->getNeighbour()); //this entry will become null
		}
	}
	//step3: schedule a rewake
	scheduleAt(simTime() + par("WakeUpDelay"), wakeUpDelayMsg);
}

void Broker::handleNameServerMessage(NSMessage* nsm) {
	LinkedList<STNode>* requestedNodes = dynamic_cast<LinkedList<STNode>*>(nsm->getRequestedNodes());
	if (!requestedNodes->isEmpty())
	{
		Node<STNode>* requestedNode = requestedNodes->start;
		Broker* best = NULL;
		double bestDelay = simulation.getSystemModule()->getSubmodule("nameServer",0)->par("maxDelay");
		bestDelay++;
		for(int i=0;i<requestedNodes->getSize();i++)
		{
			Broker* candidate = dynamic_cast<Broker*>(requestedNode->getContent());
			double candidateDelay = this->ping(candidate);
			if(candidate!=this && candidateDelay<bestDelay)
			{
				best = candidate;
				bestDelay = candidateDelay;
			}
			requestedNode = requestedNode->getNext();
		}

		if(best!=NULL)		//if there exists best one
		{

			//picked out the best one
			cGate* myGate = getFreeOutputGate();
			cGate* hisGate = best->getFreeInputGate();
			if (myGate == NULL || hisGate == NULL)
			{
				//retry later
				//why is this possible?
				scheduleAt(par("WakeUpDelay"), wakeUpDelayMsg);
				cancelAndDelete(nsm);
				return;
			}

			cDelayChannel* cha = cDelayChannel::create("DelayChannel");
			cha->setDelay(bestDelay);

			myGate->connectTo(hisGate,cha);
			neighboursMap.addMapping(best, myGate);
			//TODO handle the case in which you have no free InputGate
			send(new ConnectionRequestMessage(this), myGate);
			cancelAndDelete(nsm);
			return;
		}
		else
		{
			//this means that there is only one broker in the list
			//and it's this one (which is impossible, but hey... better safe that sorry)
			//retry later
			scheduleAt(par("WakeUpDelay"), wakeUpDelayMsg);
			cancelAndDelete(nsm);
			return;
		}
	}
	else
	{
		//we assume we're the first Broker registering to the network or only one not sleeping
		scheduleAt(simTime() + par("SleepDelay"), sleepDelayMsg);
		cancelAndDelete(nsm);
	}
}

void Broker::handleConnectionRequest(ConnectionRequestMessage* crm) {
	//TODO here you should separate between client gates, and broker gates (and produce corrisponding mappings, with subscribings, etc.. )
	//TODO also, it should be handled the case in which the broker is out of Free Gates, in which case he should return an error Message (not available, something like this)
	STNode* stn = crm->getRequesterNode();
	cGate* outGate = getFreeOutputGate();

	cDelayChannel* cha = cDelayChannel::create("DelayChannel");
	cha->setDelay(this->ping(stn));

	outGate->connectTo(stn->getFreeInputGate(),cha);

	neighboursMap.addMapping(stn, outGate);
	cancelAndDelete(crm);
}

void Broker::handleDisconnectionRequest(DisconnectionRequestMessage* drm) {
	cGate* myOutputGate = neighboursMap.getOutputGate(drm->getRequesterNode());
	if (myOutputGate==NULL){
		EV << "Broker: big error, the disconnection requesting node is not known to me (not mapped to any of my outGates)";
		return;
	}
	myOutputGate->disconnect();
	neighboursMap.removeMapping(drm->getRequesterNode()); //if you remove the mapping, you automatically remove all the subscriptions to him, all we need is now to refresh subscriptions in some way, to see if we need to unsubscribe to any particular topic
	cancelAndDelete(drm);
}

cGate* Broker::getFreeInputGate() {
	int nr = par("nrInputGates");
	for (int i = 0; i < nr; i++) {
		cGate* g = gate("in", i);
		if (!g->isConnected())
			return g;
	}
	return NULL;
}

cGate* Broker::getFreeOutputGate() {
	int nr = par("nrOutputGates");
	for (int i = 0; i < nr; i++) {
		cGate* g = gate("out", i);
		if (!g->isConnected())
			return g;
	}
	return NULL;
}
//I do not handle in any way the P/S in the Broker
void Broker::handleSubscription(SubscriptionMessage* sm){cancelAndDelete(sm);}
void Broker::handleUnsubscription(UnsubscriptionMessage* um){cancelAndDelete(um);}
void Broker::handlePublish(PublishMessage* pm){cancelAndDelete(pm);}
