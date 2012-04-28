/*
 * GACoordinator.h
 *
 *  Created on: Apr 28, 2012
 *      Author: Hermanus Brummer
 */

#ifndef GACOORDINATOR_H_
#define GACOORDINATOR_H_

class GACoordinator {
public:
	GACoordinator(int numCities, int *DistMatrix);
	virtual ~GACoordinator();
	void start();

private:
	int _numCities;
	int *_DistMatrix;
};

#endif /* GACOORDINATOR_H_ */
