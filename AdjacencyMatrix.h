/**
 * Universidade Federal do Rio de Janeiro
 * COS242 - Teoria dos Grafos
 * @descr	Trabalho pratico da disciplina
 * @author	Bruno Tomas / Jonathan Augusto
 */

#ifndef ADJACENCYMATRIX_H_
#define ADJACENCYMATRIX_H_

#pragma once

#include "Graph.h"

class Graph;

/**
 * @brief Definition of as adjacency matrix.
 */
class AdjacencyMatrix : public std::vector< vector<float> > {

public:

	bitset<2> weighted; // [0] to weighted graph; [1] to a negative weight

	AdjacencyMatrix(){
		new (this) vector< vector<float> >( 0, vector<float>(0, 0.0));
	}

	AdjacencyMatrix(unsigned long nodes){
		new (this) vector< vector<float> >( nodes-1, vector<float>(0, 0.0));
		unsigned int i;
		for (i = 0; i < nodes-1; i++ ){
			vector<float> v (nodes-i-1, 0.0);
			this->at(i) = v;
		}
	}

	bool setAdjacency (unsigned long node1, unsigned long node2, float weight){
		if (node1 == node2) return true;
		if (node1 < node2) this->at(node1-1).at(node2-node1-1) = weight;
		else if (node1 > node2) this->at(node2-1).at(node1-node2-1) = weight;
		return false;
	}

	/**
	 * @brief Test adjacency between two nodes using adjacency matrix.
	 * @param	this	Adjacency matrix.
	 * @param	node1	Label of a node.
	 * @param	node2 	Label of the other node.
	 */
	float getAdjacency (unsigned long node1, unsigned long node2){
		if (node1 == node2) return 0;
		if (node1 < node2) return at(node1-1).at(node2-node1-1);
		return at(node2-1).at(node1-node2-1);
	}

	unsigned long getNodesNumber(){
		return size()+1;
	}



	/**
	 * @brief Builds and returns adjacency matrix from description file
	 * (Line with number of nodes and then each line
	 * with an edge)
	 * @param filename	String containing filename.
	 */
	void build(string filename){

		cout << ":: CREATE ADJACENCY MATRIX ::" << endl;

		unsigned long nodes_n, node1, node2, weight, edges_n;

		ifstream file (filename, ifstream::in);
		if (file.fail()) cout << "Read error :(" << endl;

		clock_t start = clock();

		file >> nodes_n;

		cout << "File: " << filename << endl;

		new (this) AdjacencyMatrix (nodes_n);

		cout << "Created " << nodes_n << " nodes in adjacency matrix" << endl;

		edges_n = 0;
		while (!file.eof()){
			file >> node1;
			file >> node2;
			if (file.eof()) break;
			file >> weight;
			if (weight != 1) weighted[0] = true;
			if (weight < 0) weighted[1] = true;
			setAdjacency(node1,node2,weight);
			edges_n++;
		}

		cout << "Created " << edges_n << " edges in adjacency matrix" << endl;

		clock_t end = clock();

		ofstream operationsFile (OPERATIONSFILE_NAME, ifstream::app);
		if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
		else operationsFile << "buildFM:" << (float)((end - start)/CLOCKS_PER_SEC) << ":" << size()*sizeof(unsigned long) << endl;
		operationsFile.flush();
		operationsFile.close();

		file.close();

		//return adjm;
	}

	/**
	 * @brief Breadth-first search using adjacency matrix.
	 * Returns the connected component found.
	 * @param	adjl			Reference to adjacency matrix.
	 * @param	startingNode	Label of starting node.
	 * @param	filename		Filename where search tree will be written
	 * 							(it will be concatenated with "bfsm_")
	 */
	set<unsigned long> bfs(unsigned long startingNode, string filename){

		cout << ":: BFS USING ADJACENCY MATRIX ::" << endl;
		ofstream file ("bfsm_"+filename, ifstream::out);
		queue<unsigned long> searchQueue;
		unsigned long set[size()][3]; // [0] to dad node; [1] to node level; [2] to flag
		std::set<unsigned long> connected;
		unsigned long node;

		float start = clock()/CLOCKS_PER_SEC;

		for (unsigned int i = 1; i <= getNodesNumber(); i++){
			set[i-1][2] = 0; // node flag
			set[i-1][1] = -1;
		}

		cout << "Starting node: " << startingNode << endl;

		set[startingNode-1] = {0,0,1};
		searchQueue.push(startingNode);

		file << "n: " << startingNode << "\troot\tlevel: " << set[startingNode-1][1] << endl;

		while (!searchQueue.empty()){

			node = searchQueue.front();
			searchQueue.pop();
			connected.insert(node);

			for (unsigned int i = 1; i <= getNodesNumber(); i++){
				if (getAdjacency(node,i)&&(set[i-1][2] == 0)){
					set[i-1][2] = 1;
					searchQueue.push(i);
					set[i-1][0] = node;
					set[i-1][1] = set[node-1][1] + 1;
				}
			}
		}

		float end = clock()/CLOCKS_PER_SEC;

		ofstream operationsFile (OPERATIONSFILE_NAME, ifstream::app);
		if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
		else operationsFile << "bfsL:" << end - start << endl;
		operationsFile.flush();
		operationsFile.close();

		for (unsigned int i = 1; i <= getNodesNumber(); i++)
			if ((set[i-1][2] == 1)&&(set[i-1][1] > 0))
				file << "n: " << i <<"\tdad: " << set[i-1][0] <<  "\tlevel: " << set[i-1][1] << endl;


		file.flush();
		file.close();

		return connected;
	}

