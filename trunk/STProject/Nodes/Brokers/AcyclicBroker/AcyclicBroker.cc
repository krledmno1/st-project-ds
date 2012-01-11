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

#include "AcyclicBroker.h"

Define_Module(AcyclicBroker);

AcyclicBroker::AcyclicBroker() {}
AcyclicBroker::~AcyclicBroker() {}

void AcyclicBroker::sleep(){
	//EV << "AcyclicBroker: Method Sleep"; //check if virtualization works.. it does...
	//broker disconnection is a lot messier than client disconnection.
	//step0: decide if I can sleep (if I'm the only broker present, means I cannot sleep) Thus, I check if I have any connected Brokers
	//TODO what if in the same time 2 connected brokers decide to disconnect? Hell will break loose, we need to use a sort of "locking" mechanism in this
	//std::vector<NeighbourEntry*>* brokersVectorPointer = ;
	std::vector<NeighbourEntry*> brokersVector = neighboursMap.getBrokersVector();
	if (brokersVector.size()<=0) { //it means we cannot sleep, we reschedule the sleep
		scheduleAt(simTime() + par("SleepDelay"), sleepMsg);
		EV << "Cannot sleep, I'm alone";
		//delete(brokersVectorPointer); //stupid garbage collection
		return;
	}
	//step1: we need to redirect every other Broker, in order not to break the chain
	if (brokersVector.size()>1){ //if we're connected with only 1 broker we're not breaking any chain
		for (unsigned int i=1;i<brokersVector.size();i++){
			if (brokersVector[i]!=NULL && brokersVector[i-1]!=NULL){
				send(new ConnectionRequestMessage(brokersVector[i]->getNeighbour()),brokersVector[i-1]->getOutGate());
				send(new ConnectionRequestMessage(brokersVector[i-1]->getNeighbour()),brokersVector[i]->getOutGate());
			} else {
				EV << "AcyclicBroker: brokersVector got NULL entries";
			}

		}
	}
	//step2: unregister from NameServ through a Disconnect message, such that we do not receive any more connection requests from either brokers or clients
	sendDirect(new DisconnectionRequestMessage(this), getNSGate());
	//step3: send disconnection requests to all neighbors
	std::vector<NeighbourEntry*> neighbours = neighboursMap.getNeighboursVector();
	for (unsigned int i = 0; i < neighbours.size(); i++) {
		if (neighbours[i] != NULL) {
			send(new DisconnectionRequestMessage(this), neighbours[i]->getOutGate());
			neighbours[i]->getOutGate()->disconnect();
			neighboursMap.removeMapping(neighbours[i]->getNeighbour()); //this entry will become null
		}
	}
	//step4: schedule a rewake
	scheduleAt(simTime() + par("WakeUpDelay"), wakeUpMsg);
}
