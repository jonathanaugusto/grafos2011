#include "Includes.h"

using namespace std;

int main(int argc, char *argv[])
{
	Graph grafo;

	if(argv[1]!= NULL){
		grafo.buildGraph(argv[1]);
		grafo.buildInformationFile(argv[1]);
	}


	system ("PAUSE");
    return 0;
}

