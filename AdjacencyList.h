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

	bitset<2> weighted; // [0] to weighted graph; [1] to a negative weight

	AdjacencyList(){
		new (this) vector< set <pair <unsigned long, float> > > ( 0, set < pair <unsigned long, float> >());
	}

	AdjacencyList(unsigned long nodes){
		new (this) vector< set< pair <unsigned long, float> > >( nodes+1,  set <pair <unsigned long, float>>());
	}

	unsigned long getNodesNumber(){
		return size()-1;
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

	vector<unsigned long> nonWeightedPath(unsigned long startingNode, unsigned long endingNode, string filename){

		cout << ":: MINIMUM PATH USING ADJACENCY LIST ::" << endl;
		ofstream file ("pathl_"+filename, ifstream::out);

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
			for (set <pair <unsigned long, float> >::iterator it = at(node).begin(); it != at(node).end(); it++){
				if (flag[it->first] == false){
					flag[it->first] = true;
					searchStack.push(it->first);
					path[it->first] = path[node];
					path[it->first].push_back(it->first);
				}
			}
		}

		float end = clock()/CLOCKS_PER_SEC;

		ofstream operationsFile (OPERATIONSFILE_NAME, ifstream::app);
		if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
		else operationsFile << "pathnL:" << filename << ":" << end - start << endl;
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

	pair<float,vector<unsigned long> > dijkstra(unsigned long startingNode, unsigned long endingNode, string filename){

		cout << ":: DIJKSTRA USING ADJACENCY LIST ::" << endl;
		ofstream file ("dijkl_"+filename, ifstream::out);

		vector<float> distance (getNodesNumber()+1, numeric_limits<float>::infinity());

		list <pair<unsigned long,float*> > nodes; // V-S

		for (unsigned int i = 1; i <= getNodesNumber(); ++i) {
			if (i != startingNode) nodes.push_back(make_pair(i,&distance[i]));
		}
		distance[startingNode] = 0;
		nodes.push_front (make_pair(startingNode,&distance[startingNode]));
		file << "n: " << startingNode << "\troot" << endl;

		vector <vector<unsigned long> > path (getNodesNumber()+1, vector <unsigned long>(0));
		path[startingNode].push_back(startingNode);

		float start = clock()/CLOCKS_PER_SEC;

		while (nodes.size() > 0){
			pair<unsigned long,float*> node = *nodes.begin();
			nodes.pop_front();
			nodes.size()%500 == 0? cout << nodes.size() << " nodes remaining" << endl : cout << "";
			for (set <pair <unsigned long, float> >::iterator it = at(node.first).begin(); it != at(node.first).end(); it++){
					if (distance[it->first] > distance[node.first] + it->second){
						distance[it->first] = distance[node.first] + it->second;
						path[it->first] = path[node.first];
						path[it->first].push_back(it->first);
				}
			}
			nodes.sort(sortNodesByDistance);
		}

		float end = clock()/CLOCKS_PER_SEC;

		ofstream operationsFile (OPERATIONSFILE_NAME, ifstream::app);
		if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
		else operationsFile << "dijkL:" << filename << ":" << end - start << endl;
		operationsFile.flush();
		operationsFile.close();

		for (unsigned int i = 1; i <= getNodesNumber(); i++){
			if ((path[i].size() > 0)&&(i != startingNode)){
				file << "n: " << i <<"\tdist: " << distance[i] << "\tpath: " << path[i][0];
				for (unsigned int j = 1; j < path[i].size(); ++j) {
					file << "-" << path[i][j];
				}
				file << endl;
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
#endif /* ADJACENCYLIST_H_ */
