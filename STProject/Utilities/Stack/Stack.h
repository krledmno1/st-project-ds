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

#ifndef STACK_H_
#define STACK_H_

#include "LinkedList.h"

template<class T>
class Stack {
public:

	void push(T* t);
	T* pop();
	T* top();
	bool isEmpty();

	Stack();
	virtual ~Stack();

private:
	LinkedList<T> stack;
};



template<class T>
Stack<T>::Stack() {
	// TODO Auto-generated constructor stub

}
template<class T>
void Stack<T>::push(T* t)
{
	stack.addToFront(t);
}

template<class T>
T* Stack<T>::pop()
{
	if(!stack.isEmpty())
		return stack.removeFromFront();
	else return NULL;
}

template<class T>
T* Stack<T>::top()
{
	if(!stack.isEmpty())
		return stack.getStart();
	else return NULL;
}

template<class T>
bool Stack<T>::isEmpty(){
	return stack.isEmpty();
}


template<class T>
Stack<T>::~Stack() {
	// TODO Auto-generated destructor stub
}


#endif /* STACK_H_ */
