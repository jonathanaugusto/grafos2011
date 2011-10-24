/**
 * Universidade Federal do Rio de Janeiro
 * COS242 - Teoria dos Grafos
 * @descr	Trabalho pratico da disciplina
 * @author	Bruno Tomas / Jonathan Augusto
 */

#ifndef GRAPH_H
#define GRAPH_H

#pragma once

#include "Node.h"
#include "Edge.h"
#include "AdjacencyMatrix.h"
#include "AdjacencyList.h"


using namespace std;

/* Generate overload of all relational functions
 * deriving their behavior from operator== and operator< .
 */
using namespace rel_ops;

/**
 * @brief Abstraction of a graph.
 */
class Graph{

	public:

		/**
	 	 * @brief Vector of graph nodes.
	 	 */
		vector<Node> g_nodes;

		/**
	 	 * @brief Vector of graph edges.
	 	 */
		set<Edge,Edge::compare> g_edges;

		/**
	 	 * @brief Constructor of an empty graph.
	 	 */
		Graph(){
			g_nodes.reserve(0);
		}

		/**
	 	 * @brief Constructor of a graph reserving
	 	 * space for a given numbers of edges and of nodes.
	 	 * @param	nodes_n	Number of nodes.
	 	 * @param	edges_n	Number of edges.
	 	 */
		Graph (unsigned long nodes_n, unsigned long edges_n){
			g_nodes.reserve(nodes_n+1);
		}

		/**
		 * @brief Returns number of edges in graph.
		 */
		unsigned long getEdgesNumber(){
			return g_edges.size();
		}

		/**
		 * @brief Returns number of nodes in graph.
		 * g_nodes[0] is not used to allow access
		 * to g_nodes by label number.
		 */
		unsigned long getNodesNumber(){
			return g_nodes.size()-1;
		}

		/**
		 * @brief Calculates medium degree of a graph.
		 */
		float getMediumDegree(){
			unsigned long medium_d = 0;
			for (unsigned long i = 1; i <= g_nodes.size(); i++) medium_d += g_nodes[i].edges.size();
			return (medium_d / getNodesNumber());
		}

		/**
		 * @brief Calculates and returns empirical
		 * distribution for degrees of nodes.
		 *
		 */
		float* getEmpiricalDistribution(){

			float *node_d = new float[getNodesNumber()];
			for (unsigned long i = 0; i < getNodesNumber(); i++)
				node_d[i] = 0.0;
			for (unsigned long i = 1; i <= getNodesNumber(); i++)
				node_d[g_nodes[i].edges.size()] ++;
			for (unsigned long i = 0; i < getNodesNumber(); i++)
				node_d[i] /= getNodesNumber();

			return node_d;
		}

		/**
		 * @brief Overload of relational operator.
		 */
		bool operator== (Graph graph){
			for (unsigned int i = 1; i <= getNodesNumber(); i++)
				if (g_nodes[i] != graph.g_nodes[i]) return false;
				if (g_edges != graph.g_edges) return false;
			return true;
		}


		/**
		 * @brief Build graph from description file
		 * (Line with number of nodes and then each line
		 * with an edge)
		 * @param filename	String containing filename.
		 */
		void buildGraph(string filename){

			cout << ":: CREATE GRAPH ::" << endl;

			unsigned long nodes_n, edges_n, node1_n, node2_n, weight;

			ifstream file (filename, ifstream::in);
			if (file.fail()) cout << "Read error :(" << endl;

			clock_t start = clock();

			file >> nodes_n;

			cout << "File: " << filename << endl;

			Node node0(0);
			g_nodes.push_back(node0); // g_nodes[0] empty

			for (unsigned long i = 1; i <= nodes_n; i++){
				Node node(i);
				g_nodes.push_back(node);
			}
			cout << "Created " << getNodesNumber() << " nodes in graph" << endl;

			edges_n = 0;
			while (!file.eof()){
					file >> node1_n;
					file >> node2_n;
					file >> weight;

				Edge *edge = (Edge *) malloc (sizeof(Edge));
				edge->weight = weight;
				edge->isDirected = false;

				edge->addNodes(&g_nodes[node1_n], &g_nodes[node2_n]);

				//g_nodes[node1_n].addEdge(edge);
				//g_nodes[node2_n].addEdge(edge);

				g_edges.insert(*edge);
				edges_n++;
			}

			cout << "Created " << getEdgesNumber() << " edges in graph" << endl;

			clock_t end = clock();

			ofstream operationsFile (OPERATIONSFILE_NAME, ifstream::app);
			if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
			else operationsFile << "buildFG:" << (float)((end - start)/CLOCKS_PER_SEC) << ":" << g_edges.size()*sizeof(Edge)+g_nodes.size()*sizeof(Edge) << endl;
			operationsFile.flush();
			operationsFile.close();

			file.close();
		}


