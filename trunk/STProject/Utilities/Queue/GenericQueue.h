#ifndef CHECKQUEUEHPP
#define CHECKQUEUEHPP

#include "LinkedList.h"

template<class T>
class GenericQueue
{
private:
	LinkedList<T> list;
public:
	void enqueue(T* t);
	T* dequeue();
	T* checkTop();
	bool isEmpty();
	void dequeueAll();
	GenericQueue(void);
	~GenericQueue(void);
};

template<class T>
GenericQueue<T>::GenericQueue()
{

}

template<class T>
void GenericQueue<T>::enqueue(T* t)
{
	list.addToBack(t);
}

template<class T>
T* GenericQueue<T>::dequeue()
{
	return list.removeFromFront();
}

template<class T>
T* GenericQueue<T>::checkTop()
{
	return list.getStart();
}

template<class T>
bool GenericQueue<T>::isEmpty()
{
	return list.isEmpty();
}

template<class T>
void GenericQueue<T>::dequeueAll()
{
	list.removeAll();
}

template<class T>
GenericQueue<T>::~GenericQueue()
{
	dequeueAll();
}

#endif
