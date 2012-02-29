/*
 * VectorClock.h
 *
 *  Created on: Feb 23, 2012
 *      Author: dd
 */

using namespace std;
#include "STNode.h"
#include "Pair.h"
#include "LinkedList.h"

class STNode;

#ifndef VECTORCLOCK_H_
#define VECTORCLOCK_H_

class VectorClock {
public:
	LinkedList<Pair> *timeStamp;
	VectorClock();
	virtual ~VectorClock();
	void update(VectorClock *vc);
    LinkedList<Pair>* getTimeStamp();
    void setTimeStamp(LinkedList<Pair> *timeStamp);
    Pair* search(STNode *stnode);
};

#endif /* VECTORCLOCK_H_ */
