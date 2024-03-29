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

bool sortNodesByDistance (pair <unsigned long, float*> n1, pair <unsigned long, float*> n2){
		if (*(n1.second) < *(n2.second)) return true;
		else if (*(n1.second) == *(n2.second)){
			if (n1.first < n2.first) return true;
			return false;
		}
		return false;
	}

ostream& operator<< (ostream& out, vector<unsigned long> v){
	out << v[0];
	for (unsigned int i = 1; i < v.size(); ++i) {
		out << "-" << v[i];
	}
	return out;
}

/**
 * @brief Definition of an adjacency matrix.
 */
class AdjacencyMatrix : public std::vector< vector<float> > {

	public:

		bitset<2> weighted; // [0] to weighted graph; [1] to a negative weight

		unsigned long edges;

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

		unsigned long getEdgesNumber(){
			return edges;
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
				boost::thread *thread = new boost::thread(boost::bind(&AdjacencyMatrix::parallelDijkstra,this,(getNodesNumber()/num_threads)*i+1,(getNodesNumber()/num_threads)*(i+1),filename));
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
				ifstream file ("dijkm"+s+"_"+filename, ifstream::in);

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

			edges = edges_n;
			cout << "Created " << edges_n << " edges in adjacency matrix" << endl;

			clock_t end = clock();

			ofstream operationsFile (OPERATIONSFILE_NAME, ifstream::app);
			if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
			else operationsFile << "buildFM:" << filename << ":" << (float)((end - start)/CLOCKS_PER_SEC) << ":" << size()*sizeof(unsigned long) << endl;
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
			else operationsFile << "pathnM:" << filename << ":" << end - start << endl;
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

			//cout << ":: DIJKSTRA USING ADJACENCY MATRIX ::" << endl;

			vector<float> distance (getNodesNumber()+1, numeric_limits<float>::infinity());
			list <pair<unsigned long,float*> > nodes; // V-S
			vector <vector<unsigned long> > path (getNodesNumber()+1, vector <unsigned long>(0));

			if (filename != "") {
					stringstream ss; string s;
					ss << startingNode; s = ss.str();
					ifstream file ("dijkm"+s+"_"+filename);
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
				for (unsigned int i = 1; i <= getNodesNumber(); i++){
					if (getAdjacency (node.first, i)!= 0){
						if (distance[i] > distance[node.first] + getAdjacency (node.first, i)){
							distance[i] = distance[node.first] + getAdjacency (node.first, i);
							path[i] = path[node.first];
							path[i].push_back(i);
						}
					}
				}
				nodes.sort(sortNodesByDistance);
			}

			float end = clock()/CLOCKS_PER_SEC;

			ofstream operationsFile (OPERATIONSFILE_NAME, ifstream::app);
			if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
			else operationsFile << "dijkM:" << filename << ":" << end - start << endl;
			operationsFile.flush();
			operationsFile.close();

			if (filename != "") {
				stringstream ss; string s;
				ss << startingNode; s = ss.str();
				ofstream file ("dijkm"+s+"_"+filename, ofstream::out);
				file << startingNode << endl;
				for (unsigned int i = 1; i <= getNodesNumber(); i++){
					if ((path[i].size() > 0)&&(i != startingNode)){
						file << i <<" " << distance[i] << " " << path[i][0];
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
			AdjacencyMatrix adjm (getNodesNumber());

			for (unsigned int i = 1; i <= getNodesNumber(); i++)
				for (unsigned int j = i+1; j <= getNodesNumber()-1; j++)
					adjm.setAdjacency (i,j,0);

			// Set represents border edges (sorted by weight)
			// pair <unsigned long, unsigned long> represents nodes (first < second);
			// float represents edge weight.
			set <pair< pair<unsigned long, unsigned long> ,float>,AdjacencyMatrix::compareEdges> cut;

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
					adjm.setAdjacency (edge.first.first, edge.first.second, edge.second);
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
			adjm.edges = edges_n;

			float end = clock()/CLOCKS_PER_SEC;
			ofstream operationsFile (OPERATIONSFILE_NAME, ofstream::app);
			if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
			else operationsFile << "primG:" << filename << ":" << end - start << endl;
			operationsFile.flush();
			operationsFile.close();

			float total = .0;
			for (unsigned int i = 1; i <= adjm.getNodesNumber()-1; i++)
				for (unsigned int j = i+1; j <= adjm.getNodesNumber(); j++)
					total += adjm.getAdjacency(i,j);

			cout << "MST weight: " << total << endl;
			cout << "Edges: " << adjm.getEdgesNumber() << endl;

			if (filename != "") {
				ofstream file ("primm_"+filename, ofstream::out);
				file << total << endl;
				for (unsigned int i = 1; i <= adjm.getNodesNumber()-1; i++)
						for (unsigned int j = i+1; j <= adjm.getNodesNumber(); j++)
							if (adjm.getAdjacency(i,j) != 0)
						file << i << " " << j << " " << adjm.getAdjacency(i,j) << endl;
				file.flush();
				file.close();
			}

		}

	};

#endif /* ADJACENCYMATRIX_H_ */
