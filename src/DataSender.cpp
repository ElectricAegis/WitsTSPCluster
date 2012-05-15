/*
 * DataSender.cpp
 *
 *  Created on: 12 May 2012
 *      Author: Kailesh Ramjee
 *
 *      Libraries used:
 *				Poco::Net
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

#include "DataSender.h"

DataSender::DataSender(string address):
_address(address)
{
	// TODO Auto-generated constructor stub

}

DataSender::~DataSender() {
	// TODO Auto-generated destructor stub
}

void DataSender::makeRequest() {
	HTTPClientSession session(_address);
	//s.setProxy("localhost", srv.port());
	HTTPRequest request(HTTPRequest::HTTP_POST, "/TSPResults.json"); // This will have to change
	request.setContentType("application/json");
	request.setKeepAlive(true);

	session.sendRequest(request) << generateRequestPOSTString();

	HTTPResponse response;
	std::istream& rs = session.receiveResponse(response);

	// Output the HTTP response message and the content of the message
	response.write(cout) ;
	Poco::StreamCopier::copyStream(rs, cout);
}

string DataSender::generateRequestPOSTString() {
	ifstream myResultFile("TSPResults.json", ifstream::binary);
	string tempString, requestMessage;
	getline(myResultFile,tempString);
	while ( myResultFile ) {
		requestMessage += tempString + "\n";
		getline(myResultFile, tempString);
	}
	myResultFile.close();
	return requestMessage;
}

