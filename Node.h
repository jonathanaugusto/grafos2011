/**
 * Universidade Federal do Rio de Janeiro
 * COS242 - Teoria dos Grafos
 * @descr	Trabalho pratico da disciplina
 * @author	Bruno Tomas / Jonathan Augusto
 */

#ifndef NODE_H
#define NODE_H

#pragma once		// Solve cross-reference (Node includes Edge, that includes Node...)
class Edge;			// Defined in Edge.h
#include "Edge.h"
using namespace std;

/**
 * @brief Abstraction of a graph node.
 */
class Node{

	public:

		/**
		 * @brief Unique identifier of a node.
		 */
		unsigned long int label;

		/**
		 * @brief Vector that stores pointers to
		 * edges connected to this node.
		 */
		set<Edge*> edges;

		/**
		 * @brief Flag for node mark in search
		 * functions (BFS, DFS)
		 */
		bitset<1> bit_flag;

		/**
		 * @brief Minimum distance from another node
		 * (used in path functions)
		 */
		float distance;

		/**
		 * @brief Constructor for class Node.
		 * @param nodeLabel		Identifier for object.
		 */
		Node(unsigned long int nodeLabel){
			label = nodeLabel;
			bit_flag.reset();
		}

		/**
		 * @brief Constructor for class Node.
		 * @param nodeLabel			Identifier for object.
		 * @param connectedEdges	Vector of pointers to
		 * 							edges connected to this.
		 */
		Node(unsigned long int nodeLabel, set<Edge*> connectedEdges){
			label = nodeLabel;
			edges = connectedEdges;
			bit_flag.reset();
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

		friend ostream& operator<< (ostream& out, vector<Node *> v){
			out << v[0]->label;
			for (unsigned int i = 1; i < v.size(); ++i) {
				out << "-" << v[i]->label;
			}
			return out;
		}

		void addEdge(Edge*);
		// defined in Edge.h

		void printEdges();
		// defined in Edge.h

		void flag(){
			bit_flag.set();
		}

		void unflag(){
			bit_flag.reset();
		}

		bool test(){
			return bit_flag.test(0);
		}

		unsigned long getDegree(){
			return edges.size();
		}

		list<Node *> getConnectedNodes();

};

bool sortByLabel (Node *node1, Node *node2){
	return node1->label < node2->label;
}

#endif