	/**
	 * @brief Depth-first search using adjacency matrix.
	 * Returns the connected component found.
	 * @param	this			Reference to adjacency matrix.
	 * @param	startingNode	Label of starting node.
	 * @param	filename		Filename where search tree will be written
	 * 							(it will be concatenated with "dfsm_")
	 */
	set<unsigned long> dfs(unsigned long startingNode, string filename){


		cout << ":: DFS USING ADJACENCY MATRIX ::" << endl;
		ofstream file ("dfsm_"+filename, ifstream::out);

		stack<unsigned long> searchStack;
		unsigned long set[size()][3]; // [0] to dad node; [1] to node level; [2] to flag
		std::set<unsigned long> connected;
		unsigned long node;

		float start = clock()/CLOCKS_PER_SEC;

		for (unsigned int i = 1; i <= size(); i++){
			set[i-1][2] = 0; // node flag
			set[i-1][1] = -1;
		}

		cout << "Starting node: " << startingNode << endl;

		set[startingNode-1] = {0,0,0};
		searchStack.push(startingNode);

		file << "n: " << startingNode << "\troot\tlevel: " << set[startingNode-1][1] << endl;

		while (!searchStack.empty()){

			node = searchStack.top();
			searchStack.pop();
			connected.insert(node);

			if(set[node-1][2] == 0){
				set[node-1][2] = 1; // flag
				for (unsigned int i = 1; i <= getNodesNumber() ; i++){
					if (getAdjacency(node,i)&&(set[i-1][2] == 0)){
						searchStack.push(i);
						set[i-1][0] = node;
						set[i-1][1] = set[node-1][1] + 1;
					}
				}
			}

		}

		float end = clock()/CLOCKS_PER_SEC;

		ofstream operationsFile (OPERATIONSFILE_NAME, ifstream::app);
		if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
		else operationsFile << "dfsL:" << end - start << endl;
		operationsFile.flush();
		operationsFile.close();

		for (unsigned int i = 1; i <= getNodesNumber(); i++)
			if ((set[i-1][2] == 1)&&(set[i-1][1] > 0))
				file << "n: " << i <<"\tdad: " << set[i-1][0] <<  "\tlevel: " << set[i-1][1] << endl;

		file.flush();
		file.close();


		return connected;

	}

	vector<unsigned long> nonWeightedPath(unsigned long startingNode, unsigned long endingNode, string filename){

		cout << ":: MINIMUM PATH USING ADJACENCY MATRIX ::" << endl;
		ofstream file ("pathm_"+filename, ifstream::out);

		queue<unsigned long> searchStack;
		vector< vector <unsigned long> > path (getNodesNumber()+1, vector <unsigned long>(0));
		vector<bool> flag (getNodesNumber()+1, false);

		float start = clock()/CLOCKS_PER_SEC;

		cout << "Starting node: " << startingNode << "; Ending node: " << endingNode << endl;
		searchStack.push(startingNode);
		flag[startingNode] = true;
		path[startingNode].push_back(startingNode);

		file << "n: " << startingNode << "\troot" << endl;

		while (!searchStack.empty()){
			unsigned long node = searchStack.front();
			searchStack.pop();
			for (unsigned int i = 1; i <= getNodesNumber(); i++){
				if ((flag[i] == false) && (getAdjacency (i,node) != 0)){
					flag[i] = true;
					searchStack.push(i);
					path[i] = path[node];
					path[i].push_back(i);
				}
			}
		}

		float end = clock()/CLOCKS_PER_SEC;

		ofstream operationsFile (OPERATIONSFILE_NAME, ifstream::app);
		if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
		else operationsFile << "pathnM:" << end - start << endl;
		operationsFile.flush();
		operationsFile.close();

		for (unsigned int i = 1; i <= getNodesNumber(); i++){
			if (path[i].size() > 1){
				file << "n: " << i <<"\tpath: " << path[i][0];
				for (unsigned int j = 0; j < path[i].size(); ++j) {
					file << "-" << path[i][j];
				}
				file << endl;
			}
		}

		file.flush();
		file.close();

		return path[endingNode];

	}

