/*
 * GACoordinator.cpp
 *
 *  Created on: Apr 28, 2012
 *      Author: Hermanus Brummer
 */

#include "GACoordinator.h"

#include <stdio.h>

//#include "tsp.h"

GACoordinator::GACoordinator(int numCities, int *DistMatrix, int populationSize) {
	// TODO Auto-generated constructor stub
	_numCities = numCities;
	_DistMatrix = DistMatrix;
	_populationSize = populationSize;
}

GACoordinator::~GACoordinator() {
	// TODO Auto-generated destructor stub
}

std::vector<GAPath> GACoordinator::getPopulation() {
	return _population;
}

void GACoordinator::start() {
	//-- Generate Population
	_population.clear();
	for (int j = 0; j < _populationSize; ++j) {
		GAPath tempPath = GAPath(_numCities);
		_population.push_back(tempPath);
	}

    printf("Population: %d\n", _numCities);
    for( int i = 0 ; i<_populationSize ; i++ )
    {
       for( int j=0 ; j<_numCities ; j++ ) {
    	   int temp = _population.at(i).path.at(j);
          printf("%5d",  temp);
       }
       printf("\n");
    }
}
