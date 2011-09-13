#include "Includes.h"

using namespace std;

int main(int argc, char *argv[])
{
	Node node1 (1);

	Node node2 (2);
	Node node3 (3);
	Node node4 (4);

	Edge edge1 (node1, node2, 1, false);
	cout << "Passo 1: " << node1 << endl;

	node1.addEdge(&edge1);
	cout << "Passo 2: " << node1 << endl;

	node1.addEdge(&edge1);
	cout << "Passo 3: " << node1 << endl;

	Edge edge2 (node1, node4, 1, false);
	Edge edge3 (node2, node3, 1, false);

	cout << node1.edges.size() << " arestas ligadas a node1:" << endl;
	for (unsigned int i = 0; i < node1.edges.size(); i++)
		cout << *(node1.edges[i]) << " ";

	cout << "Passo 4: " << node1 << endl;

	system ("PAUSE");
    return 0;
}

