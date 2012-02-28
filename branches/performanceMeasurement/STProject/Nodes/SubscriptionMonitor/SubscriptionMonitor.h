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

#ifndef SUBSCRIPTIONMONITOR_H_
#define SUBSCRIPTIONMONITOR_H_
#include <vector>

/*
 * This component has the simple role of keeping track of whether the owner
 * is subscribed to a topic or not.
 */
class SubscriptionMonitor {
public:
	SubscriptionMonitor(int nrTopics);
	virtual ~SubscriptionMonitor();

	void subscribe(int topic);
	void unsubscribe(int topic);
	void unsubscribeAll();

	bool isSubscribed(int topic);

	int getRandomSubscribedTopic(); //-1 if no subscribed topic
	int getRandomUnsubscribedTopic(); //-1 if all topics are subscribed

private:
	std::vector<bool> subscriptions;
	int size;
};

#endif /* SUBSCRIPTIONMONITOR_H_ */
