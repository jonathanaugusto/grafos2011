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
		return this->at(node1-1).at(node2-node1-1);
	}



	/**
	 * @brief Builds and returns adjacency matrix from description file
	 * (Line with number of nodes and then each line
	 * with an edge)
	 * @param filename	String containing filename.
	 */
	AdjacencyMatrix buildAdjacencyMatrix(string filename){

		cout << ":: CREATE ADJACENCY MATRIX ::" << endl;

		unsigned long nodes_n, node1, node2, weight;

		ifstream file (filename, ifstream::in);
		if (file.fail()) cout << "Read error :(" << endl;

		clock_t start = clock();

		file >> nodes_n;

		cout << "File: " << filename << endl;

		AdjacencyMatrix adjm( nodes_n );

		while (!file.eof()){
			file >> node1;
			file >> node2;
			file >> weight;
			adjm.setAdjacency(node1,node2,weight);
		}

		clock_t end = clock();

		ofstream operationsFile (OPERATIONSFILE_NAME, ifstream::app);
		if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
		else operationsFile << "buildFM:" << (float)((end - start)/CLOCKS_PER_SEC) << ":" << adjm.size()*sizeof(unsigned long) << endl;
		operationsFile.flush();
		operationsFile.close();

		cout << endl;



		file.close();

		return adjm;
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

		for (unsigned int i = 1; i <= size(); i++){
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

			for (unsigned int i = 1; i <= size(); i++){
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

		for (unsigned int i = 1; i <= size(); i++)
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
				for (unsigned int i = 1; i <= size() ; i++){
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

		for (unsigned int i = 1; i <= size(); i++)
			if ((set[i-1][2] == 1)&&(set[i-1][1] > 0))
				file << "n: " << i <<"\tdad: " << set[i-1][0] <<  "\tlevel: " << set[i-1][1] << endl;

		file.flush();
		file.close();


		return connected;

	}

};
#endif /* ADJACENCYMATRIX_H_ */
