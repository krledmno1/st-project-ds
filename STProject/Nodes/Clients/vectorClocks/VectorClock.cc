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
}

VectorClock::~VectorClock() {
	delete timeStamp;
}

LinkedList<Pair>* VectorClock::getTimeStamp()  {
	return timeStamp;
}

void VectorClock::setTimeStamp(LinkedList<Pair> *timeStamp) {

	for(Node<Pair> *p = timeStamp->start; p != NULL; p= p->getNext()) {
		Pair *pair = new Pair(p->getContent()->getNode(), p->getContent()->getValue());
		this->timeStamp->addToBack(pair);
	}
}

void VectorClock::update(VectorClock *vc) {
	//EV << "Merging: my dimension " << timeStamp->size << " his dimension " << vc->timeStamp->size << "\n";
	LinkedList<Pair>* tsExt = vc->getTimeStamp();

	Node<Pair> (*externalPairIterator) = tsExt->start;

	/*DEBUG*/if (externalPairIterator == NULL) { EV << "VectorClock: externalPairIterator is NULL!\n"; return; }

	while (externalPairIterator != NULL) {
		/*DEBUG */if (externalPairIterator->getContent() == NULL || externalPairIterator->getContent()->getNode()==NULL) { EV << "VectorClock: externalPairIterator got NULL Content\n"; externalPairIterator = externalPairIterator->getNext(); continue; }
		Pair *internalPairMatch = search(externalPairIterator->getContent()->getNode());

		if (internalPairMatch != NULL) {
			int v1 = externalPairIterator->getContent()->getValue();
			int v2 = internalPairMatch->getValue();
			internalPairMatch->setValue(v1 < v2 ? v2 : v1); //the max
		} else {
			timeStamp->addToBack(new Pair(externalPairIterator->getContent()->getNode(), externalPairIterator->getContent()->getValue()));
		}
		externalPairIterator = externalPairIterator->getNext();
	}
	//EV << "Ended merging, my dimension: " << timeStamp->size << "\n";
}

Pair* VectorClock::search(STNode *stnode) {
	Node<Pair> *p = timeStamp->start;
	while (p != NULL) {
		if (p->getContent() != NULL)
		{
			//EV << p->getContent()->getNode();
			if( p->getContent()->getNode() == stnode)
			return p->getContent();
		}
		p = p->getNext();

	}

	return NULL;
}

void VectorClock::printClock() {
	Node<Pair> * pairIterator = timeStamp->start;
	EV << "Vector Clock: ";
	while(pairIterator!=NULL){
		EV << "<" << pairIterator->getContent()->getNode() << " -> "<< pairIterator->getContent()->getValue() << "> ";
		pairIterator = pairIterator->getNext();
	}
	EV << "\n";
}



