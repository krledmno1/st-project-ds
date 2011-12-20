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

#ifndef BCNODE_H_
#define BCNODE_H_

class BCNode {
public:
	BCNode(cGate* b);
	virtual ~BCNode();

	cGate* getBrokerGate();

	void setNextNode(BCNode* ln);
	BCNode* getNextNode();
private:
	cGate* broker;
	BCNode* node;
};

#endif /* BCNODE_H_ */
