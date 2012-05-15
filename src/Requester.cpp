/*
 * Requester.cpp
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

#include "Requester.h"

Requester::Requester(string address):
_address(address)
{
	// TODO Auto-generated constructor stub

}

Requester::~Requester() {
	// TODO Auto-generated destructor stub
}

void Requester::connectToWebServer() {
	HTTPClientSession session(_address);
	//s.setProxy("localhost", srv.port());
	HTTPRequest request(HTTPRequest::HTTP_GET, "/TSPData.json");
	session.sendRequest(request);

	HTTPResponse response;
	istream& receivedStream = session.receiveResponse(response);
	createJSONFile(receivedStream);
}

void Requester::createJSONFile(istream& rs) {
	ofstream myFile("TSPData.json");
	if (myFile.is_open())
	{
		Poco::StreamCopier::copyStream(rs, myFile);
		myFile.close();
	}
}
