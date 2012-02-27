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

#ifndef MAP_H_
#define MAP_H_

#include <stdlib.h>
#include <KeyValue.h>




template<class A, class B>
class Map {
public:
	Map();
	virtual ~Map();
	void setMapping(A* key, B value);
	B removeMapping(A* key);
	B getValue(A* key);
	B removeLastMapping();
	void removeAll();

private:
	KeyValue<A,B>** mappings;
	int length;
	int physicalLength;
};

template<class A, class B>
Map<A,B>::Map() {
	this->length=0;
	this->physicalLength=0;
	this->mappings = NULL;
}

template<class A, class B>
Map<A,B>::~Map() {
	removeAll();
}

template<class A, class B>
void Map<A,B>::setMapping(A* key, B value)
{
	if(this->physicalLength==0)
	{
		//init
		this->physicalLength = 4;
		this->length = 1;

		this->mappings = new KeyValue<A,B>*[this->physicalLength];
		this->mappings[0] = new KeyValue<A,B>();
		this->mappings[0]->key = key;
		this->mappings[0]->value = value;

	}
	else
	{
		if(length>=physicalLength)
		{
			//extend
			int newLength = this->physicalLength*2;
			KeyValue<A,B>** newMappings = new KeyValue<A,B>*[newLength];
			for(int i =0;i<length;i++)
			{
				newMappings[i] = this->mappings[i];
			}
			newMappings[length] = new KeyValue<A,B>();
			newMappings[length]->key = key;
			newMappings[length]->value = value;


			delete [] this->mappings;
			this->mappings = newMappings;
			this->physicalLength = newLength;

			this->length++;


		}
		else
		{
			//just add
			this->mappings[this->length] = new KeyValue<A,B>();
			this->mappings[this->length]->key = key;
			this->mappings[this->length]->value = value;
			this->length++;
		}
	}
}


template<class A, class B>
B Map<A,B>::removeMapping(A* key)
{
	B ret = NULL;
	if(length==0)
	{
		//do nothing
	}
	else
	{
		if(length==physicalLength/2)
		{
			//remove and shrink
			bool cond = false;
			int i = 0;
			for(int i =0;i<length;i++)
			{
				if(mappings[i]->key==key)
				{
					cond = true;
					break;
				}
			}
			if(cond)
			{
				//exists
				ret = mappings[i]->value;
				delete mappings[i];
				int newLength = physicalLength/2;
				KeyValue<A,B>** newMappings = new KeyValue<A,B>*[newLength];
				int k = 0;
				for(int j =0;j<length;j++)
				{
					if(j!=i)
						newMappings[k++] = this->mappings[j];
				}
				delete [] this->mappings;
				this->mappings = newMappings;
				this->physicalLength = newLength;
				this->length--;


			}
			else
			{
				//do nothing
			}



		}
		else
		{
			//just remove
			bool cond = false;
			int i = 0;
			for(int i =0;i<length;i++)
			{
				if(mappings[i]->key==key)
				{
					cond = true;
					break;
				}
			}
			if(cond)
			{
				//exists
				ret = mappings[i]->value;
				delete mappings[i];
				KeyValue<A,B>** newMappings = new KeyValue<A,B>*[physicalLength];
				int k = 0;
				for(int j =0;j<length;j++)
				{
					if(j!=i)
						newMappings[k++] = this->mappings[j];
				}
				delete [] this->mappings;
				this->mappings = newMappings;
				this->length--;

			}
			else
			{
				//do nothing
			}

		}
	}
	return ret;
}

template<class A, class B>
B Map<A,B>::removeLastMapping()
{
	B ret=NULL;
		if(length==0)
		{
			//do nothing
		}
		else
		{
			if(length==physicalLength/2)
			{
				//remove and shrink
				this->length--;
				ret = mappings[this->length]->value;
				delete mappings[this->length];

				int newLength = physicalLength/2;
				KeyValue<A,B>** newMappings = new KeyValue<A,B>*[newLength];

				for(int i =0;i<length;i++)
				{
					newMappings[i] = this->mappings[i];
				}

				delete [] this->mappings;
				this->mappings = newMappings;
				this->physicalLength = newLength;
			}
			else
			{
				//just remove
				this->length--;
				ret = mappings[this->length]->value;
				delete mappings[this->length];
			}
		}
		return ret;
}

template<class A, class B>
B Map<A,B>::getValue(A* key)
{
	for(int i =0;i<length;i++)
	{
		if(mappings[i]->key==key)
		{
			return mappings[i]->value;
		}
	}
	return NULL;
}
template<class A, class B>
void Map<A,B>::removeAll()
{
	for(int i =0;i<length;i++)
	{
		delete mappings[i];
	}
	delete [] mappings;
	mappings = NULL;
	length = 0;
	physicalLength = 0;
}
#endif /* MAP_H_ */
