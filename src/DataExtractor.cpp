/*
 * DataExtractor.cpp
 *
 *  Created on: 12 May 2012
 *      Author: Kailesh Ramjee
 *
 *      Libraries used:
 *			Jsoncpp
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

#include "DataExtractor.h"

DataExtractor::DataExtractor() {
	// TODO Auto-generated constructor stub

}

DataExtractor::~DataExtractor() {
	// TODO Auto-generated destructor stub
}

void DataExtractor::parseDataFromFile() {
	Json::Reader myReader;
	ifstream myInputFile("TSPData.json", ifstream::binary);
	bool parsingSucceeded = myReader.parse(myInputFile, _rootValue);
	if (!parsingSucceeded)
	{
		// Notify the user that parsing has failed and display an error message
		cout << "The parsing of configuration has failed:\n"
				<< myReader.getFormattedErrorMessages();
	}
}

RequestData DataExtractor::constructTSPData() {
	RequestData myData;
	myData.setNumberOfCities(_rootValue.get("noOfCities", "0" ).asUInt());

	vector<vector<unsigned int> > distanceMatrix;
	vector<unsigned int> row;

	const Json::Value distMatrixRoot = _rootValue["distanceMatrix"];
	for (unsigned int i = 0; i < distMatrixRoot.size(); ++i )  // Iterates over the sequence elements.
	{
		for(unsigned int j = 0; j < distMatrixRoot[i].size(); ++j)
		{
			row.push_back(distMatrixRoot[i][j].asUInt());
			//cout << "i = " << i << " j = " << j << " value = " << distMatrixRoot[i][j].asUInt() << endl;
		}
		distanceMatrix.push_back(row);
		row.clear();
	}

	myData.setDistanceMatrix(distanceMatrix);

	vector<string> cityNames;
	const Json::Value cityNamesRoot = _rootValue["names"];
	for (unsigned int i = 0; i < cityNamesRoot.size(); ++i )
	{
		cityNames.push_back(cityNamesRoot[i].asString());
		//cout << "i = " << i << " " << cityNamesRoot[i].asString() << endl;
	}
	myData.setCityNames(cityNames);
	return myData;
}
