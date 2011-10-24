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

	graph.build(argv[1]);
	adjm.build(argv[1]);

	graph.dijkstra(1,7,argv[1]);
	adjm.dijkstra(1,7,argv[1]);

	cout << "All done! :D" << endl;

	system ("PAUSE");
    return 0;
}

