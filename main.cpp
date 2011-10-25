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
	graph.build(argv[1]);
	/*for (set <Edge *>::iterator it = graph.g_nodes[1].edges.begin(); it != graph.g_nodes[1].edges.end(); it++)
		cout << **it << endl;*/
	graph.prim(1,argv[1]);

	//graph.dijkstra(1,100,argv[1]);
	//graph.convertToMatrix().dijkstra(1,argv[1]);

	cout << "All done! :D" << endl;

	system ("PAUSE");
    return 0;
}

