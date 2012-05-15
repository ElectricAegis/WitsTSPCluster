/*
 * GAPath.cpp
 *
 *  Created on: Apr 30, 2012
 *      Author: Hermanus Brummer
 *
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *      OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
 *      NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
 *      ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR
 *      OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, ARISING
 *      FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *      OTHER DEALINGS IN THE SOFTWARE.
 */

#include "GAPath.h"

#include <cstdlib>	// for srand and rand
#include <ctime>	// for time
#include <memory.h>
#include <numeric>
#include <limits.h>
#include <algorithm> // for random_shuffle

bool GAPath::isSeeded = false;

GAPath::GAPath(int numberCities) {
	//TODO: place somewhere else
	if (!isSeeded) {
		srand(time(0));
		isSeeded = true;
	}
	length=INT_MAX;
	numCities = numberCities;
	// Initialize random path
	path.clear();
	path.push_back(0);
	std::vector<int> tempPath;
	for (int i = 1; i < numCities; i++) tempPath.push_back(i);
	std::random_shuffle(tempPath.begin(),tempPath.end());
	path.insert(path.end(), tempPath.begin(), tempPath.end());
}

GAPath::~GAPath() {
	// TODO Auto-generated destructor stub
}

int GAPath::calculatePathLength(int *distanceMatrix){
	int d = 0;
	for (int i = 0; i < path.size()-1; ++i) {
		//row then column
		d += distanceMatrix[path.at(i)*numCities + path.at(i+1)];
	}
	//length =  std::accumulate(path.begin(), path.end(),0);
	length = d;
	return length;
}

void GAPath::insertionPoints(int &first, int &last) {
	// make sure that first element is not selected
	first = 1 + (rand() % (path.size()-1));
	last = 1 + (rand() % (path.size()-1));
	// ensure insertion points are not equal
	while (first == last) {
		first = 1 + (rand() % (path.size()-1));
		last = 1 + (rand() % (path.size()-1));
	}
	if (last < first) std::swap(first,last);
}

void GAPath::swapPoints(int first, int last){
	int temp = path.at(first);
	path.at(first) = path.at(last);
	path.at(last) = temp;
}

void GAPath::flipPoints(int first, int last) {
	while (first < last) {
		swapPoints(first, last);
		first++;
		last--;
	}
}

void GAPath::slidePoints(int first, int last) {
	int temp = path.at(first);

	path.erase(path.begin()+first);
	path.insert(path.begin()+last,temp);
}

//void randomizeArray(std::vector<> &array)	{
//		//-- Shuffle elements by randomly exchanging each with one other
//		for (int i = 1; i < (array.size()-1); i++) {
//			int r = i + (rand() % (array.size()-i));	// random remaining position
//			int temp = array.at(i);
//			array.at(i) = array.at(r);
//			array.at(r) = temp;
//		}
//}
