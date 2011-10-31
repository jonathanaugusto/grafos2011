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
	unsigned long edges;

	AdjacencyList(){
		new (this) vector< set <pair <unsigned long, float> > > ( 0, set < pair <unsigned long, float> >());
	}

	AdjacencyList(unsigned long nodes){
		new (this) vector< set< pair <unsigned long, float> > >( nodes+1,  set <pair <unsigned long, float>>());
	}

	unsigned long getNodesNumber(){
		return size()-1;
	}

	unsigned long getEdgesNumber(){
		return edges;
	}

	/**
	 * @brief Test adjacency between two nodes using adjacency list.
	 * @param	this	Adjacency list.
	 * @param	node1	Label of a node.
	 * @param	node2 	Label of the other node.
	 */
	float getAdjacency (unsigned long node1, unsigned long node2){
		if (node1 == node2)
			return 0;

		for (set< pair<unsigned long, float> >::iterator it = this->at(node1).begin(); it != this->at(node1).end(); it++){
			if (it->first == node2 && it->second != 0)
				return it->second;
			}
		return 0;
	}

	void setAdjacency (unsigned long node1, unsigned long node2, float weight){
		if (node1 == node2) return;
		this->at(node1).insert(make_pair(node2,weight));
		this->at(node2).insert(make_pair(node1,weight));
	}

	void parallelDijkstra (unsigned int first_node, unsigned int last_node, string filename){
		for (unsigned int i = first_node; i <= last_node; i++){
			dijkstra(i, filename);
		}
	}

	/**
	 * @brief Calculates and returns empirical
	 * distribution for distances between node pairs.
	 *
	 */
	pair<map<float,float>,float> getEmpiricalAndMedium(string filename){

		map<float,float> distances;
		float medium = .0;

		/*cout << "Running Dijkstra...";
		for (unsigned int i = 1; i <= getNodesNumber(); i++){
			dijkstra(i, filename);
		}
		cout << endl;*/

		unsigned int num_threads = 0;
		if(getNodesNumber() > MAX_THREADS) num_threads = MAX_THREADS - getNodesNumber()%MAX_THREADS;
		else num_threads = getNodesNumber();

		cout << "Running Dijkstra with " << num_threads << " threads.";

		boost::thread_group threads;
		for (unsigned int i = 0; i < num_threads; i++){
			boost::thread *thread = new boost::thread(boost::bind(&AdjacencyList::parallelDijkstra,this,(getNodesNumber()/num_threads)*i+1,(getNodesNumber()/num_threads)*(i+1),filename));
			threads.add_thread(thread);
		}
		threads.join_all();

		cout << " Done!" << endl;

		cout << "Retrieving Dijkstra files.";
		for (unsigned int i = 1; i <= getNodesNumber(); i++){
			stringstream ss;
			string s;
			ss << i;
			s = ss.str();
			ifstream file ("dijkl"+s+"_"+filename, ifstream::in);

			unsigned int start, end;
			float distance;
			string path;

			file >> start;
			while (!file.eof()){
				file >> end;
				if (file.eof()) break;
				file >> distance;
				file >> path;
				//	cout << end << " " << distance << " " << path << endl;
				distances[distance]++;
				medium += distance;
			}
			file.close();
			cout << ".";
		}

		cout << " Done!" << endl;

		medium /= (getNodesNumber()*(getNodesNumber()-1)/2);
		medium /= 2;

		for (unsigned int i = 0; i < distances.size(); i++){
			distances[i] /= (getNodesNumber()*(getNodesNumber()-1)/2);
			distances[i] /= 2;
		}

		return make_pair(distances,medium);
	}


	void build(string filename){

		cout << ":: CREATE ADJACENCY LIST ::" << endl;

		unsigned long nodes_n, node1, node2, weight, edges_n;

		ifstream file (filename, ifstream::in);
		if (file.fail()) cout << "Read error :(" << endl;

		clock_t start = clock();

		file >> nodes_n;

		cout << "File: " << filename << endl;

		new (this) AdjacencyList (nodes_n);

		cout << "Created " << nodes_n << " nodes in adjacency list" << endl;

		edges_n = 0;
		while (!file.eof()){
			file >> node1;
			file >> node2;
			if (file.eof()) break;
			file >> weight;
			if (weight != 1) weighted[0] = true;
			if (weight < 0) weighted[1] = true;
			at(node1).insert(make_pair(node2,weight));
			at(node2).insert(make_pair(node1,weight));
			edges_n++;
		}

		edges = edges_n;
		cout << "Created " << edges_n << " edges in adjacency list" << endl;


		clock_t end = clock();

		ofstream operationsFile (OPERATIONSFILE_NAME, ifstream::app);
		if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
		else operationsFile << "buildFL:" << filename << ":" << (float)((end - start)/CLOCKS_PER_SEC) << ":" << size()*sizeof(unsigned long) << endl;
		operationsFile.flush();
		operationsFile.close();

		file.close();
	}

	void buildInformationFile2 (string filename){

		ofstream file ("info2_"+filename, ofstream::out);
		file << "# n = " << getNodesNumber() << endl;
		file << "# m = " << getEdgesNumber() << endl;

		//file << "# d_media = " << getMediumDistance() << endl;

		float start = clock()/CLOCKS_PER_SEC;

		pair<map<float,float>,float> pair = getEmpiricalAndMedium(filename);
		file << "# d_med = " << pair.second << endl;

		for (unsigned long i = 0; i < pair.first.size(); i++)
			file << i << " " << pair.first[i] << endl;

		float end = clock()/CLOCKS_PER_SEC;

		ofstream operationsFile (OPERATIONSFILE_NAME, ifstream::app);
		if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
		else operationsFile << "info2:" << filename << ":" << end - start << endl;
		operationsFile.flush();
		operationsFile.close();

		file.flush();
		file.close();

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

		//cout << ":: DIJKSTRA USING ADJACENCY LIST ::" << endl;

		vector<float> distance (getNodesNumber()+1, numeric_limits<float>::infinity());
		list <pair<unsigned long,float*> > nodes; // V-S
		vector <vector<unsigned long> > path (getNodesNumber()+1, vector <unsigned long>(0));

		if (filename != "") {
				stringstream ss; string s;
				ss << startingNode; s = ss.str();
				ifstream file ("dijkl"+s+"_"+filename);
				if (file) {
					file.close();
					return make_pair(distance[endingNode],path[endingNode]);
				}
		}

		for (unsigned int i = 1; i <= getNodesNumber(); ++i) {
			if (i != startingNode) nodes.push_back(make_pair(i,&distance[i]));
		}
		distance[startingNode] = 0;
		nodes.push_front (make_pair(startingNode,&distance[startingNode]));
		path[startingNode].push_back(startingNode);

		float start = clock()/CLOCKS_PER_SEC;

		while (nodes.size() > 0){
			pair<unsigned long,float*> node = *nodes.begin();
			nodes.pop_front();
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

		if (filename != "") {
			stringstream ss; string s;
			ss << startingNode; s = ss.str();
			ofstream file ("dijkl"+s+"_"+filename, ofstream::out);
			file << startingNode << endl;
			for (unsigned int i = 1; i <= getNodesNumber(); i++){
				if ((path[i].size() > 0)&&(i != startingNode)){
					file << i << " " << distance[i] << " " << path[i][0];
					for (unsigned int j = 1; j < path[i].size(); ++j) {
						file << "-" << path[i][j];
					}
					file << endl;
				}
			}
			file.flush();
			file.close();
		}

		return make_pair(distance[endingNode],path[endingNode]);
	}

	pair<float,vector<unsigned long> > dijkstra(unsigned long startingNode, string filename){
		// For all nodes
		cout << ".";
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

	struct compareEdges{
		bool operator() (pair< pair<unsigned long, unsigned long>,float> p1, pair< pair<unsigned long, unsigned long>,float> p2){
			if (p1.second < p2.second) return true;
			else if (p1.second == p2.second)
				if (p1.first < p2.first) return true;
				else return false;
			else return false;
		}
	};

	void prim (unsigned long startingNode, string filename){

		cout << ":: PRIM USING ADJACENCY MATRIX ::" << endl;

		float start = clock()/CLOCKS_PER_SEC;

		// New AdjacencyMatrix represents set of edges and nodes already verified.
		AdjacencyList adjl (getNodesNumber());

		for (unsigned int i = 1; i <= getNodesNumber(); i++)
			for (unsigned int j = i+1; j <= getNodesNumber()-1; j++)
				adjl.setAdjacency (i,j,0);

		// Set represents border edges (sorted by weight)
		// pair <unsigned long, unsigned long> represents nodes (first < second);
		// float represents edge weight.
		set <pair< pair<unsigned long, unsigned long> ,float>,AdjacencyList::compareEdges> cut;

		// Initialization: all node costs are infinity, and all edges and nodes are unmarked.
		vector<float> distance (getNodesNumber()+1, numeric_limits<float>::infinity());
		map<pair<unsigned long, unsigned long>,bool> edgeFlag;
		vector<bool> nodeFlag (getNodesNumber()+1, false);

		// Initialization: starting node is marked, and all its edges are added to border.
		nodeFlag[startingNode] = true;
		for (unsigned int i = 1; i <= getNodesNumber(); i++) {
			float w = getAdjacency(startingNode,i);
			if (w != 0){
				if (i < startingNode){
					edgeFlag [make_pair(i,startingNode)] = true;
					cut.insert (make_pair(make_pair(i,startingNode),w));
				}
				else{
					edgeFlag [make_pair(startingNode,i)] = true;
					cut.insert (make_pair(make_pair(startingNode,i),w));
				}
			}
		}

		unsigned long nodes_n = 1;
		unsigned long edges_n = 0;

		while (nodes_n <= getNodesNumber()){

			// Catch lightest border edge (first at ordered set)
			pair< pair<unsigned long, unsigned long> ,float> edge = *cut.begin();
			cut.erase(cut.begin());

			// If it's still a border edge, add it at adjacency matrix
			if ((nodeFlag[edge.first.first] xor nodeFlag[edge.first.second])){
				adjl.setAdjacency (edge.first.first, edge.first.second, edge.second);
				edges_n++;
			}
			// Go over border through this edge.
			// If that node is marked, it's already inside.
			unsigned long node2;
			nodeFlag[edge.first.second] == true? node2 = edge.first.first : node2 = edge.first.second ;

			if ((nodeFlag[node2] == false) && (distance[node2] > edge.second)){
				nodeFlag[node2] = true;
				distance[node2] = edge.second;
				nodes_n++;

				for (unsigned int i = 1; i <= getNodesNumber(); i++) {
					if (getAdjacency(node2,i) != 0){
						if ((i < node2) && (edgeFlag [make_pair(i,node2)] == false)){
							edgeFlag [make_pair(i,node2)] = true;
							cut.insert (make_pair(make_pair(i,node2),getAdjacency(i,node2)));
						}
						else if ((i > node2) && (edgeFlag [make_pair(node2,i)] == false)){
							edgeFlag [make_pair(node2,i)] = true;
							cut.insert (make_pair(make_pair(node2,i),getAdjacency(node2,i)));
						}
					}
				}
			}

			cout << "cut = ";
			for (set <pair< pair<unsigned long, unsigned long> ,float> >::iterator it = cut.begin(); it != cut.end(); it++)
				cout << it->first.first << "-" << it->first.second << "(" << it->second << ") | ";
			cout << endl;

			if (nodes_n == getNodesNumber()) break;
		}
		adjl.edges = edges_n;

		float end = clock()/CLOCKS_PER_SEC;
		ofstream operationsFile (OPERATIONSFILE_NAME, ofstream::app);
		if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
		else operationsFile << "primG:" << filename << ":" << end - start << endl;
		operationsFile.flush();
		operationsFile.close();

		float total = .0;
		for (unsigned int i = 1; i <= adjl.getNodesNumber()-1; i++)
			for (unsigned int j = i+1; j <= adjl.getNodesNumber(); j++)
				total += adjl.getAdjacency(i,j);

		cout << "MST weight: " << total << endl;
		cout << "Edges: " << adjl.getEdgesNumber() << endl;

		if (filename != "") {
			ofstream file ("priml_"+filename, ofstream::out);
			file << total << endl;
			for (unsigned int i = 1; i <= adjl.getNodesNumber()-1; i++)
					for (unsigned int j = i+1; j <= adjl.getNodesNumber(); j++)
						if (adjl.getAdjacency(i,j) != 0)
					file << i << " " << j << " " << adjl.getAdjacency(i,j) << endl;
			file.flush();
			file.close();
		}

	}

};
#endif /* ADJACENCYLIST_H_ */
