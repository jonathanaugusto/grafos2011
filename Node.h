#ifndef NODE_H
#define NODE_H

#pragma once // Para resolver a referencia cruzada (Node inclui Edge, que inclui Node...)
class Edge; // Indica que ha' uma classe Node a ser referenciada aqui
#include "edge.h"
#include "includes.h"

using namespace std;

class Node{

	public:
		static int lastLabel;

		int label;
		vector<Edge*> edges;
		bitset<1> flag;

		Node(){
			if (!lastLabel) label = lastLabel = 1;
			lastLabel++;
		}

		Node(int nodeLabel){
			label = nodeLabel;
		}

		Node(int nodeLabel, vector<Edge*> connectedEdges){
			label = nodeLabel;
			edges = connectedEdges;
		}

		Node &operator=(Node node){
			label = node.label;
			edges = node.edges;
			return *this;
		}

		bool operator== (Node node){
			return label == node.label;
		}

		void addEdge(Edge *edge){
			edges.push_back(edge);
		}
};

int Node::lastLabel = 1;

#endif
