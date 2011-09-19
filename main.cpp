#include "Includes.h"

using namespace std;

int main(int argc, char *argv[])
{
	if(argv[1] == NULL){
		cout << "Usage: " << argv[0] << " <filename>" << endl;
		system ("PAUSE");
		return 0;
	}

	Graph grafo;
	Adjacency **adjMatrix;

	grafo.buildGraph(argv[1]);
	grafo.buildInformationFile(argv[1]);

	grafo.buildAdjMatrix (adjMatrix);
	cout << grafo.testAdjacency(2,4) << endl;


	system ("PAUSE");
    return 0;
}

