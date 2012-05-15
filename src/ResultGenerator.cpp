/*
 * ResultGenerator.cpp
 *
 *  Created on: 12 May 2012
 *      Author: Kailesh Ramjee
 *
 *      Libraries used:
 *      		Jsoncpp
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

#include "ResultGenerator.h"

ResultGenerator::ResultGenerator(unsigned int bestPathLength, vector<string> names):
_bestPathLength(bestPathLength),
_route(names)
{
	Json::StyledWriter writer;
	string outputConfig = writer.write( _responseRoot );

}

ResultGenerator::~ResultGenerator() {
	// TODO Auto-generated destructor stub
}

void ResultGenerator::setRoute(vector<unsigned int> route) {

	vector<string> tempRoute(_route);
	for(unsigned int i = 0; i < route.size();  ++i)
	{
		_route[route[i]] = tempRoute[i];
	}
}

void ResultGenerator::constructJSONStream() {

	// The values are placed in alphabetical order in the JSON stream
	// Therefore navigationPath appears before optimalPath
	_responseRoot["optimalPath"] = _bestPathLength;

	Json::Value pathSequenceRoot(Json::arrayValue);
	for(unsigned int i = 0; i < _route.size(); ++i)
	{
		pathSequenceRoot.append(Json::Value(_route[i]));
	}
	_responseRoot["navigationPath"] = pathSequenceRoot;
}

void ResultGenerator::createJSONFile() {
	ofstream jsonFile("TSPResults.json");
	jsonFile << _responseRoot;
	jsonFile.close();
}
