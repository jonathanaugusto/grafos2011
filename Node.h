#ifndef NODE_H
#define NODE_H

#pragma once // Para resolver a referencia cruzada (Node inclui Edge, que inclui Node...)
class Edge; // Indica que ha' uma classe Node a ser referenciada aqui
#include "Edge.h"
#include "Includes.h"

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

		bool operator== (Node node) const{
			return label == node.label;
		}
/*
		bool operator!= (Node node){
			if (this==node) return false;
			return true;
		}*/

		bool operator< (Node node) const{
			return label < node.label;
		}

		friend ostream& operator<< (ostream& out, Node node){
			out << "n" << node.label << " (d=" << node.edges.size() << ")";
			return out;
		}

		void addEdge(Edge*);
		// defined in Edge.h

		void printEdges();
		// defined in Edge.h

};



int Node::lastLabel = 1;

#endif
