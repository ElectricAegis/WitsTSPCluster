/*
 * GAWorker.h
 *
 *  Created on: May 1, 2012
 *      Author: manie
 */

#ifndef GAWORKER_H_
#define GAWORKER_H_

#include "GAPath.h"
#include <vector>

class GAWorker {
public:
	GAWorker(int numCities, int *DistMatrix);
	virtual ~GAWorker();

	void start();

private:
	int _numCities;
	int *_DistMatrix;
	std::vector<GAPath> _population;
	int _populationSize;
};

#endif /* GAWORKER_H_ */
