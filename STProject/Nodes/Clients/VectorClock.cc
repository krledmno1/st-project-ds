/*
 * VectorClock.cpp
 *
 *  Created on: Feb 23, 2012
 *      Author: dd
 */

#include "VectorClock.h"
#include "LinkedList.h"
#include "STNode.h"
#include "Pair.h"

//class Pair;

VectorClock::VectorClock() {
	timeStamp = new LinkedList<Pair>();
	// TODO Auto-generated constructor stub

}

VectorClock::~VectorClock() {
	delete timeStamp;
	// TODO Auto-generated destructor stub
}

LinkedList<Pair>* VectorClock::getTimeStamp()  {
	return timeStamp;
}

void VectorClock::setTimeStamp(LinkedList<Pair> *timeStamp) {

	for(Node<Pair> *p = timeStamp->start; p != NULL; p= p->getNext())
	{
		Pair *pair = new Pair(p->getContent()->getNode(), p->getContent()->getValue());
		this->timeStamp->addToBack(pair);
	}
}

void VectorClock::update(VectorClock *vc) {

	LinkedList<Pair>* tsExt = vc->getTimeStamp();
//	LinkedList<Pair> *tsInt = this->timeStamp;

	Node<Pair> (*pExt) = tsExt->start;

	if (pExt == NULL) {
		return;
	}

	while (pExt->getNext() != NULL) {
		Pair *pInt = search(pExt->getContent()->getNode());

		if (pExt->getContent() == NULL) {
			pExt = pExt->getNext();
			continue;
		}

		if (pInt != NULL) {
			int v1 = pExt->getContent()->getValue();
			int v2 = pInt->getValue();
			pInt->setValue(v1 < v2 ? v2 : v1);
		} else {
			timeStamp->addToBack(
					new Pair(pExt->getContent()->getNode(),
							pExt->getContent()->getValue()));
		}
		pExt = pExt->getNext();
	}

}

Pair* VectorClock::search(STNode *stnode) {

	Node<Pair> *p = timeStamp->start;

	while (p != NULL) {

		if (p->getContent() != NULL)
		{
			EV << p->getContent()->getNode();
			if( p->getContent()->getNode() == stnode)
			return p->getContent();
		}
		p = p->getNext();

	}

	return NULL;
}

