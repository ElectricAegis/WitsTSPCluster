/*
 * DataSender.h
 *
 *  Created on: 12 May 2012
 *      Author: Kailesh Ramjee
 *
 *      Libraries used:
 *      		Poco::Net
 *      		Poco::URI
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

#ifndef DATASENDER_H_
#define DATASENDER_H_

#include <iostream>
#include <fstream>
#include <string>
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"

using namespace std;
using namespace Poco::Net;

class DataSender {
public:
	/**
	 * A Constructor
	 * @param address the address of the web-server
	 */
	DataSender(string address);
	/**
	 * A Destructor
	 */
	virtual ~DataSender();
	/**
	 * Make a request as a client. This request will POST
	 * the results using a JSON structure.
	 */
	void makeRequest();
	/**
	 * Generates a string from a JOSN file
	 * @return The generated string
	 */
	string generateRequestPOSTString();

private:
	/**
	 * The address of the web-server
	 */
	string _address;
};

#endif /* DATASENDER_H_ */
