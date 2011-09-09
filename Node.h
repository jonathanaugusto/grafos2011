#ifndef NODE_H
#define NODE_H

#pragma once // Para resolver a referencia cruzada (Node inclui Edge, que inclui Node...)
#include "Includes.h"

using namespace std;

class Edge; // Indica que ha' uma classe Edge a ser referenciada aqui

class Node{

	public:
		static int lastLabel;

		int label;
		vector<Edge> *edges;

		Node(){
			if (!lastLabel) label = lastLabel = 1;
			lastLabel++;
		}

		Node(int nodeLabel){
			label = nodeLabel;
		}

		Node(int nodeLabel, vector<Edge> *connectedEdges){
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

		void addEdge(Edge edge){
			if(!(*edges).size()) (*edges).reserve(sizeof(edge));
			(*edges).push_back(edge);
		}
};

int Node::lastLabel = 1;

#endif
