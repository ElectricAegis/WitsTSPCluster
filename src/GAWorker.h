/*
 * GAWorker.h
 *
 *  Created on: May 1, 2012
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
