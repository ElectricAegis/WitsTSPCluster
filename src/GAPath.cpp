/*
 * GAPath.cpp
 *
 *  Created on: Apr 30, 2012
 *      Author: manie
 */

#include "GAPath.h"

#include <cstdlib>	// for srand and rand
#include <ctime>	// for time
#include <memory.h>

void randomizeArray(int arraySize, std::vector<int> &array);

bool GAPath::isSeeded = false;

GAPath::GAPath(int numberCities) {
	//TODO: place somewhere else
	if (!isSeeded) {
		srand(time(0));
		isSeeded = true;
	}
	length=0;
	numCities = numberCities;
	// Initialize random path
	randomizeArray(numCities,path);
}

GAPath::~GAPath() {
	// TODO Auto-generated destructor stub
}

void randomizeArray(int arraySize, std::vector<int> &array)	{
		array.clear();
		for (int i = 0; i < arraySize; i++) array.push_back(i);

		//-- Shuffle elements by randomly exchanging each with one other
		for (int i = 1; i < (arraySize-1); i++) {
			int r = i + (rand() % (arraySize-i));	// random remaining position
			int temp = array.at(i);
			array.at(i) = array.at(r);
			array.at(r) = temp;
		}
}
