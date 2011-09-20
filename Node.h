/**
 * Universidade Federal do Rio de Janeiro
 * COS242 - Teoria dos Grafos
 * @descr	Trabalho pratico da disciplina - Parte 1
 * @author	Bruno Tomas / Jonathan Augusto
 */

#ifndef NODE_H
#define NODE_H

#pragma once		// Solve cross-reference (Node includes Edge, that includes Node...)
class Edge;			// Defined in Edge.h
#include "Edge.h"
#include "Includes.h"

using namespace std;

/**
 * @brief Abstraction of a graph node.
 */
class Node{

	public:

		/**
		 * @brief Unique identifier of a node.
		 */
		int label;

		/**
		 * @brief Vector that stores pointers to
		 * edges connected to this node.
		 */
		vector<Edge*> edges;

		/**
		 * @brief Flag for node mark in search
		 * functions (BFS, DFS)
		 */
		bitset<1> flag;

		/**
		 * @brief Constructor for class Node.
		 * @param nodeLabel		Identifier for object.
		 */
		Node(int nodeLabel){
			label = nodeLabel;
		}

		/**
		 * @brief Constructor for class Node.
		 * @param nodeLabel			Identifier for object.
		 * @param connectedEdges	Vector of pointers to
		 * 							edges connected to this.
		 */
		Node(int nodeLabel, vector<Edge*> connectedEdges){
			label = nodeLabel;
			edges = connectedEdges;
		}

		/**
		 * @brief Overload of assignment operator.
		 */
		Node &operator=(Node node){
			label = node.label;
			edges = node.edges;
			return *this;
		}

		/**
		 * @brief Overload of relational operators.
		 */
		bool operator== (Node node) const{
			return label == node.label;
		}

		/**
		 * @brief Overload of relational operators.
		 */
		bool operator< (Node node) const{
			return label < node.label;
		}

		/**
		 * @brief Overload of output insertion operator.
		 */
		friend ostream& operator<< (ostream& out, Node node){
			out << "n" << node.label << " (d=" << node.edges.size() << ")";
			return out;
		}

		void addEdge(Edge*);
		// defined in Edge.h

		void printEdges();
		// defined in Edge.h

};



#endif
