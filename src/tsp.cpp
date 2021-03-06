//////////////////////////////////////////////////////////////////////////
// Traveling Salesman Problem with MPI
// Note: this is a C++ program.
// Author: Hermanus L. Brummer
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
// NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
// ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR
// OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// Process 0 manages the queue of incomplte paths, and keeps
// track of the best path.
//
// All other processes are workers that get and put jobs from and into 
// the queue. Each time they get a path, they are also informed
// about the best length so far.
//
// Note that, unlike previous examples, this one does not work with
// only one process.
//
// Starting city is assumed to be city 0.
//////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <memory.h>

#include "mpi.h"
#include "list.h"
#include "tsp.h"

#include "GACoordinator.h"
#include "GAWorker.h"
#include "GAPath.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/skeleton_and_content.hpp>
#include <boost/mpi/timer.hpp>
#include <vector>

#include "Requester.h"
#include "DataExtractor.h"
#include "RequestData.h"
#include "ResultGenerator.h"
#include "DataSender.h"
#include <iostream>
#include <json/json.h>

using namespace std;

namespace mpi = boost::mpi;

using namespace std;

int myrank, NumProcs, NumCities;

int *Dist;

///////////////////////////////////////////////////////////////////////////

Path::Path ()
{ 
	length=0;
	visited=1;
	for (int i=0; i<NumCities; i++) city[i]=i;
}

void Path::Set (int len, int *cit, int vis)
{
	length = len;
	memcpy (city, cit, NumCities*sizeof(int));
	visited = vis;
}

void Path::Print()
{
	for (int i=0; i<visited; i++)
		printf("  %d", city[i]);
	printf("; length = %d\n", length);
}

///////////////////////////////////////////////////////////////////////////


void Fill_Dist( void )
{

	if (myrank == 0)    // I'll read the matrix
		// actualy, you must redirect the input to a file
		// I'll not print any message, first it reads the number of cities
		// and then it reads the matrix
		scanf("%d", &NumCities);

	// global operation, all processes must call it
	MPI_Bcast( &NumCities, 1, MPI_INT, 0, MPI_COMM_WORLD);
	assert(NumCities<=MAXCITIES);

	Dist = new int[NumCities*NumCities];

	if (myrank == 0)
		for( int i = 0 ; i<NumCities ; i++ )
			for( int j = 0 ; j<NumCities ; j++ )
				scanf("%d", &Dist[i*NumCities + j]);

	// global operation, all processes must call it
	MPI_Bcast( Dist,                   // the buffer
			NumCities*NumCities,    // number of elements
			MPI_INT,                // type of elements
			0,                      // the root for the broadcast
			MPI_COMM_WORLD);        // the most used communicator

	if (myrank == 0)        // print the matrix for debugging
	{
		printf("Number of cities: %d\n", NumCities);
		for( int i = 0 ; i<NumCities ; i++ )
		{
			for( int j=0 ; j<NumCities ; j++ )
				printf("%5d", Dist[i*NumCities+j] );
			printf("\n");
		}
	}
}



