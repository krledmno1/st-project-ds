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

#ifndef JOINMESSAGE_H_
#define JOINMESSAGE_H_

#include "STMessage.h"
#include "Broker.h"

class JoinMessage: public STMessage {
public:

	long convergenceNo;
	Broker* requester;
	bool leaving;
	JoinMessage();
	virtual ~JoinMessage();
};

#endif /* JOINMESSAGE_H_ */
