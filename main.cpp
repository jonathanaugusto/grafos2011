/**
 * Universidade Federal do Rio de Janeiro
 * COS242 - Teoria dos Grafos
 * @descr	Trabalho pratico da disciplina - Parte 1
 * @author	Bruno Tomas / Jonathan Augusto
 */

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
	AdjacencyMatrix adjm;
	AdjacencyList adjl;

	grafo.buildGraph(argv[1]);
	grafo.buildInformationFile(argv[1]);

	adjm = grafo.buildAdjMatrix ();
	adjl = grafo.buildAdjList ();

	grafo.bfs(adjm,123,argv[1]);
	grafo.bfs(adjl,123,argv[1]);

	grafo.bfs(adjm,489,argv[1]);
	grafo.bfs(adjl,489,argv[1]);

/*	grafo.dfs(adjm,10,argv[1]);
	grafo.dfs(adjl,10,argv[1]);
	grafo.foundConnectedComponents(adjl,argv[1]);*/

	cout << "All done! :D" << endl;

	system ("PAUSE");
    return 0;
}