	vector<unsigned long> nonWeightedPath(unsigned long startingNode, string filename){
		// For all nodes
		return nonWeightedPath(startingNode, 0, filename);
	}

	vector<unsigned long> nonWeightedPath(unsigned long startingNode, unsigned long endingNode){
		// For a specific node
		return nonWeightedPath(startingNode, endingNode, "");
	}


	struct sortByDistance{
				bool operator() (pair <unsigned long,float*> p1, pair <unsigned long,float*> p2){
					if (*(p1.second) < *(p2.second))
						return true;
					else if (p1.first < p2.first)
						return true;
					return false;
				}
	};

	pair<float,vector<unsigned long> > dijkstra(unsigned long startingNode, unsigned long endingNode, string filename){

		cout << ":: DIJKSTRA USING ADJACENCY MATRIX ::" << endl;
		ofstream file ("dijkm_"+filename, ifstream::out);

		vector <vector<unsigned long> > path (getNodesNumber()+1, vector <unsigned long>(0));

		set <pair <unsigned long,float*>, sortByDistance > nodes; // V-S
		float distance[getNodesNumber()+1];
		float start = clock()/CLOCKS_PER_SEC;

		for (unsigned int i = 1; i <= getNodesNumber(); ++i) {
			distance[i] = numeric_limits<float>::infinity();
			if (i != startingNode) nodes.insert (make_pair(i,&distance[i]));
		}
		distance[startingNode] = 0;
		nodes.insert (make_pair(startingNode,&distance[startingNode]));
		file << "n: " << startingNode << "\troot" << endl;

		while (nodes.size() > 0){
			pair <unsigned long,float*> node = *nodes.begin();
			nodes.erase(nodes.begin());
			for (unsigned int i = 1; i <= getNodesNumber(); i++){
				if (getAdjacency (node.first, i)!= 0){
					if (distance[i] > distance[node.first] + getAdjacency (node.first, i)){
						distance[i] = distance[node.first] + getAdjacency (node.first, i);
						path[i] = path[node.first];
						path[i].push_back(node.first);
					}
				}
			}
		}

		float end = clock()/CLOCKS_PER_SEC;

		ofstream operationsFile (OPERATIONSFILE_NAME, ifstream::app);
		if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
		else operationsFile << "dijkM:" << end - start << endl;
		operationsFile.flush();
		operationsFile.close();

		for (unsigned int i = 1; i <= getNodesNumber(); i++){
			if (path[i].size() > 0){
				file << "n: " << i <<"\tpath: ";
				for (unsigned int j = 0; j < path[i].size(); ++j, file << "-") {
					file << path[i][j];
				}
				file << i << endl;
			}
		}
		file.flush();
		file.close();

		return make_pair(distance[endingNode],path[endingNode]);
	}


	pair<float,vector<unsigned long> > dijkstra(unsigned long startingNode, string filename){
		// For all nodes
		return dijkstra(startingNode, 0, filename);
	}

	pair<float,vector<unsigned long> > dijkstra(unsigned long startingNode, unsigned long endingNode){
		// For a specific node
		return dijkstra(startingNode, endingNode, "");
	}

	pair<float, vector<unsigned long> > path(unsigned long startingNode, string filename){
		// For all nodes
		if (weighted[0] == false){ // non-weighted graph: BFS-like algorithm
			vector<unsigned long> p = nonWeightedPath(startingNode, filename);
			return make_pair(p.size(),p);
		}
		else if (weighted[1] == false)
			return dijkstra (startingNode, filename); // weighted graph: Dijkstra algorithm
		else cout << "Weighted graph, but negative weights"; // we can't do anything
		return {};
	}

	pair<float, vector<unsigned long> > path(unsigned long startingNode, unsigned long endingNode){
		// For a specific node
		if (weighted[0] == false){ // non-weighted graph: BFS-like algorithm
			vector<unsigned long> p = nonWeightedPath(startingNode, endingNode);
			return make_pair(p.size(),p);
		}
		else if (weighted[1] == false)
			return dijkstra (startingNode, endingNode); // weighted graph: Dijkstra algorithm
		else cout << "Weighted graph, but negative weights"; // we can't do anything
		return {};
	}

};
#endif /* ADJACENCYMATRIX_H_ */
