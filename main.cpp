/**
 * Universidade Federal do Rio de Janeiro
 * COS242 - Teoria dos Grafos
 * @descr	Trabalho pratico da disciplina
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
	Graph graph;
	AdjacencyMatrix adjm;
	AdjacencyList adjl;

	graph.buildGraph(argv[1]);
	graph.bfs(1,argv[1]);
	graph.nonWeightedPath(1,41,argv[1]);

	cout << "All done! :D" << endl;

	system ("PAUSE");
    return 0;
}

