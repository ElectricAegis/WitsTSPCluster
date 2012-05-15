/*
 * GACoordinator.h
 *
 *  Created on: Apr 28, 2012
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
