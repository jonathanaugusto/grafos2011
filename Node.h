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
			edges.reserve(1);
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

		bool operator< (Node node){
			return label < node.label;
		}

		friend ostream& operator<< (ostream& out, Node node){
			out << "Node " << node.label << " connected to " << node.edges.size() << " edges";
			return out;
		}

		void addEdge(Edge *edge){
			if(edges.empty())
				cout << "Edges' vector of node " << label << " is empty" << endl;

			for (unsigned int i = 0; i < edges.size(); i++)
			if (*&edges[i] == *&edge){
				cout << "Edge already added!" << endl;
				return;
			}
			edges.push_back(*&edge);
			cout << "Edge successfully added to " << label << endl;
		}

};

int Node::lastLabel = 1;

#endif
