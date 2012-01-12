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

#include "SubscriptionMonitor.h"
#include "stdlib.h"
#include "time.h"

SubscriptionMonitor::SubscriptionMonitor(int s) {
	size = s;
	subscriptions.resize(size);
	for (int i=0;i<size;i++){
		subscriptions[i] = false;
	}
	srand(time(0));
}

SubscriptionMonitor::~SubscriptionMonitor() {}

void SubscriptionMonitor::subscribe(int topic){
	subscriptions[topic] = true;
}
void SubscriptionMonitor::unsubscribe(int topic){
	subscriptions[topic] = false;
}

void SubscriptionMonitor::unsubscribeAll(){
	for (int i=0;i<size;i++){subscriptions[i] = false;}
}

bool SubscriptionMonitor::isSubscribed(int topic){
	return subscriptions[topic];
}

int SubscriptionMonitor::getRandomSubscribedTopic(){//-1 if no subscribed topic
	//first we check that we have at least one topic
	bool empty = true;
	for (int i=0;i<size && empty;i++) {if (subscriptions[i]) empty = false;}
	if (empty){//if there's no subscribed topic, we just return -1
		return -1;
	}
	//now we're sure we have at least one subscribed topic
	int topicChosen = rand() % size;
	while (!subscriptions[topicChosen]){ //this way we make sure we didn't pick a non subscribed topic
		topicChosen = rand() % size;
	}
	return topicChosen;
}
int SubscriptionMonitor::getRandomUnsubscribedTopic(){//-1 if all topics are subscribed
	//first we check that we have at least one free topic
	bool full = true;
	for (int i=0;i<size && full;i++) {if (!subscriptions[i]) full = false;}
	if (full){//if there's no free topic, we just return -1
		return -1;
	}
	//now we're sure we have at least one topic free
	int topicChosen = rand() % size;
	while (subscriptions[topicChosen]){ //this way we make sure we didn't pick a subscribed topic
		topicChosen = rand() % size;
	}
	return topicChosen;
}
