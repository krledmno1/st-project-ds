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

#include "NetworkConditionTable.h"

NetworkConditionTable::NetworkConditionTable() {
	// TODO Auto-generated constructor stub

	clientNum = 0;
	brokerNum = 0;

}
NetworkConditionTable::NetworkConditionTable(int cNum, int bNum) {

	clientNum = cNum;
	brokerNum = bNum;

}
double NetworkConditionTable::getDelay(STNode* n1, STNode* n2)
{
	Map<STNode,double>* next = table.getValue(n1);
	return next->getValue(n2);
}
Map<STNode,Map<STNode,double>*>* NetworkConditionTable::getTable()
{
	return &table;
}

NetworkConditionTable::~NetworkConditionTable() {
	// TODO Auto-generated destructor stub
}