		/**
		 * @brief Build information file of a graph,
		 * containing nodes' and edges' number,
		 * medium degree etc.
		 * @param filename	String containing filename
		 * 					(it will be concatenated with "info_")
		 */
		void buildInformationFile (string filename){

			ofstream file ("info_"+filename, ifstream::out);
			file << "# n = " << getEdgesNumber() << endl;
			file << "# m = " << getNodesNumber() << endl;
			file << "# d_medio = " << getMediumDegree() << endl;

			float *nodeDegrees = getEmpiricalDistribution();

			for (unsigned long i = 0; i < getNodesNumber()-1; i++)
				if (nodeDegrees[i]!= 0) file << i << " " << nodeDegrees[i] << endl;

			file.flush();
			file.close();
		}

		/**
		 * @brief Builds and returns adjacency matrix of a graph.
		 */
		AdjacencyMatrix convertToMatrix (){

			cout << ":: CREATE ADJACENCY MATRIX ::" << endl;

			float start = clock()/CLOCKS_PER_SEC;

			AdjacencyMatrix adjm( getNodesNumber() );

			for (set<Edge>::iterator it = g_edges.begin(); it != g_edges.end(); it++){
				unsigned long n1 =	it->from->label;
				unsigned long n2 =	it->to->label;
				if (n1<n2) adjm[n1-1][n2-n1-1] = it->weight;
				else adjm[n2-1][n1-n2-1] = it->weight;
				//cout << "e(" << n1 << "," << n2 << ") ";
			}

			float end = clock()/CLOCKS_PER_SEC;

			ofstream operationsFile (OPERATIONSFILE_NAME, ifstream::app);
			if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
			else operationsFile << "buildGM:" << end - start << ":" << getNodesNumber()*getNodesNumber()/8 << endl;
			operationsFile.flush();
			operationsFile.close();

			return adjm;
		}

		/**
		 * @brief Builds and returns adjacency list of a graph.
		 */
		AdjacencyList convertToList(){

			cout << ":: CREATE ADJACENCY LIST ::" << endl;

			float start = clock()/CLOCKS_PER_SEC;

			AdjacencyList adjl (getNodesNumber()+1);

			for (unsigned long i = 1; i <= getNodesNumber(); i++){
				for (set<Edge *>::iterator it=g_nodes[i].edges.begin(); it!=g_nodes[i].edges.end(); it++){
					if ((*it)->from == &g_nodes[i])
						adjl[i].insert(make_pair ((*it)->to->label,(*it)->weight));
					else
						adjl[i].insert(make_pair ((*it)->from->label,(*it)->weight));
				}
				//adjl[i].sort();
			}

			float end = clock()/CLOCKS_PER_SEC;

			ofstream operationsFile (OPERATIONSFILE_NAME, ifstream::app);
			if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
			else {
				unsigned long total = 0;
				for (unsigned long i = 0; i < adjl.size(); i++)
					total += adjl[i].size()*sizeof(unsigned long);
				operationsFile << "buildGL:" << end - start << ":" << g_edges.size()*sizeof(Edge)+g_nodes.size()*sizeof(Edge) << endl;
			}
			operationsFile.flush();
			operationsFile.close();


			return adjl;
		}

		set<Node *> bfs(unsigned long startingNode,  string filename){

			cout << ":: BFS USING GRAPH ::" << endl;
			ofstream file ("bfsg_"+filename, ifstream::out);

			queue<Node *> searchStack;
			Node* dadNode[g_nodes.size()+1];
			vector<long> nodeLevels (g_nodes.size()+1, -1);
			std::set<Node *> connected;

			float start = clock()/CLOCKS_PER_SEC;

			for (unsigned long i = 0; i < this->g_nodes.size(); i++){
				nodeLevels[i] = 0;
			}

			cout << "Starting node: " << startingNode << endl;

			nodeLevels[startingNode] = 0;
			searchStack.push(&g_nodes[startingNode]);
			g_nodes[startingNode].flag();

			file << "n: " << startingNode << "\troot\tlevel: " << nodeLevels[startingNode] << endl;

			while (!searchStack.empty()){
				Node *node = searchStack.front();
				searchStack.pop();
				connected.insert(node);
				cout << "T " << *node << " | ";
				list <Node *> connectedNodes = node->getConnectedNodes();
				for (list<Node *>::iterator it=connectedNodes.begin(); it!=connectedNodes.end(); it++){
					Node *node2 = *it;
					if (node2->isflagged() == false){
						node2->flag();
						searchStack.push(node2);
						dadNode[node2->label] = node;
						nodeLevels[node2->label] = nodeLevels[node->label] + 1;
					}
				}
			}

			float end = clock()/CLOCKS_PER_SEC;

			ofstream operationsFile (OPERATIONSFILE_NAME, ifstream::app);
			if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
			else operationsFile << "dfsL:" << end - start << endl;
			operationsFile.flush();
			operationsFile.close();

			for (unsigned int i = 1; i < g_nodes.size(); i++)
				if (nodeLevels[i] > 0)
					file << "n: " << i <<"\tdad: " << dadNode[i]->label <<  "\tlevel: " << nodeLevels[i] << endl;

			file.flush();
			file.close();


			return connected;


		}

		vector <Edge *> path(unsigned long node){


		}

		vector <Edge *> path (unsigned long startingNode, unsigned long endingNode){


		}


};

#endif
