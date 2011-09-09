#ifndef EDGE_H
#define EDGE_H

#pragma once // Para resolver a referencia cruzada (Node inclui Edge, que inclui Node...)
#include "Includes.h"

using namespace std;

class Node; // Indica que ha' uma classe Node a ser referenciada aqui

class Edge{

	public:
		int weight;
		Node *from, *to;
		bool isDirected;
		bool flag;

		Edge(){
			from = to = NULL;
			weight = 1;
			isDirected = false;
		}

		Edge(Node *fromNode, Node *toNode, int edgeWeight, bool isDir){
			from = fromNode;
			to = toNode;
			(*fromNode).addEdge (this);
			(*toNode).addEdge (this);
			weight = edgeWeight;
			isDirected = isDir;
		}


};

#endif
