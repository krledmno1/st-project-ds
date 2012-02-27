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

#ifndef STMESSAGE_H_
#define STMESSAGE_H_

/*
 * This is the superclass of all the Messages as we discussed. It will feature all types of the derivated messages, and it will provide a "type" check mechanism.
 */
#include <cmessage.h>

class STMessage: public cMessage {
public:
	STMessage();
	virtual ~STMessage();

	int getType();
	//TODO I dont know how to access them, if you could implement the type checking using constants, I'm still using the dynamic_cast
	static const int NAME_SERVER_MSG = 0;
	static const int CONNECTION_REQUEST_MSG = 1;
	static const int DISCONNECTION_REQUEST_MSG = 2;
	static const int SUBSCRIPTION_MESSAGE = 3;
	static const int PUBLISH_MESSAGE = 4;
	static const int UNSUBSCRIPTION_MESSAGE = 5;
	static const int NEW_BROKER_NOTIFICATION = 6;

	static const int CONNECTION_MESSAGE = 7;
	static const int UPDATE_MESSAGE = 8;
	static const int MWOE_MESSAGE = 9;
	static const int EXPAND_MESSAGE = 10;

protected:
	int messageType;
};

#endif /* STMESSAGE_H_ */