void Coordinator ()
{
	MPI_Status status;
	Msg_t msg;

	// To keep track of processes that are waiting for a Path
	int* waiting = new int[NumProcs];     // to save ranks
	int nwait = 0;	                // how many are waiting
	int bpath = 0;                        // just count the number of best
	// path received

	Path Shortest;
	LIST queue;
	Path *P = new Path;
	queue.Insert(P, 0);	       // initialize queue with the first task
	// one zero-length path
	Shortest.length = INT_MAX;   // The initial Shortest path must be bad

	printf("Coord started ...\n"); fflush(stdout);
	while (nwait < NumProcs-1)
	{
		MPI_Recv (&msg, MSGSIZE, MPI_INT, MPI_ANY_SOURCE,
				MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		switch (status.MPI_TAG)
		{
		case BEST_PATH_TAG:
			if (msg.length < Shortest.length)
			{
				bpath ++;
				printf("Got best path %d, source = %d, length = %d\n",
						bpath, status.MPI_SOURCE, msg.length);
				fflush(stdout);

				// update Shortest:
				Shortest.Set (msg.length, msg.city, NumCities);
				for( int i = 1 ; i<NumProcs ; i++ )
					MPI_Send( &(Shortest.length), 1, MPI_INT, i,
							UPDATE_BEST_PATH_TAG, MPI_COMM_WORLD );
			}
			break;

		case PUT_PATH_TAG:
			if (nwait>0)
			{
				// Don't put path into queue; send it to one waiting process
				MPI_Send (&msg, MSGSIZE, MPI_INT, waiting[--nwait],
						REPLY_PATH_TAG, MPI_COMM_WORLD);
			}
			else
			{
				P = new Path();
				P->Set (msg.length, msg.city, msg.visited);
				queue.Insert(P, msg.length);
			}
			break;

		case GET_PATH_TAG:
			if (!queue.IsEmpty())
			{
				// get a path and send it along with bestlength
				P = (Path *)queue.Remove(NULL);
				msg.length = P->length;
				memcpy (msg.city, P->city, MAXCITIES*sizeof(int));
				msg.visited = P->visited;
				MPI_Send (&msg, MSGSIZE, MPI_INT, status.MPI_SOURCE,
						REPLY_PATH_TAG, MPI_COMM_WORLD);
				delete P;
			}
			else
			{
				// requester must wait
				waiting[nwait++] = status.MPI_SOURCE;
				if (nwait==NumProcs-1)
					// Tell everbody that we're done
					for (int i=1; i<NumProcs; i++)
						MPI_Send (NULL, 0, MPI_INT, i, DONE_TAG, MPI_COMM_WORLD);
			}
			break;
		}
	}
	printf("Shortest path:\n");
	Shortest.Print();
}

void Fill_Dist_Vect( vector<vector<unsigned int> > distanceMatrix )
{

	if (myrank == 0)    {
		NumCities = distanceMatrix.size();
	}

	// global operation, all processes must call it
	MPI_Bcast( &NumCities, 1, MPI_INT, 0, MPI_COMM_WORLD);
	assert(NumCities<=MAXCITIES);

	Dist = new int[NumCities*NumCities];

	if (myrank == 0)
		for( int i = 0 ; i<NumCities ; i++ )
			for( int j = 0 ; j<NumCities ; j++ )
				Dist[i*NumCities + j] = distanceMatrix.at(i).at(j);

	// global operation, all processes must call it
	MPI_Bcast( Dist,                   // the buffer
			NumCities*NumCities,    // number of elements
			MPI_INT,                // type of elements
			0,                      // the root for the broadcast
			MPI_COMM_WORLD);        // the most used communicator

	if (myrank == 0)        // print the matrix for debugging
	{
		printf("Number of cities: %d\n", NumCities);
		for( int i = 0 ; i<NumCities ; i++ )
		{
			for( int j=0 ; j<NumCities ; j++ )
				printf("%5d", Dist[i*NumCities+j] );
			printf("\n");
		}
	}
}

void Worker ()
{ 
	MPI_Status status;
	Msg_t msg;
	int shortestLength = INT_MAX;

	printf("Worker started ...\n"); fflush(stdout);

	MPI_Send (NULL, 0, MPI_INT, 0, GET_PATH_TAG, MPI_COMM_WORLD);

	while (1)
	{
		MPI_Recv (&msg, MSGSIZE, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		if (status.MPI_TAG==DONE_TAG)
		{
			printf("Worker %d received DONE_TAG ..\n", myrank);

			break; // we're done
		}

		if (status.MPI_TAG==UPDATE_BEST_PATH_TAG)
		{
			shortestLength = msg.length;    // we sent just one INT
			// length is the first field in msg
			continue;
		}


		// we must have a REPLY_PATH_TAG
		msg.visited++;
		if (msg.visited==NumCities)
		{
			int d1 = Dist[ (msg.city[NumCities-2])*NumCities + msg.city[NumCities-1] ];
			int d2 = Dist[(msg.city[NumCities-1]) * NumCities ];
			if (d1 * d2)    // both edges exist
			{
				msg.length += d1 + d2;

				// if path is good, send it to coordinator
				if (msg.length < shortestLength)
					MPI_Send (&msg, MSGSIZE, MPI_INT, 0, BEST_PATH_TAG, MPI_COMM_WORLD);
			}
			// not a valid path, ask for another partial path
		}
		else
		{
			// For each city not yet visited, extend the path:
			// (I'll use the same msg space to compute every extended the path)
			int length = msg.length;
			for (int i=msg.visited-1; i<NumCities; i++)
			{

				// swap city[i] and city[visted-1]
				if (i > msg.visited-1)
				{
					int tmp = msg.city[msg.visited-1];
					msg.city[msg.visited-1] = msg.city[i];
					msg.city[i] = tmp;
				}

				// visit city[visited-1]
				if (int d = Dist[(msg.city[msg.visited-2])*NumCities + msg.city[msg.visited-1] ])
				{
					msg.length = length + d;
					if (msg.length < shortestLength)
						MPI_Send (&msg, MSGSIZE, MPI_INT, 0, PUT_PATH_TAG, MPI_COMM_WORLD);
				}
			}
		}
		MPI_Send (NULL, 0, MPI_INT, 0, GET_PATH_TAG, MPI_COMM_WORLD);
	}
}


int main(int argc, char *argv[])
{
		mpi::environment env(argc, argv);
		mpi::communicator world;
		myrank = world.rank();
		NumProcs = world.size();
//		mpi::timer
		mpi::timer myTimer;
	//  MPI_Init (&argc, &argv);
	//  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	//  MPI_Comm_size(MPI_COMM_WORLD, &NumProcs);

	  if (NumProcs<2) {
	    printf("At least 2 processes are required\n");
	    exit(-1);
	  }

	  vector<vector<unsigned int> > distanceMatrix;

	  vector<string> cityNames;

	  if (myrank == 0) {
			//Requester
			string webServerAddress = "192.168.1.167";
			Requester myRequester(webServerAddress);
			myRequester.connectToWebServer();

			//Extractor
			DataExtractor myExtractor;
			myExtractor.parseDataFromFile();
			RequestData myTSPData = myExtractor.constructTSPData();
			// For the algorithm
			distanceMatrix = myTSPData.getDistanceMatrix();
			cityNames = myTSPData.getCityNames();
	  }
	  // Initialize distance matrix. Ususally done by one process
	  // and bcast, or initialized from a file in a shared file system.
//	  Fill_Dist_Vect(distanceMatrix);  // process 0 read the data and broadcast it to the others
	  Fill_Dist();

	//  if (myrank==0)
	//    Coordinator();
	//  else
	//    Worker();

	  if (myrank == 0) {

		  GACoordinator coordinator = GACoordinator(NumCities, Dist, 40000);
		  printf("coordinator starting \n");
		  coordinator.start(10);
		  printf("coordinator finishing\n");
		  printf("Time: %f\n", myTimer.elapsed());

//		  	//-------Algorithm-------
////		  	vector<unsigned int> bestRoute; // Starts at 0 being the first city in the distance matrix
//		  	GAPath resultPath = coordinator.getShortesPath();
//		  	vector<unsigned int> bestRoute = resultPath.path;
////		  	bestRoute.push_back(0);
////		  	bestRoute.push_back(2);
////		  	bestRoute.push_back(1);
////		  	bestRoute.push_back(3);
//		  	unsigned int optimalPathLength = resultPath.length;
//		  	//-----------------------
//
//		  	//Generator result
//		  	vector<unsigned int> route;
//		  	ResultGenerator myGenerator(optimalPathLength, cityNames);
//		  	myGenerator.setRoute(bestRoute);
//		  //	myGenerator.constructJSONSwebServerAddresstream();
//		  	myGenerator.constructJSONStream();
//		  	myGenerator.createJSONFile();
//
//		  	//Send the result to the Web-server
//		  	DataSender mySender("192.168.1.167");
//		  	mySender.makeRequest();

	  } else {
		  GAWorker worker = GAWorker(NumCities, Dist);
		  printf("worker %d starting\n",myrank);
		  worker.start();
		  printf("worker %d finishing\n",myrank);
	  }


	return 0;

	//  printf("\n");
	//  return 0;
}

