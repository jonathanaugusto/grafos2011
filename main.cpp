#include "Includes.h"

using namespace std;

int main(int argc, char *argv[])
{
	Node node1 (1);
	Node node2 (2);
	Node node3 (3);
	Node node4 (4);

	Edge edge1 (node1, node2, 1, false);
	Edge edge2 (node1, node4, 1, false);
	Edge edge3 (node2, node3, 1, false);

	cout << "Ate' agora, tudo certo! :)" << endl;
	system ("PAUSE");
    return 0;
}

