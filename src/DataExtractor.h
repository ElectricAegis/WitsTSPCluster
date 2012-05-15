/*
 * DataExtractor.h
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
#ifndef DATAEXTRACTOR_H_
#define DATAEXTRACTOR_H_

#include "RequestData.h"
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;


class DataExtractor {
public:
	/**
	 * A Constructor
	 */
	DataExtractor();
	/**
	 * A Destructor
	 */
	virtual ~DataExtractor();
	/**
	 * Receives the data from a file and parses the data
	 */
	void parseDataFromFile();
	/**
	 * Receives the data from a file and parses the data
	 */
	RequestData constructTSPData();

private:
	/**
	 * Json::Value that forms the root of the file
	 */
	Json::Value _rootValue;
};

#endif /* DATAEXTRACTOR_H_ */
