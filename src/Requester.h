/*
 * Requester.h
 *
 *  Created on: 12 May 2012
 *      Author: Kailesh Ramjee
 *
 *      Libraries used:
 *      		Poco::Net
 *      		Poco::StreamCopier
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

#ifndef REQUESTER_H_
#define REQUESTER_H_

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include <fstream>
#include <string>
using namespace std;
using namespace Poco::Net;

class Requester {
public:
	/**
	 * A Constructor
	 */
	Requester(string address);
	/**
	 * A Destructor
	 */
	virtual ~Requester();
	/**
	 *  Creates connection to web-server and receives
	 *  TSP data in the from of an output stream
	 */
	void connectToWebServer();
	/**
	 *  Dumps the data in a JSON file
	 *  @param JSONFileName The name of the file to store the TSP data
	 */
	void createJSONFile(istream& rs);

private:
	/**
	 * The name of address of the web-server
	 */
	string _address;
};

#endif /* REQUESTER_H_ */
