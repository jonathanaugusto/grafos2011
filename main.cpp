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

	adjm.build(argv[1]);
	cout << adjm.dijkstra(1,7,argv[1]).first << endl;
	vector <unsigned long> v = adjm.dijkstra(1,7,argv[1]).second;
	cout << v[0];
	for (unsigned int i = 1; i < v.size(); ++i) {
		cout << "-" << v[i];
	}
	//graph.dijkstra(1,100,argv[1]);
	//graph.convertToMatrix().dijkstra(1,argv[1]);

	cout << "All done! :D" << endl;

	system ("PAUSE");
    return 0;
}

