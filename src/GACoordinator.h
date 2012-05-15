/*
 * GACoordinator.h
 *
 *  Created on: Apr 28, 2012
 *      Author: Hermanus Brummer
 */

#ifndef GACOORDINATOR_H_
#define GACOORDINATOR_H_

#include "GAPath.h"

#include <vector>

class GACoordinator {
public:
	GACoordinator(int numCities, int *DistMatrix, int populationSize);
	virtual ~GACoordinator();

	void start(int iterations = 2);
	std::vector<GAPath> getPopulation();
	GAPath getShortesPath() { return _shortestPath; }

private:
	int _numCities;
	int *_DistMatrix;
	std::vector<GAPath> _population;
	int _populationSize;
	GAPath _shortestPath;
};

#endif /* GACOORDINATOR_H_ */
