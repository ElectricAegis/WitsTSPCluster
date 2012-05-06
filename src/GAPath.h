/*
 * GAPath.h
 *
 *  Created on: Apr 30, 2012
 *      Author: manie
 */

#ifndef GAPATH_H_
#define GAPATH_H_

//#include <vector>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
//const unsigned int MAXCITIES = 25;

enum {FINISH_TAG, CONT_TAG, CALC_DIST_TAG, MUTATE_TAG};

class GAPath {
public:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version )
	{
		ar & length;
		ar & path;
		ar & numCities;
		ar & isSeeded;
	}
	//public attributes
	int length;			// length of path
	std::vector<int> path;
	int numCities;
	static bool isSeeded;

	//public methods
	GAPath(int numberCities = 0);
	virtual ~GAPath();
	int calculatePathLength(int *distanceMatrix);
	void insertionPoints(int &first, int &last);
	void swapPoints(int first, int last);
	void flipPoints(int first, int last);
	void slidePoints(int first, int last);

//private:
};

//void randomizeArray(std::vector<int> &array);

#endif /* GAPATH_H_ */
