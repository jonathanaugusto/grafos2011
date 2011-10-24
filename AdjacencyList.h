/**
 * Universidade Federal do Rio de Janeiro
 * COS242 - Teoria dos Grafos
 * @descr	Trabalho pratico da disciplina
 * @author	Bruno Tomas / Jonathan Augusto
 */

#ifndef ADJACENCYLIST_H_
#define ADJACENCYLIST_H_

#pragma once

#include "Graph.h"

using namespace std;


/**
 * @brief Sorts connected components by size (decreasingly)
 */
bool sortBySize (set<unsigned long> set1, set<unsigned long> set2){
	if  (set1.size() > set2.size()) return true;
	return false;
}


/**
 * @brief Definition of as adjacency list.
 */
class AdjacencyList : public vector< set <pair <unsigned long, float> > > {


public:

	AdjacencyList(){
		new (this) vector< set <pair <unsigned long, float> > > ( 0, set < pair <unsigned long, float> >());
	}

	AdjacencyList(unsigned long nodes){
		new (this) vector< set< pair <unsigned long, float> > >( nodes+1,  set <pair <unsigned long, float>>());
	}

	/**
	 * @brief Test adjacency between two nodes using adjacency list.
	 * @param	this	Adjacency list.
	 * @param	node1	Label of a node.
	 * @param	node2 	Label of the other node.
	 */
	float testAdjacency (unsigned long node1, unsigned long node2){
		if (node1 == node2){
			cout << "Nodes are same" << endl;
			return false;
		}

		for (set< pair<unsigned long, float> >::iterator it = this->at(node1).begin(); it != this->at(node1).end(); it++){
			if (it->first == node2 && it->second != 0)
				return it->second;
			}
		return 0;

	}

	/**
	 * @brief Breadth-first search using adjacency list.
	 * Returns the connected component found.
	 * @param	this			Reference to adjacency list.
	 * @param	startingNode	Label of starting node.
	 * @param	filename		Filename where search tree will be written
	 * 							(it will be concatenated with "bfsl_")
	 */
	set <unsigned long> bfs(unsigned long startingNode, string filename){

		cout << ":: BFS USING ADJACENCY LIST ::" << endl;
		ofstream file ("bfsl_"+filename, ifstream::out);
		ofstream file2 ("majorlevel", ifstream::app);
		queue<pair<unsigned long, float>> searchQueue;
		unsigned long set[this->size()+1][3]; // [0] to dad node; [1] to node level; [2] to flag
		std::set <unsigned long> connected;
		pair<unsigned long, float> node, node2;

		float start = clock()/CLOCKS_PER_SEC;

		for (unsigned int i = 1; i <= this->size(); i++)
			set[i][2] = 0; // node flag

		cout << "Starting node: " << startingNode << endl;

		set[startingNode][2] = 1;
		set[startingNode][1] = 0;

		searchQueue.push(make_pair(startingNode,0));

		file << "n: " << startingNode << "\troot\tlevel: " << set[startingNode][1] << endl;

		while (!searchQueue.empty()){

			node = searchQueue.front();
			searchQueue.pop();
			connected.insert(node.first);

			for (std::set< pair<unsigned long, float> >::iterator it = this->at(node.first).begin(); it != this->at(node.first).end() ; it++){
				node2 = *it;

				if (set[node2.first][2] == 0){
					set[node2.first][2] = 1;
					searchQueue.push(node2);
					set[node2.first][0] = node.first;
					set[node2.first][1] = set[node.first][1] + 1;
				}
			}
		}

		float end = clock()/CLOCKS_PER_SEC;

		ofstream operationsFile (OPERATIONSFILE_NAME, ifstream::app);
		if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
		else operationsFile << "bfsL:" << end - start << endl;
		operationsFile.flush();
		operationsFile.close();

		unsigned int majorLevel = 0;
		for (unsigned int i = 1; i <= this->size() ; i++)
			if ((set[i][2] == 1)&&(set[i][1] > 0)){
				file << "n: " << i <<"\tdad: " << set[i][0] <<  "\tlevel: " << set[i][1] << endl;
				if (set[i][1] > majorLevel) majorLevel = set[i][1];
			}

		file2 << "initial: " << startingNode << " | major level: " << majorLevel << endl;

		file.flush();
		file.close();
		file2.flush();
		file2.close();
		return connected;
	}

