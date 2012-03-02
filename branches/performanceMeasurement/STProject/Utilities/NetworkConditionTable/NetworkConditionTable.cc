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
#include <iostream>

NetworkConditionTable::NetworkConditionTable() {
	// TODO Auto-generated constructor stub

	clientNum = 0;
	brokerNum = 0;

}
NetworkConditionTable::NetworkConditionTable(int cNum, int bNum, double min, double max) {

	clientNum = cNum;
	brokerNum = bNum;

		STNode* n1 = NULL;
		STNode* n2 = NULL;

		//create client-broker relation
		for(int i = 0;i<cNum;i++)
		{
			Map<STNode,double>* temp;
			n1 = dynamic_cast<STNode*>(simulation.getSystemModule()->getSubmodule("clients",i));
			temp = new Map<STNode,double>();
			for(int j = 0;j<bNum;j++)
			{

				n2 = dynamic_cast<STNode*>(simulation.getSystemModule()->getSubmodule("borkers",j));
				double delay = min+(((double)rand()/(double)RAND_MAX)*(max-min));
				while(!isUnique(delay))
				{
					delay = min+(((double)rand()/(double)RAND_MAX)*(max-min));
				}
				std::cout << delay <<endl;
				temp->setMapping(n2,delay);

			}
			table.setMapping(n1,temp);
		}

		//broker->client relation
		for(int i = 0;i<bNum;i++)
		{
			Map<STNode,double>* temp;
			n1 = dynamic_cast<STNode*>(simulation.getSystemModule()->getSubmodule("borkers",i));
			temp = new Map<STNode,double>();
			for(int j = 0;j<cNum;j++)
			{
				n2 = dynamic_cast<STNode*>(simulation.getSystemModule()->getSubmodule("clients",j));
				double delay = table.getValue(n2)->getValue(n1);
				std::cout << delay <<endl;


				temp->setMapping(n2,table.getValue(n2)->getValue(n1));
			}
			table.setMapping(n1,temp);
		}


		//broker-broker relation
		for(int i = 0;i<bNum;i++)
		{
			Map<STNode,double>* temp=NULL;


			n1 = dynamic_cast<STNode*>(simulation.getSystemModule()->getSubmodule("borkers",i));
			temp = table.getValue(n1);
			for(int j = 0;j<bNum;j++)
			{
				n2 = dynamic_cast<STNode*>(simulation.getSystemModule()->getSubmodule("borkers",j));
				double delay;

				if(i<j)
				{
					delay = min+(((double)rand()/(double)RAND_MAX)*(max-min));
					while(!isUnique(delay))
					{
						delay = min+(((double)rand()/(double)RAND_MAX)*(max-min));
					}
				}
				else
				{
					if(i>j)
					{
						delay=table.getValue(n2)->getValue(n1);
					}
					else
					{
						delay=0;
					}

				}
				std::cout << delay <<endl;

				temp->setMapping(n2,delay);

			}
			table.setMapping(n1,temp);


		}


				printTable();

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


bool NetworkConditionTable::isUnique(double delay)
{
	if(delay==0)
	{
		return false;
	}
	for(Node<double>* node = values.start;node!=NULL;node=node->getNext())
	{
		if(*(node->getContent())==delay)
		{
			return false;
		}
	}
	values.addToBack(new double(delay));
	return true;


}

void NetworkConditionTable::printTable()
{
	STNode* n1;
	STNode* n2;
	for(int i = 0;i<clientNum+brokerNum;i++)
	{
		if(i<clientNum)
		{
			n1 = dynamic_cast<STNode*>(simulation.getSystemModule()->getSubmodule("clients",i));
		}
		else
		{
			n1 = dynamic_cast<STNode*>(simulation.getSystemModule()->getSubmodule("borkers",i-clientNum));

		}

		for(int j = 0;j<brokerNum;j++)
		{
			n2 = dynamic_cast<STNode*>(simulation.getSystemModule()->getSubmodule("borkers",j));
			if(i<clientNum)
			{

				std::cout << "Client " << i << " --> " << "Broker " << j << "   " << table.getValue(n1)->getValue(n2) << "\n";
			}
			else
			{
				std::cout << "Broker " << i-clientNum << " --> " << "Broker " << j << "   " << table.getValue(n1)->getValue(n2) << "\n";
			}
		}
	}

	std::cout << "End of table";

	for(int i = 0;i<brokerNum;i++)
	{
		n1 = dynamic_cast<STNode*>(simulation.getSystemModule()->getSubmodule("borkers",i));
		for(int j = 0;j<clientNum;j++)
		{
			n2 = dynamic_cast<STNode*>(simulation.getSystemModule()->getSubmodule("clients",j));
			std::cout << "Broker " << i << " --> " << "Client " << j << "   " << table.getValue(n1)->getValue(n2) << "\n";


		}
	}



}

NetworkConditionTable::~NetworkConditionTable() {
	// TODO Auto-generated destructor stub
}


