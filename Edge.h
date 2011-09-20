/**
 * Universidade Federal do Rio de Janeiro
 * COS242 - Teoria dos Grafos
 * @descr	Trabalho pratico da disciplina - Parte 1
 * @author	Bruno Tomas / Jonathan Augusto
 */

#ifndef EDGE_H
#define EDGE_H

#pragma once		// Solve cross-reference (Node includes Edge, that includes Node...)
class Node;			// Defined in Edge.h
#include "Node.h"
#include "Includes.h"

using namespace std;

/**
 * @brief Abstraction of a graph edge.
 */
class Edge{

	public:

		/**
		 * @brief Value of edge weight.
		 */
		unsigned long int weight;

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
			return ((*from == *(edge.from)) && (*to == *(edge.to)) && (isDirected == false) && (edge.isDirected == false));
		}

		/**
		 * @brief Overload of relational operators.
		 */
		bool operator< (Edge edge) const{
			return ((*from == *(edge.from)) && (*to == *(edge.to)) && ((weight < edge.weight)));
		}

		/**
		 * @brief Overload of output insertion operator.
		 */
		friend ostream& operator<< (ostream& out, Edge edge){
			out << "e(" << edge.from->label << "," << edge.to->label;
			if (!edge.isDirected) out << ",-)";
			else out << ",->)";
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


};

/**
 * @brief Add edge to a node.
 * @param edge	Pointer to edge.
 */
void Node::addEdge(Edge *edge){

	for (unsigned unsigned long int i = 0; i < edges.size(); i++)
		if (*edges[i] == *edge) return;

	edges.push_back(edge);

};

/**
 * @brief Display list of connected edges.
 */
void Node::printEdges (){
		cout << "Edges connected to node " <<  label << ":" << endl;
		for (unsigned unsigned long int i = 0; i < edges.size(); i++){
			cout << *edges[i] << endl;
		}
		cout << endl;
}


#endif
