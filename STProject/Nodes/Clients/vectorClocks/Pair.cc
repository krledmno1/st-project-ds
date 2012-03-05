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

#include "Pair.h"

Pair::Pair(STNode* n, int v) {
	node = n;
	value = v;
	// TODO Auto-generated constructor stub

}

Pair::~Pair() {
	// TODO Auto-generated destructor stub
}

STNode* Pair::getNode() const
{
    return node;
}

int Pair::getValue() const
{
    return value;
}

void Pair::setNode(STNode* node)
{
    this->node = node;
}

void Pair::setValue(int value)
{
    this->value = value;
}



