/**
 * Universidade Federal do Rio de Janeiro
 * COS242 - Teoria dos Grafos
 * @descr	Trabalho pratico da disciplina
 * @author	Bruno Tomas / Jonathan Augusto
 */

#ifndef EDGE_H
#define EDGE_H

#pragma once		// Solve cross-reference (Node includes Edge, that includes Node...)

#include "Node.h"

using namespace std;

/**
 * @brief Abstraction of a graph edge.
 */
class Edge{

	public:

		/**
		 * @brief Value of edge weight.
		 */
		float weight;

		/**
		 * @brief Pointers to the nodes connected.
		 */
		Node *from, *to;

		/**
		 * @brief Flag for node directed (true) or undirected (false).
		 */
		bool isDirected;

		/**
		 * @brief Constructor for class Edge.
		 */
		Edge(){
			from = to = NULL;
			weight = 1;
			isDirected = false;
		}

		/**
		 * @brief Constructor for class Edge.
		 * @param	edgeWeight	Value of edge weight.
		 * 			isDir		Flag for (un)directed edge.
		 */
		Edge(unsigned long int edgeWeight, bool isDir){

			weight = edgeWeight;
			isDirected = isDir;
			from = to = NULL;
		}

		/**
		 * @brief Constructor for class Edge.
		 * @param	fromNode	Pointer to edge that connects from.
		 * 			toNode		Pointer to edge that connects to.
		 * 			edgeWeight	Value of edge weight.
		 * 			isDir		Flag for (un)directed edge.
		 */
		Edge(Node *fromNode, Node *toNode, unsigned long int edgeWeight, bool isDir){

			weight = edgeWeight;
			isDirected = isDir;
			this->addNodes(fromNode, toNode);
		}

		/**
		 * @brief Overload of relational operators.
		 */
		bool operator== (Edge edge) const{
			return ((from == edge.from) && (to == edge.to) && (isDirected == false) && (edge.isDirected == false));
		}

		/**
		 * @brief Overload of relational operators.
		 */
		bool operator< (Edge edge) const{
			if (weight < edge.weight) return true;
			else if (weight > edge.weight) return false;
			else {
				if (*from < *(edge.from)) return true;
				else if (*from < *(edge.from)) return false;
				else {
					if (*to < *(edge.to)) return true;
					else if (*to < *(edge.to)) return false;
				}
			}
		}

		/**
		 * @brief Overload of output insertion operator.
		 */
		friend ostream& operator<< (ostream& out, Edge edge){
			out << "e(" << edge.from->label;
			if (!edge.isDirected) out << "-" << edge.to->label << ")";
			else out << "->" << edge.to->label << ")";
			cout << "=" << edge.weight;
			return out;
		}

		/**
		 * @brief Add nodes to an edge.
		 * @param	fromNode	Pointer to edge that connects from.
		 * 			toNode		Pointer to edge that connects to.
		 */
		void addNodes (Node *fromNode, Node *toNode){
			from = fromNode;
			to = toNode;
			fromNode->addEdge (this);
			toNode->addEdge (this);
		}

		struct compare {
			bool operator() (const Edge &edge1, const Edge &edge2){
				return edge1 < edge2;
			}
		};
};

/**
 * @brief Add edge to a node.
 * @param edge	Pointer to edge.
 */
void Node::addEdge(Edge *edge){

	set<Edge *>::iterator it;
	it = edges.find (edge);
	if (it == edges.end())
		edges.insert(edge);

};

/**
 * @brief Display list of connected edges.
 */
void Node::printEdges (){
		cout << "Edges connected to node " <<  label << ":" << endl;
		for (set<Edge *>::iterator it=edges.begin(); it!=edges.end(); it++)
			cout << *it << endl;
		cout << endl;
}


list<Node *> Node::getConnectedNodes(){
	list<Node *> connected;
	for (set<Edge *>::iterator it=edges.begin(); it!=edges.end(); it++){
		if((*(*(*it)).from) == *this)
			connected.push_back(&*(*(*it)).to);
		else
			connected.push_back(&*(*(*it)).from);
	}
	connected.sort(sortByLabel);

	return connected;
}






#endif
