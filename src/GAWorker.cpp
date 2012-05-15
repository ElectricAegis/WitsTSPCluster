/*
 * GAWorker.cpp
 *
 *  Created on: May 1, 2012
 *      Author: manie
 */

#include "GAWorker.h"
#include <stdio.h>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/skeleton_and_content.hpp>
#include <limits.h>

namespace mpi = boost::mpi;

GAWorker::GAWorker(int numCities, int *DistMatrix) {
	_numCities = numCities;
	_DistMatrix = DistMatrix;
}

GAWorker::~GAWorker() {
	// TODO Auto-generated destructor stub
}

void GAWorker::start() {
	mpi::communicator world;
	int statusTag = CONT_TAG;

	while(statusTag == CONT_TAG) {
		std::vector<GAPath> population;
		world.recv(0 /* source */, CALC_DIST_TAG /* count */, mpi::skeleton(population));
		world.recv(0 /* source */, CALC_DIST_TAG /* count */, mpi::get_content(population));

//		printf("Population on worker %d: %d\n", world.rank(), population.size());
//		for( int i = 0 ; i<population.size() ; i++ )
//		{
//		 for( int j=0 ; j<_numCities ; j++ ) {
//		   int temp = population.at(i).path.at(j);
//			printf("%5d",  temp);
//		 }
//		 printf("\n");
//		}

		int shortestDistance = INT_MAX;
		int shortestDistanceIndex = 0;
		for( int i = 0 ; i<population.size() ; i++ ) {
			int tempDistance = population.at(i).calculatePathLength(_DistMatrix);
			if (tempDistance < shortestDistance) {
				shortestDistance = tempDistance;
				shortestDistanceIndex = i;
			}
		}

		world.send(0 /* target */, 1 /* count */, mpi::skeleton(population));
		world.send(0 /* target */, 1 /* count */, mpi::get_content(population));

		world.send(0 /* target */, 1 /* count */, mpi::skeleton(population.at(shortestDistanceIndex)));
		world.send(0 /* target */, 1 /* count */, mpi::get_content(population.at(shortestDistanceIndex)));

		world.recv(0, FINISH_TAG, mpi::skeleton(statusTag));
		world.recv(0, FINISH_TAG, mpi::get_content(statusTag));

		if (statusTag == FINISH_TAG) {
			printf("Finish tag received\n");
		}
	}
}