	/**
	 * @brief Depth-first search using adjacency list.
	 * Returns the connected component found.
	 * @param	this			Reference to adjacency lsit.
	 * @param	startingNode	Label of starting node.
	 * @param	filename		Filename where search tree will be written
	 * 							(it will be concatenated with "dfsl_")
	 */
	set <unsigned long> dfs(unsigned long startingNode, string filename){

		cout << ":: DFS USING ADJACENCY LIST ::" << endl;
		ofstream file ("dfsl_"+filename, ifstream::out);
		stack<pair<unsigned long, float> > searchStack;
		unsigned long set[this->size()+1][3]; // [0] to dad node; [1] to node level; [2] to flag
		std::set<unsigned long> connected;
		pair<unsigned long, float> node, node2;

		float start = clock()/CLOCKS_PER_SEC;

		for (unsigned int i = 1; i <= this->size(); i++)
			set[i][2] = 0; // node flag

		cout << "Starting node: " << startingNode << endl;

		set[startingNode][1] = 0;
		searchStack.push(make_pair(startingNode,0));

		file << "n: " << startingNode << "\troot\tlevel: " << set[startingNode][1] << endl;

		while (!searchStack.empty()){

			node = searchStack.top();
			searchStack.pop();
			connected.insert(node.first);

			if (set[node.first][2] == 0){

				set[node.first][2] = 1;

				for (std::set<pair <unsigned long, float> >::iterator it = this->at(node.first).begin(); it != this->at(node.first).end() ; it++){
					node2 = *it;

						searchStack.push(node2);
						if(set[node2.first][2] == 0){
							set[node2.first][0] = node.first;
							set[node2.first][1] = set[node.first][1] + 1;
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

		for (unsigned int i = 1; i <= this->size() ; i++)
			if ((set[i][2] == 1)&&(set[i][1] > 0))
				file << "n: " << i <<"\tdad: " << set[i][0] <<  "\tlevel: " << set[i][1] << endl;

		file.flush();
		file.close();


		return connected;
	}

	/**
	 * @brief Found all connected component in graph using adjacency list.
	 * @param	this		Adjacency list.
	 * @param	filename	Filename where search tree will be written
	 * 							(it will be concatenated with "dfsm_")
	 */
	void foundConnectedComponents (string filename){

		cout << ":: FOUND CONNECTED COMPONENTS ::" << endl;
		ofstream file ("cc_"+filename, ifstream::out);
		string fn;
		set<unsigned long> component{};
		list <set <unsigned long> > sortedComponents;

		float start = clock()/CLOCKS_PER_SEC;

		for(unsigned long i = 1; i <= this->size(); i++){
			for (set<unsigned long>::iterator it = component.find(i); i == *it; i++, *it++);
			if (i >= this->size()-1) break;
			component = bfs(i,filename);
			sortedComponents.push_back(component);
			sortedComponents.sort(sortBySize);
			sortedComponents.unique();
			if (!(sortedComponents.size() % 200)) cout << sortedComponents.size() <<" components found\n";
		}
		cout << sortedComponents.size() <<" component";
		sortedComponents.size() > 1 ? cout << "s" : cout << "";
		cout << " found\n";


		float end = clock()/CLOCKS_PER_SEC;

		ofstream operationsFile (OPERATIONSFILE_NAME, ifstream::app);
		if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
		else operationsFile << "ccL:" << end - start << endl;
		operationsFile.flush();
		operationsFile.close();

		cout << "Writing file..." << endl;

		file << sortedComponents.size() << endl;

		for (list <set <unsigned long> >::iterator it = sortedComponents.begin(); it != sortedComponents.end(); it++){
			file << "[size=" << it->size() << "]";
			for (set <unsigned long>::iterator it2 = it->begin(); it2 != it->end(); it2++)
				file << " " << *it2;
			file << endl;
		}

		file.flush();
		file.close();
	}




};
#endif /* ADJACENCYLIST_H_ */
