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

	graph.build(argv[1]);
	cout << graph.dijkstra(1,10,argv[1]).first << endl;
	cout << graph.dijkstra(1,10,argv[1]).second << endl;
	//graph.dijkstra(1,100,argv[1]);
	//graph.convertToMatrix().dijkstra(1,argv[1]);

	cout << "All done! :D" << endl;

	system ("PAUSE");
    return 0;
}

