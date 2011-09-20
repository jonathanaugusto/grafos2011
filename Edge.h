#ifndef EDGE_H
#define EDGE_H

#pragma once // Para resolver a referencia cruzada (Node inclui Edge, que inclui Node...)
class Node; // Indica que ha' uma classe Node a ser referenciada aqui
#include "Node.h"
#include "Includes.h"

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

		Edge(int edgeWeight, bool isDir){

			weight = edgeWeight;
			isDirected = isDir;
			from = to = NULL;
		}

		Edge(Node *fromNode, Node *toNode, int edgeWeight, bool isDir){

			weight = edgeWeight;
			isDirected = isDir;
			this->addNodes(fromNode, toNode);
		}

		bool operator== (Edge edge) const{
			return ((*from == *(edge.from)) && (*to == *(edge.to)) && (isDirected == false) && (edge.isDirected == false));
		}

		bool operator< (Edge edge) const{
			return ((*from == *(edge.from)) && (*to == *(edge.to)) && ((weight < edge.weight)));
		}


		friend ostream& operator<< (ostream& out, Edge edge){
			out << "e(" << edge.from->label << "," << edge.to->label;
			if (!edge.isDirected) out << ",-)";
			else out << ",->)";
			return out;
		}

		void addNodes (Node *fromNode, Node *toNode){
			from = fromNode;
			to = toNode;
			fromNode->addEdge (this);
			toNode->addEdge (this);
		}

		void set(){
			flag.set();
		}

		void unset(){
			flag.reset();
		}

		bool isset(){
			if (flag.any()) return true;
			return false;
		}

};

void Node::addEdge(Edge *edge){

	for (unsigned int i = 0; i < edges.size(); i++)
		if (*edges[i] == *edge) return;

	edges.push_back(edge);

};

void Node::printEdges (){
		cout << "Edges connected to node " <<  label << ":" << endl;
		for (unsigned int i = 0; i < edges.size(); i++){
			cout << *edges[i] << endl;
		}
		cout << endl;
}


#endif
