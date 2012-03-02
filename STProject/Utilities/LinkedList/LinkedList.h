#ifndef LINKEDLISTHPP
#define LINKEDLISTHPP

#include <stdlib.h>

template<class T>
class Node
{
private:
	T* content;
	Node<T>* next;
public:
	Node();
	void setContent(T* t);
	void setNext(Node* n);
	T* getContent();
	Node<T>* getNext();
	~Node();
};

template<class T>
class LinkedList
{
	
public:
	Node<T>* start;
	Node<T>* end;
	int size;
	LinkedList();
	int getSize();
	void addToFront(T* n);
	void addToBack(T* n);
	T* removeFromFront();
	T* getStart();
	Node<T>* find(T* n);
	Node<T>* findBefore(T* n);
	T* removeNode(T* n);
	void removeAll();
	bool isEmpty();
	~LinkedList();
};

template<class T>
Node<T>::Node()
{
	content = 0;
	next = 0;

}

template<class T>
void Node<T>::setContent(T* t)
{
	this->content = t;
}

template<class T>
void Node<T>::setNext(Node* n)
{
	this->next = n;
}

template<class T>
//Node<T>* Node<T>::getContent()
T* Node<T>::getContent()
{
	return content;
}

template<class T>
Node<T>* Node<T>::getNext()
{
	return next;
}

template<class T>
Node<T>::~Node()
{

}

template<class T>
LinkedList<T>::LinkedList()
{
	start = 0;
	end = 0;
	size = 0;
}

template<class T>
bool LinkedList<T>::isEmpty()
{
	if(start==0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template<class T>
void LinkedList<T>::addToFront(T* t)
{
	Node<T>* node;
	node = new Node<T>();
	node->setContent(t);
	node->setNext(0);
	if(start==0 || end==0)
	{
		start = node;
		end = start;
	}
	else
	{
		if(start==end)
		{
			start = node;
			node->setNext(end);
		}
		else
		{
			node->setNext(start);
			start = node;
		}
	}
	size++;
}

template<class T>
void LinkedList<T>::addToBack(T* t)
{
	Node<T>* node;
	node = new Node<T>();
	node->setContent(t);
	node->setNext(0);
	if(start==0 && end==0)
	{
		start = node;
		end = start;
	}
	else
	{
		if(start == end)
		{
			start->setNext(node);
			end = node;
		}
		else
		{
			end->setNext(node);
			end = node;
		}

	}
	size++;
	
}

template<class T>
T* LinkedList<T>::removeFromFront()
{
	if(start==0 || end==0)
	{
		return 0;
	}
	else
	{
		size--;
		if(start==end)
		{
			T* con;
			con = start->getContent();
			delete start;
			start = 0;
			end = 0;
			return con;
		}
		else
		{
			T* con;
			con = start->getContent();
			Node<T>* temp;
			temp = start;
			start = start->getNext();
			delete temp;
			return con;
		}
	}

}

template<class T>
T* LinkedList<T>::getStart()
{
	return this->start->getContent();
}

template<class T>
void LinkedList<T>::removeAll()
{
	while(removeFromFront()!=0);

}

template<class T>
Node<T>* LinkedList<T>::find(T* n)
{
	if(start!=NULL)
	{
		Node<T>* trav=this->start;
		while(trav!=this->end)
		{
			if(trav->getContent()==n)
			{
				return trav;
			}
			trav=trav->getNext();
		}
		if(trav->getContent()==n)
		{
			return trav;
		}
	}
	return NULL;
}


template<class T>
Node<T>* LinkedList<T>::findBefore(T* n)
{
	if(start!=NULL)
	{
		if(start->getNext()!=NULL)
		{

			Node<T>* target=this->start;
			Node<T>* trav=this->start->getNext();

			while(trav!=this->end)
			{
				if(trav->getContent()==n)
				{
					return target;
				}
				target=trav;
				trav=trav->getNext();
			}
			if(trav->getContent()==n)
			{
				return target;
			}
		}
	}
	return NULL;
}


template<class T>
T* LinkedList<T>::removeNode(T* n)
{
	if(start->getContent()==n)
	{
		return removeFromFront();
	}
	Node<T>* node = findBefore(n);
	if(node!=NULL)
	{
		size--;
		T* ret = node->getNext()->getContent();
		Node<T>* temp = node->getNext();
		node->setNext(node->getNext()->getNext());
		if(end==temp)
		{
			end = node;
		}

		delete temp;
		return ret;
	}
	return NULL;


}

template<class T>
int LinkedList<T>::getSize()
{
	return size;
}

template<class T>
LinkedList<T>::~LinkedList()
{
	removeAll();
}

#endif
