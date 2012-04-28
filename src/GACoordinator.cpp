/*
 * GACoordinator.cpp
 *
 *  Created on: Apr 28, 2012
 *      Author: Hermanus Brummer
 */

#include "GACoordinator.h"

#include <cstdlib>	// for srand and rand
#include <ctime>	// for time
#include <memory.h>
#include <stdio.h>

//#include "tsp.h"

GACoordinator::GACoordinator(int numCities, int *DistMatrix) {
	// TODO Auto-generated constructor stub
	_numCities = numCities;
	_DistMatrix = DistMatrix;
}

GACoordinator::~GACoordinator() {
	// TODO Auto-generated destructor stub
}

void GACoordinator::start() {
	//-- Generate Population
	int popSize = 50;
	int population[popSize][_numCities];
	srand(time(0));

	int tempPath[_numCities];

	for (int i = 0; i < _numCities; ++i) {
			tempPath[i] = i;
	}

	for (int j = 0; j < popSize; ++j) {
		//-- Shuffle elements by randomly exchanging each with one other

		for (int i = 0; i < (_numCities-1); i++) {
			int r = i + (rand() % (_numCities-i));	// random remaining position
			int temp = tempPath[i];
			tempPath[i] = tempPath[r];
			tempPath[r] = temp;
		}

		 memcpy (population[j], tempPath, _numCities*sizeof(int));
	}

    printf("Population: %d\n", _numCities);
    for( int i = 0 ; i<popSize ; i++ )
    {
       for( int j=0 ; j<_numCities ; j++ )
          printf("%5d",  population[i][j]);
       printf("\n");
    }
}
