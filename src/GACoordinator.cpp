/*
 * GACoordinator.cpp
 *
 *  Created on: Apr 28, 2012
 *      Author: Hermanus Brummer
 */

#include "GACoordinator.h"
#include "GAPath.h"

#include <stdio.h>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/skeleton_and_content.hpp>
#include <boost/mpi/timer.hpp>
#include <algorithm> // for random_shuffle

namespace mpi = boost::mpi;
//#include "tsp.h"

GACoordinator::GACoordinator(int numCities, int *DistMatrix, int populationSize) {
	_numCities = numCities;
	_DistMatrix = DistMatrix;
	// ensure population is devisable by 4, due to for mutation strategies;
	if ((populationSize % 4) != 0) populationSize = 4*(populationSize/4 + 1);
	_populationSize = populationSize;
	_shortestPath = GAPath();
}

GACoordinator::~GACoordinator() {
	// TODO Auto-generated destructor stub
}

std::vector<GAPath> GACoordinator::getPopulation() {
	return _population;
}

void GACoordinator::start(int iterations) {
	//-- Generate Initial Population
	_population.clear();
	for (int j = 0; j < _populationSize; ++j) {
		GAPath tempPath = GAPath(_numCities);
		_population.push_back(tempPath);
	}

//    printf("Population: %d\n", _population.size());
//    for( int i = 0 ; i<_populationSize ; i++ )
//    {
//       for( int j=0 ; j<_numCities ; j++ ) {
//    	   int temp = _population.at(i).path.at(j);
//          printf("%5d",  temp);
//       }
//       printf("\n");
//    }

    mpi::communicator world;
    int numProcs = world.size();
    int Split = _populationSize/(numProcs-1);

    printf("Number of Processors %d\n", numProcs);
    printf("Work Split %d\n", Split);

    for (int loop = 0; loop < iterations; ++loop) {
    	printf("ITERATION %d\n", loop);

    	printf("Calculate Distance\n");
    	printf("Sending\n");
    	mpi::timer myTimer;
		//send paths for distance calculation
		for (int i = 1; i < numProcs; ++i) {
			std::vector<GAPath>::const_iterator first = _population.begin() + (i-1)*Split;
			std::vector<GAPath>::const_iterator last = _population.begin() + i*Split;
			std::vector<GAPath> tempVec(first, last);
			printf("Before Send Time: %f\n", myTimer.elapsed());
			world.send(i /* target */, CALC_DIST_TAG /* count */, mpi::skeleton(tempVec));
			world.send(i /* target */, CALC_DIST_TAG /* count */, mpi::get_content(tempVec));
			printf("After Send Time: %f\n", myTimer.elapsed());
		}

		//receive paths from distance calculation determine shortest path
		printf("Receiving\n");
		_population.clear();
		for (int i = 1; i < numProcs; ++i) {
			std::vector<GAPath> population;
			world.recv(i /* source */, 1 /* count */, mpi::skeleton(population));
			world.recv(i /* source */, 1 /* count */, mpi::get_content(population));

			_population.insert(_population.end(), population.begin(), population.end());

			GAPath tempShortestPath = GAPath(_numCities);
			printf("Before  Recieve Time: %f\n", myTimer.elapsed());
			world.recv(i /* source */, 1 /* count */, mpi::skeleton(tempShortestPath));
			world.recv(i /* source */, 1 /* count */, mpi::get_content(tempShortestPath));
			printf("After Receive Time: %f\n", myTimer.elapsed());
			if (tempShortestPath.length < _shortestPath.length){
				_shortestPath = tempShortestPath;
			}
		}

//		for( int i = 0 ; i<_population.size() ; i++ ) {
////				printf("length of path: %5d\n", _population.at(i).length);
//			}

//		printf("the shortest Path is: %5d\n", _shortestPath.length);
		printf("Shuffle\n");
		//randomize order of paths for fair comparison
		std::random_shuffle(_population.begin(), _population.end());

//		printf("Population randomized: %d\n", _population.size());
//			for( int i = 0 ; i<_populationSize ; i++ )
//			{
//			   for( int j=0 ; j<_numCities ; j++ ) {
//				   int temp = _population.at(i).path.at(j);
//				  printf("%5d",  temp);
//			   }
//			   printf("\n");
//			}

		//mutate current paths
		printf("Mutate\n");
		for (int i = 0; i < _population.size(); i += 4) {
			std::vector<GAPath>::const_iterator first = _population.begin() + i;
			std::vector<GAPath>::const_iterator last = _population.begin() + i + 4;
			std::vector<GAPath> samplePopulation(first, last);
			int shortestDistance = INT_MAX;
			int shortestDistanceIndex = 0;
			for (int j = 0; j < samplePopulation.size(); ++j) {
				if (samplePopulation.at(j).length < shortestDistance){
					shortestDistance = samplePopulation.at(j).length;
					shortestDistanceIndex = j;
				}
			}

			_population.at(i) = samplePopulation.at(shortestDistanceIndex);
			_population.at(i+1) = samplePopulation.at(shortestDistanceIndex);
			_population.at(i+2) = samplePopulation.at(shortestDistanceIndex);
			_population.at(i+3) = samplePopulation.at(shortestDistanceIndex);
			int insertFirst = 0;
			int insertLast = 0;
			samplePopulation.at(0).insertionPoints(insertFirst,insertLast);
			_population.at(i+1).swapPoints(insertFirst,insertLast);
			_population.at(i+2).flipPoints(insertFirst, insertLast);
			_population.at(i+3).slidePoints(insertFirst,insertLast);
//			printf("insertion points: first = %d last = %d\n", insertFirst, insertLast);
		}

//		printf("Population after mutation: %d\n", _population.size());
//				for( int i = 0 ; i<_populationSize ; i++ )
//				{
//				   for( int j=0 ; j<_numCities ; j++ ) {
//					   int temp = _population.at(i).path.at(j);
//					  printf("%5d",  temp);
//				   }
//				   printf("\n");
//				}

		//instruct slave processors to continue or terminate
		for (int i = 1; i < numProcs; ++i) {
			int tag = CONT_TAG;
			if (loop == iterations-1) tag = FINISH_TAG;
			world.send(i, FINISH_TAG, mpi::skeleton(tag));
			world.send(i, FINISH_TAG, mpi::get_content(tag));
		}
    }
    printf("Shortest Path: ");
    for (int i = 0; i < _shortestPath.path.size(); ++i) {
    	printf("%d ",_shortestPath.path.at(i));
	}
    printf("\n Shortest Path length: %d\n", _shortestPath.length);
}
