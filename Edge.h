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
		 *
		 */
		bitset<1> bit_flag;

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
			bit_flag.reset();
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
			return ((from == edge.from) && (to == edge.to) && (weight == edge.weight));
		}
		bool operator== (Edge *edge) const{
			return ((from == edge->from) && (to == edge->to) && (weight == edge->weight));
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
			return false;
		}

		/**
		 * @brief Overload of output insertion operator.
		 */
		friend ostream& operator<< (ostream& out, Edge edge){
			out << "e(" << edge.from->label;
			if (!edge.isDirected) out << "-" << edge.to->label << ")";
			else out << "->" << edge.to->label << ")";
			out << "=" << edge.weight;
			return out;
		}

		void flag(){
			bit_flag.set();
		}

		void unflag(){
			bit_flag.reset();
		}

		bool test(){
			return bit_flag[0];
		}


		/**
		 * @brief Add nodes to an edge.
		 * @param	fromNode	Pointer to node that connects from.
		 * 			toNode		Pointer to node that connects to.
		 */
		void addNodes (Node *fromNode, Node *toNode){
			from = fromNode;
			to = toNode;
			fromNode->addEdge (this);
			toNode->addEdge (this);
		}

		struct compare {
			bool operator() (const Edge &edge1, const Edge &edge2){
				if (edge1.weight < edge2.weight) return true;
				else if (edge1.weight > edge2.weight) return false;
				else {
					if (edge1.from->label < edge2.from->label) return true;
					else if (edge1.from->label < edge2.from->label) return false;
					else {
						if (edge1.to->label < edge2.to->label) return true;
						else if (edge1.to->label < edge2.to->label) return false;
					}
				}
				return false;
			}
		};

		struct comparePointers {
			bool operator() (const Edge *edge1, const Edge *edge2){
				if (edge1->weight < edge2->weight) return true;
				else if (edge1->weight > edge2->weight) return false;
				else {
					if (edge1->from->label < edge2->from->label) return true;
					else if (edge1->from->label < edge2->from->label) return false;
					else {
						if (edge1->to->label < edge2->to->label) return true;
						else if (edge1->to->label < edge2->to->label) return false;
					}
				}
				return false;
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


bool sortByWeight (const Edge *edge1, const Edge *edge2){
		 if (*edge1 < *edge2)
			 return true;
		 return false;
	}





#endif
