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
	system ("cls");
	cout << endl;

	char structure;
	do{
		cout << "Choose a data structure for the graph:\n[O]bject-oriented\nAdjacency-[M]atrix\nAdjacency-[L]ist\nData structure: ";
		cin >> structure;
		structure = toupper(structure);
		if ( (structure != 'O') && (structure != 'M') && (structure != 'L') )
			cout << "Invalid\n";
	}while ( (structure != 'O') && (structure != 'M') && (structure != 'L') );
	cout << endl;

	char operation;
	do{
		cout << "Choose an operation:\n[D]istance and minimum path\n[M]ST\n[I]nformation file w/ mean distance and empirical distribution\nOperation: ";
		cin >> operation;
		operation = toupper(operation);
		if ( (operation != 'D') && (operation != 'M') && (operation != 'I') )
			cout << "Invalid\n";
	}while ( (operation != 'D') && (operation != 'M') && (operation != 'I') );
	cout << endl;

	unsigned long startingNode;
	if ((operation == 'D') || (operation == 'M')){
		cout << "Select a starting node: ";
		cin >> startingNode;
	}
	cout << endl;

	unsigned long endingNode;
	bool hasEnding = false;
	if (operation == 'D'){
		cout << "Select an ending node or type Enter: ";
		cin >> endingNode;
		if (endingNode != NULL) hasEnding = true;
	}
	cout << endl;

	Graph g;
	AdjacencyMatrix m;
	AdjacencyList l;

	switch (structure) {
	case 'O':
		g.build(argv[1]);
		cout << endl;
		switch (operation){
		case 'D':
			if (hasEnding == true){
				pair<float, vector<Node *> > path = g.path(startingNode,endingNode);
				cout << "Distance: " << path.first << endl << "Path: " << path.second[0]->label;
				for (unsigned int i = 1; i < path.second.size(); i++) cout << "-" << path.second[i]->label;
				cout << endl;
			}
			else g.path(startingNode,argv[1]);
			break;
		case 'M':
			g.prim(startingNode,argv[1]);
			break;
		case 'I':
			g.buildInformationFile2(argv[1]);
			break;
		default:
			break;
		}
		break;
	case 'M':
		m.build(argv[1]);
		cout << endl;
		switch (operation){
		case 'D':
			if (hasEnding == true){
				pair<float, vector<unsigned long> > path = m.path(startingNode,endingNode);
				cout << "Distance: " << path.first << endl << "Path: " << path.second[0];
				for (unsigned int i = 1; i < path.second.size(); i++) cout << "-" << path.second[i];
				cout << endl;
			}
			else l.path(startingNode,argv[1]);
			break;
		case 'M':
			m.prim(startingNode,argv[1]);
			break;
		case 'I':
			m.buildInformationFile2(argv[1]);
			break;
		default:
			break;
		}
		break;
	case 'L':
		l.build(argv[1]);
		cout << endl;
		switch (operation){
		case 'D':
			if (hasEnding == true){
				pair<float, vector<unsigned long> > path = l.path(startingNode,endingNode);
				cout << "Distance: " << path.first << endl << "Path: " << path.second[0];
				for (unsigned int i = 1; i < path.second.size(); i++) cout << "-" << path.second[i];
				cout << endl;
			}
			else l.path(startingNode,argv[1]);
			break;
		case 'M':
			l.prim(startingNode,argv[1]);
			break;
		case 'I':
			l.buildInformationFile2(argv[1]);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	cout << endl;
	cout << "All done! :D" << endl;

	system ("pause");
	//system ("cls");
	cout << endl;

    return 0;
}

