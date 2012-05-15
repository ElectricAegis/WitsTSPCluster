/*
 * RequestData.cpp
 *
 *  Created on: 12 May 2012
 *      Author: Kailesh Ramjee
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

#include "RequestData.h"

RequestData::RequestData() {
	// TODO Auto-generated constructor stub

}

RequestData::~RequestData() {
	// TODO Auto-generated destructor stub
}
unsigned int RequestData::getNumberOfCities() const {
	return _numberOfCities;
}

void RequestData::setNumberOfCities(unsigned int numberOfCities) {
	_numberOfCities = numberOfCities;
}

vector<string> RequestData::getCityNames() const {
	return _cityNames;
}

void RequestData::setCityNames(vector<string> cityNames) {
	_cityNames = cityNames;
}

vector<vector<unsigned int> > RequestData::getDistanceMatrix() const {
	return _distanceMatrix;
}

void RequestData::setDistanceMatrix(vector<vector<unsigned int> > distanceMatrix) {
	_distanceMatrix = distanceMatrix;
}



