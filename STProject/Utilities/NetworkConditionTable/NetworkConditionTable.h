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

#ifndef NETWORKCONDITIONTABLE_H_
#define NETWORKCONDITIONTABLE_H_

#include <Map.h>
#include <STNode.h>
#include <LinkedList.h>

class STNode;


class NetworkConditionTable {
public:

	NetworkConditionTable();
	NetworkConditionTable(int cNum, int bNum, double min, double max);
	double getDelay(STNode* n1, STNode* n2);
	virtual ~NetworkConditionTable();
	Map<STNode,Map<STNode,double>*>* getTable();
private:
	int clientNum;
	int brokerNum;
	Map<STNode,Map<STNode,double>*> table;
	LinkedList<double> values;

	void printTable();
	bool isUnique(double delay);

};

#endif /* NETWORKCONDITIONTABLE_H_ */
