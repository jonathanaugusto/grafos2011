/**
 * Universidade Federal do Rio de Janeiro
 * COS242 - Teoria dos Grafos
 * @descr	Trabalho pratico da disciplina - Parte 1
 * @author	Bruno Tomas / Jonathan Augusto
 */

#ifndef GRAPH_H
#define GRAPH_H

#include "Includes.h"

/**
 * @brief Definition of as adjacency matrix.
 */
typedef struct vector< vector<bool> > AdjacencyMatrix;

/**
 * @brief Definition of as adjacency list.
 */
typedef struct vector< set<unsigned long long> > AdjacencyList;

using namespace std;

/* Declares template functions of all relational operators
 * deriving their behavior from operator== and operator< .
 */
using namespace rel_ops;

/**
 * @brief Sorts connected components by size (decreasingly)
 */
bool sortBySize (set<unsigned long long> set1, set<unsigned long long> set2){
	if  (set1.size() > set2.size()) return true;
	return false;
}


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
		vector<Edge> g_edges;

		/**
	 	 * @brief Constructor of an empty graph.
	 	 */
		Graph(){
			g_nodes.reserve(0);
			g_edges.reserve(0);
		}

		/**
	 	 * @brief Constructor of a graph reserving
	 	 * space for a given numbers of edges and of nodes.
	 	 * @param	nodes_n	Number of nodes.
	 	 * @param	edges_n	Number of edges.
	 	 */
		Graph (unsigned long long nodes_n, unsigned long long edges_n){
			g_nodes.reserve(nodes_n);
			g_edges.reserve(edges_n);
		}

		/**
		 * @brief Returns number of edges in graph.
		 */
		unsigned long long getEdgesNumber(){
			return g_edges.size();
		}

		/**
		 * @brief Returns number of nodes in graph.
		 * g_nodes[0] is not used to allow access
		 * to g_nodes by label number.
		 */
		unsigned long long getNodesNumber(){
			return g_nodes.size()-1;
		}

		/**
		 * @brief Calculates medium degree of a graph.
		 */
		float getMediumDegree(){
			unsigned long long medium_d = 0;
			for (unsigned long long i = 1; i <= g_nodes.size(); i++) medium_d += g_nodes[i].edges.size();
			return (medium_d / getNodesNumber());
		}

		/**
		 * @brief Calculates and returns empirical
		 * distribution for degrees of nodes.
		 *
		 */
		float* getEmpiricalDistribution(){

			float *node_d = new float[getNodesNumber()];
			for (unsigned long long i = 1; i <= getNodesNumber()-1; i++)
				node_d[i] = 0.0;
			for (unsigned long long i = 1; i <= getNodesNumber(); i++)
				node_d[g_nodes[i].edges.size()] ++;
			for (unsigned long long i = 1; i <= getNodesNumber()-1; i++)
				node_d[i] /= getNodesNumber();

			return node_d;
		}

		/**
		 * @brief Overload of relational operator.
		 */
		bool operator== (Graph graph){
			for (unsigned int i = 1; i <= getNodesNumber(); i++)
				if (g_nodes[i] != graph.g_nodes[i]) return false;
			for (unsigned int i = 0; i < getEdgesNumber(); i++)
				if (g_edges[i] != graph.g_edges[i]) return false;
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

			unsigned long long nodes_n, edges_n, node1_n, node2_n;
			time_t start, end;

			ifstream file (filename, ifstream::in);
			if (file.fail()) cout << "Read error :(" << endl;

			time(&start);

			file >> nodes_n;

			cout << "File: " << filename << endl;

			Node node0(0);
			g_nodes.push_back(node0); // g_nodes[0] empty

			for (unsigned long long i = 1; i <= nodes_n; i++){
				Node node(i);
				g_nodes.push_back(node);
			}
			cout << "Created " << getNodesNumber() << " nodes in graph" << endl;

			edges_n = 0;

			while (!file.eof()){
				Edge *edge = new Edge;

				file >> node1_n;
				file >> node2_n;
				if (node1_n == node2_n) {
					cout << "e(" << node1_n << "," << node2_n << "): Loopback edge?" << endl;
					continue;
				}
				else if ((node1_n == 0)&&(node2_n == 0)){
					cout << "e(" << node1_n << "," << node2_n << " don't exist!" << endl;
					continue;
				}

				edge->addNodes(&g_nodes[node1_n], &g_nodes[node2_n]);

				//g_nodes[node1_n].addEdge(edge);
				//g_nodes[node2_n].addEdge(edge);

				g_edges.push_back(*edge);
				edges_n++;
			}

			cout << "Created " << getEdgesNumber() << " edges in graph" << endl;

			time(&end);
			cout << "Time: " << difftime (end, start) << " seconds" << endl;
			cout << "RAM: " << g_edges.size()*sizeof(Edge)+g_nodes.size()*sizeof(Edge) << " bytes" << endl;

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
			// "Entrada": o proprio grafo que chama a funcao para si mesmo
			// "Saida": arquivo contendo as informacoes constantes do enunciado do trabalho (nada e' retornado pela funcao)


			ofstream file ("info_"+filename, ifstream::out);
			file << "# n = " << getEdgesNumber() << endl;
			file << "# m = " << getNodesNumber() << endl;
			file << "# d_medio = " << getMediumDegree() << endl;

			float *nodeDegrees = getEmpiricalDistribution();

			for (unsigned long long i = 1; i <= getNodesNumber()-1; i++)
				file << i << " " << nodeDegrees[i] << endl;

			file.flush();
			file.close();
		}

		/**
		 * @brief Builds and returns adjacency matrix of a graph.
		 */
		AdjacencyMatrix buildAdjMatrix (){
				// "Entrada": o proprio grafo que chama a funcao para si mesmo
				// Saida:  matriz de adjacencia do grafo

			cout << ":: CREATE ADJACENCY MATRIX ::" << endl;

			time_t start, end;

			time(&start);

			AdjacencyMatrix adjm( getNodesNumber()+1, vector<bool>( getNodesNumber()+1, false ) );

			for (unsigned long long i = 0; i < getEdgesNumber(); i++){
							adjm[g_edges[i].from->label][g_edges[i].to->label] = true;
							//cout << "e(" << g_edges[i].from->label << "," << g_edges[i].to->label << ") ";
							adjm[g_edges[i].to->label][g_edges[i].from->label] = true;
							//cout << "e(" << g_edges[i].to->label << "," << g_edges[i].from->label << ") ";
			}

			time(&end);
			cout << "Time: " << difftime (end, start) << " seconds" << endl;
			cout << "RAM: " << getNodesNumber()*getNodesNumber()/8 << " bytes" << endl;

			return adjm;
		}

		/**
		 * @brief Test adjacency between two nodes using adjacency matrix.
		 * @param	adjm	Adjacency matrix.
		 * @param	node1	Label of a node.
		 * @param	node2 	Label of the other node.
		 */
		bool testAdjacency (AdjacencyMatrix adjm, unsigned long long node1, unsigned long long node2){
			if (node1 == node2){
				cout << "Nodes are same" << endl;
				return false;
			}
			return adjm[node1][node2];
		}

		/**
		 * @brief Builds and returns adjacency list of a graph.
		 */
		AdjacencyList buildAdjList(){

			cout << ":: CREATE ADJACENCY LIST ::" << endl;

			time_t start, end;

			time(&start);

			AdjacencyList adjl (getNodesNumber()+1, set <unsigned long long>());

			for (unsigned long long i = 1; i <= getNodesNumber(); i++){
				for (unsigned long long j = 0; j < g_nodes[i].edges.size(); j++){
					if (g_nodes[i].edges[j]->from == &g_nodes[i]){
						adjl[i].insert(g_nodes[i].edges[j]->to->label);
					}
					else{
						adjl[i].insert(g_nodes[i].edges[j]->from->label);
					}
				}
				//adjl[i].sort();
			}

			time(&end);

			unsigned long long total = 0;
			for (unsigned long long i = 0; i < adjl[i].size(); i++){
				total += adjl[i].size()*sizeof(unsigned long long);
			}

			cout << "Time: " << difftime (end, start) << " seconds" << endl;
			cout << "RAM: " << total << " bytes" << endl;

			return adjl;
		}

		/**
		 * @brief Test adjacency between two nodes using adjacency list.
		 * @param	adjl	Adjacency list.
		 * @param	node1	Label of a node.
		 * @param	node2 	Label of the other node.
		 */
		bool testAdjacency (AdjacencyList adjl, unsigned long long node1, unsigned long long node2){
			if (node1 == node2){
				cout << "Nodes are same" << endl;
				return false;
			}

			set<unsigned long long>::iterator it;
			it = adjl[node1].find(node2);
			if (it == adjl[node1].end()) return false;
			return true;
		}

		/**
		 * @brief Breadth-first search using adjacency matrix.
		 * Returns the connected component found.
		 * @param	adjl			Reference to adjacency matrix.
		 * @param	startingNode	Label of starting node.
		 * @param	filename		Filename where search tree will be written
		 * 							(it will be concatenated with "bfsm_")
		 */
		set<unsigned long long> bfs(AdjacencyMatrix &adjm, unsigned long long startingNode, string filename){

			cout << ":: BFS USING ADJACENCY MATRIX ::" << endl;
			ofstream file ("bfsm_"+filename, ifstream::out);
			queue<unsigned long long> searchQueue;
			unsigned long long set[getNodesNumber()+1][2]; // [0] to dad node; [1] to node level
			std::set<unsigned long long> connected;
			unsigned long long node;

			time_t start, end;

			time(&start);

			for (unsigned int i = 1; i <= getNodesNumber(); i++)
				set[i][2] = false; // node flag

			cout << "Starting node: " << startingNode << endl;

			adjm[startingNode][0] = true;
			set[startingNode][1] = 0;
			searchQueue.push(startingNode);

			file << "n: " << startingNode << "\troot\tlevel: " << set[startingNode][1] << endl;

			while (!searchQueue.empty()){

				node = searchQueue.front();
				searchQueue.pop();
				connected.insert(node);

				for (unsigned int i = 1; i <= getNodesNumber() ; i++){
					if ((adjm[node][i] == true)&&(adjm[i][0] == false)){
						adjm[i][0] = true;
						searchQueue.push(i);
						set[i][0] = node;
						set[i][1] = set[node][1] + 1;
					}
				}

			}

			time(&end);
			cout << "Time: " << difftime (end, start) << " seconds" << endl;

			for (unsigned int i = 1; i <= getNodesNumber() ; i++)
				if ((adjm[i][0] == 1)&&(set[i][1] > 0))
					file << "n: " << i <<"\tdad: " << set[i][0] <<  "\tlevel: " << set[i][1] << endl;

			file.flush();
			file.close();

			return connected;
		}

		/**
		 * @brief Breadth-first search using adjacency list.
		 * Returns the connected component found.
		 * @param	adjl			Reference to adjacency list.
		 * @param	startingNode	Label of starting node.
		 * @param	filename		Filename where search tree will be written
		 * 							(it will be concatenated with "bfsl_")
		 */
		set<unsigned long long> bfs(AdjacencyList &adjl, unsigned long long startingNode, string filename){

			//cout << ":: BFS USING ADJACENCY LIST ::" << endl;
			ofstream file ("bfsl_"+filename, ifstream::out);

			queue<unsigned long long> searchQueue;
			unsigned long long set[getNodesNumber()+1][3]; // [0] to dad node; [1] to node level; [2] to flag
			std::set<unsigned long long> connected;
			unsigned long long node, node2;


			time_t start, end;

			time(&start);

			for (unsigned int i = 1; i <= getNodesNumber(); i++)
				set[i][2] = 0; // node flag

			//cout << "Starting node: " << startingNode << endl;

			set[startingNode][2] = 1;
			set[startingNode][1] = 0;
			searchQueue.push(startingNode);

			file << "n: " << startingNode << "\troot\tlevel: " << set[startingNode][1] << endl;

			while (!searchQueue.empty()){

				node = searchQueue.front();
				searchQueue.pop();
				connected.insert(node);

				for (std::set<unsigned long long>::iterator it = adjl[node].begin(); it != adjl[node].end() ; it++){
					node2 = *it;

					if (set[node2][2] == 0){
						set[node2][2] = 1;
						searchQueue.push(node2);
						set[node2][0] = node;
						set[node2][1] = set[node][1] + 1;
					}
				}
			}



			time(&end);
			//cout << "Time: " << difftime (end, start) << " seconds" << endl;

			for (unsigned int i = 1; i <= getNodesNumber() ; i++)
				if ((set[i][2] == 1)&&(set[i][1] > 0))
					file << "n: " << i <<"\tdad: " << set[i][0] <<  "\tlevel: " << set[i][1] << endl;

			file.flush();
			file.close();
			return connected;
		}


		/**
		 * @brief Depth-first search using adjacency matrix.
		 * Returns the connected component found.
		 * @param	adjm			Reference to adjacency matrix.
		 * @param	startingNode	Label of starting node.
		 * @param	filename		Filename where search tree will be written
		 * 							(it will be concatenated with "dfsm_")
		 */
		set<unsigned long long> dfs(AdjacencyMatrix &adjm, unsigned long long startingNode, string filename){


			//cout << ":: DFS USING ADJACENCY MATRIX ::" << endl;
			ofstream file ("dfsm_"+filename, ifstream::out);

			stack<unsigned long long> searchStack;
			unsigned long long set[getNodesNumber()+1][2]; // [0] to dad node; [1] to node level
			std::set<unsigned long long> connected;
			unsigned long long node;

			time_t start, end;

			time(&start);

			for (unsigned int i = 1; i <= getNodesNumber(); i++)
				adjm[i][0] = false; // node flag

			//cout << "Starting node: " << startingNode << endl;

			set[startingNode][1] = 0;
			searchStack.push(startingNode);

			file << "n: " << startingNode << "\troot\tlevel: " << set[startingNode][1] << endl;

			while (!searchStack.empty()){

				node = searchStack.top();
				searchStack.pop();
				connected.insert(node);

				if(adjm[node][0] == false){
					adjm[node][0] = true; // flag
					for (unsigned int i = 1; i <= getNodesNumber() ; i++){
						if ((adjm[node][i] == true)&&(adjm[i][0] == false)){
							searchStack.push(i);
							set[i][0] = node;
							set[i][1] = set[node][1] + 1;
						}
					}
				}

			}

			time(&end);
			//cout << "Time: " << difftime (end, start) << " seconds" << endl;

			for (unsigned int i = 1; i <= getNodesNumber() ; i++)
				if ((adjm[i][0] == 1)&&(set[i][1] > 0))
					file << "n: " << i <<"\tdad: " << set[i][0] <<  "\tlevel: " << set[i][1] << endl;

			file.flush();
			file.close();


			return connected;

		}

		/**
		 * @brief Depth-first search using adjacency list.
		 * Returns the connected component found.
		 * @param	adjl			Reference to adjacency lsit.
		 * @param	startingNode	Label of starting node.
		 * @param	filename		Filename where search tree will be written
		 * 							(it will be concatenated with "dfsl_")
		 */
		set<unsigned long long> dfs(AdjacencyList &adjl, unsigned long long startingNode, string filename){

			cout << ":: DFS USING ADJACENCY LIST ::" << endl;
			ofstream file ("dfsl_"+filename, ifstream::out);
			stack<unsigned long long> searchStack;
			unsigned long long set[getNodesNumber()+1][3]; // [0] to dad node; [1] to node level; [2] to flag
			std::set <unsigned long long> connected;
			unsigned long long node, node2;


			time_t start, end;

			time(&start);

			for (unsigned int i = 1; i <= getNodesNumber(); i++)
				set[i][2] = 0; // node flag

			cout << "Starting node: " << startingNode << endl;

			set[startingNode][1] = 0;
			searchStack.push(startingNode);

			file << "n: " << startingNode << "\troot\tlevel: " << set[startingNode][1] << endl;

			while (!searchStack.empty()){

				node = searchStack.top();
				searchStack.pop();
				connected.insert(node);

				if (set[node][2] == 0){

					set[node][2] = 1;

					for (std::set<unsigned long long>::iterator it = adjl[node].begin(); it != adjl[node].end() ; it++){
						node2 = *it;

							searchStack.push(node2);
							if(set[node2][2] == 0){	set[node2][0] = node;
							set[node2][1] = set[node][1] + 1;
						}
					}
				}

			}
			time(&end);
			cout << "Time: " << difftime (end, start) << " seconds" << endl;

			for (unsigned int i = 1; i <= getNodesNumber() ; i++)
				if ((set[i][2] == 1)&&(set[i][1] > 0))
					file << "n: " << i <<"\tdad: " << set[i][0] <<  "\tlevel: " << set[i][1] << endl;

			file.flush();
			file.close();


			return connected;
		}

		/**
		 * @brief Found all connected component in graph using adjacency list.
		 * @param	adjl		Adjacency list.
		 * @param	filename	Filename where search tree will be written
		 * 							(it will be concatenated with "dfsm_")
		 */
		void foundConnectedComponents (AdjacencyList adjl, string filename){

			cout << ":: FOUND CONNECTED COMPONENTS ::" << endl;
			ofstream file ("cc_"+filename, ifstream::out);
			string fn;
			set<unsigned long long> component{};
			list <set<unsigned long long> > sortedComponents;

			time_t start, end;

			time(&start);

			for(unsigned long long i = 1; i <= getNodesNumber(); i++){
				for (set<unsigned long long>::iterator it = component.find(i); i == *it; i++, *it++);
				if (i > getNodesNumber()) break;
				component = bfs(adjl,i,filename);
				sortedComponents.push_back(component);
				sortedComponents.sort(sortBySize);
				sortedComponents.unique();
				if (!(sortedComponents.size() % 200)) cout << sortedComponents.size() <<" components found\n";
			}
			cout << sortedComponents.size() <<" components found\n";

			time(&end);
			cout << "Time: " << difftime (end, start) << " seconds" << endl;

			/*for (set <set<unsigned long long> >::iterator it = components.begin(); it != components.end(); it++){
				cout << "Component > ";
				for (set<unsigned long long>::iterator it2 = it->begin(); it2 != it->end(); it2++)
					cout << *it2 << " ";
				cout << endl;
			}
			 */

			cout << "Writing file..." << endl;

			file << sortedComponents.size() << endl;

			for (list <set<unsigned long long> >::iterator it = sortedComponents.begin(); it != sortedComponents.end(); it++){
				file << "[size=" << it->size() << "]";
				for (set<unsigned long long>::iterator it2 = it->begin(); it2 != it->end(); it2++)
					file << " " << *it2;
				file << endl;
			}

			file.flush();
			file.close();
		}

};


#endif
