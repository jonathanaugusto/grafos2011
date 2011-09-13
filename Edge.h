#ifndef EDGE_H
#define EDGE_H

#pragma once // Para resolver a referencia cruzada (Node inclui Edge, que inclui Node...)
class Node; // Indica que ha' uma classe Node a ser referenciada aqui
#include "node.h"
#include "includes.h"

using namespace std;

class Edge{

	public:
		int weight;
		Node *from, *to;
		bool isDirected;
		bitset<1> flag;

		Edge(){
			from = to = NULL;
			weight = 1;
			isDirected = false;
		}

		Edge(Node fromNode, Node toNode, int edgeWeight, bool isDir){
			from = &fromNode;
			to = &toNode;
			fromNode.addEdge (this);
			toNode.addEdge (this);
			weight = edgeWeight;
			isDirected = isDir;
		}


};

#endif
