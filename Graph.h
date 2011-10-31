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

using namespace boost;
using boost::thread;
using boost::bind;
using boost::thread_group;

void parallelDijkstra (unsigned int, unsigned int, string);

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
	 	 * space for a given numbers of nodes.
	 	 * @param	nodes_n	Number of nodes.
	 	 */
		Graph (unsigned long nodes_n){
			g_nodes.reserve(nodes_n+1);
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
		float* getEmpiricalDegreeDistribution(){

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
		 * @brief Calculates and returns
		 * medium distance.
		 *
		 */
		float getMediumDistance(){

			float m_distance = 0;

			for (unsigned int i = 1; i <= getNodesNumber(); i++)
				for (unsigned int j = i+1; j <= getNodesNumber(); j++) {
					m_distance += path(i,j).first;
				}

			return (m_distance / ((getNodesNumber()*(getNodesNumber()-1))/2));
		}

		Graph &operator= (Graph graph){
			g_edges = graph.g_edges;
			g_nodes = graph.g_nodes;
			return *this;
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

		void parallelDijkstra (unsigned int first_node, unsigned int last_node, string filename){
			Graph g;
			for (unsigned int i = first_node; i <= last_node; i++){
				g = *this;
				g.dijkstra(i, filename);
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

			cout << "Running Dijkstra.";
			for (unsigned int i = 1; i <= getNodesNumber(); i++){
				Graph g = *this;
				g.dijkstra(i, filename);
			}

			/*unsigned int num_threads = 0;
			if(getNodesNumber() > MAX_THREADS) num_threads = MAX_THREADS - getNodesNumber()%MAX_THREADS;
			else num_threads = getNodesNumber();

			cout << "Running Dijkstra with " << num_threads << " threads.";

			boost::thread_group threads;
			for (unsigned int i = 0; i < MAX_THREADS; i++){
				Graph g = *this;
				boost::thread *thread = new boost::thread(boost::bind(&Graph::parallelDijkstra,g,(getNodesNumber()/num_threads)*i+1,(getNodesNumber()/num_threads)*(i+1),filename));
				threads.add_thread(thread);
			}
			threads.join_all();*/

			cout << " Done!" << endl;

			cout << "Retrieving Dijkstra files.";
			for (unsigned int i = 1; i <= getNodesNumber(); i++){
				stringstream ss;
				string s;
				ss << i;
				s = ss.str();
				ifstream file ("dijkg"+s+"_"+filename, ifstream::in);

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
		 * @brief Build graph from description file
		 * (Line with number of nodes and then each line
		 * with an edge)
		 * @param filename	String containing filename.
		 */
		void build(string filename){

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
				if (file.eof()) break;
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

			ofstream file ("info_"+filename, ofstream::out);
			file << "# n = " << getNodesNumber() << endl;
			file << "# m = " << getEdgesNumber() << endl;
			file << "# d_medio = " << getMediumDegree() << endl;

			float *nodeDegrees = getEmpiricalDegreeDistribution();

			for (unsigned long i = 0; i < getNodesNumber()-1; i++)
				file << i << " " << nodeDegrees[i] << endl;

			file.flush();
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
			ofstream file ("bfsg_"+filename, ofstream::out);

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
				list <Node *> connectedNodes = node->getConnectedNodes();
				for (list<Node *>::iterator it=connectedNodes.begin(); it!=connectedNodes.end(); it++){
					Node *node2 = *it;
					if (node2->test() == false){
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
			else operationsFile << "bfsG:" << end - start << endl;
			operationsFile.flush();
			operationsFile.close();

			for (unsigned int i = 1; i < g_nodes.size(); i++)
				if (nodeLevels[i] > 0)
					file << "n: " << i <<"\tdad: " << dadNode[i]->label <<  "\tlevel: " << nodeLevels[i] << endl;

			file.flush();
			file.close();

			for (unsigned int i = 1; i <= g_nodes.size(); ++i) {
				g_nodes[i].unflag();
			}

			return connected;

		}

		vector<Node *> nonWeightedPath(unsigned long startingNode, unsigned long endingNode, string filename){

			cout << ":: MINIMUM PATH USING GRAPH ::" << endl;

			queue<Node *> searchStack;
			vector< vector <Node*> > path (g_nodes.size(), {});

			float start = clock()/CLOCKS_PER_SEC;

			cout << "Starting node: " << startingNode << "; Ending node: " << endingNode << endl;
			searchStack.push(&g_nodes[startingNode]);
			g_nodes[startingNode].flag();
			path[startingNode].push_back(&g_nodes[startingNode]);

			while (!searchStack.empty()){
				Node *node = searchStack.front();
				searchStack.pop();
				list <Node *> connectedNodes = node->getConnectedNodes();
				for (list<Node *>::iterator it=connectedNodes.begin(); it!=connectedNodes.end(); it++){
					Node *node2 = *it;
					if (node2->test() == false){
						node2->flag();
						searchStack.push(node2);
						path[node2->label] = path[node->label];
						path[node2->label].push_back(node2);

					}
				}
			}

			float end = clock()/CLOCKS_PER_SEC;

			ofstream operationsFile (OPERATIONSFILE_NAME, ifstream::app);
			if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
			else operationsFile << "pathnG:" << filename << ":" << end - start << endl;
			operationsFile.flush();
			operationsFile.close();

			if (filename != "") {
				ofstream file ("pathg_"+filename, ofstream::out);
				file << "n: " << startingNode << "\troot" << endl;
				for (unsigned int i = 1; i < path.size(); i++){
					if ((path[i].size() > 1) && (i != startingNode)){
						file << "n: " << i << "\tpath: " << path[i][0]->label;
						for (unsigned int j = 1; j < path[i].size(); ++j) {
							file << "-" << path[i][j]->label;
						}
						file << endl;
					}
				}
				file.flush();
				file.close();
			}

			for (unsigned int i = 1; i <= g_nodes.size(); ++i) {
				g_nodes[i].unflag();
			}

			return {};

		}

		vector<Node *> nonWeightedPath(unsigned long startingNode, string filename){
			// For all nodes
			return nonWeightedPath(startingNode, 0, filename);
		}

		vector<Node *> nonWeightedPath(unsigned long startingNode, unsigned long endingNode){
			// For a specific node
			return nonWeightedPath(startingNode, endingNode, "");
		}


		pair<float, vector<Node *> > dijkstra(unsigned long startingNode, unsigned long endingNode, string filename){

			//cout << ":: DIJKSTRA USING GRAPH ::" << endl;


			static vector<float> distances (getNodesNumber()+1, numeric_limits<float>::infinity());
			vector <vector<Node *> > path (g_nodes.size()+1, {});
			list <Node *> nodes; // V-S

			struct sortByDistance{
				bool operator() (Node *node1, Node *node2){
					if (distances[node1->label] < distances[node2->label]) return true;
					else if (distances[node1->label] == distances[node2->label])
						if (node1->label < node2->label) return true;
						else return false;
					else return false;
				}
			};

			if (filename != "") {
					stringstream ss; string s;
					ss << startingNode; s = ss.str();
					ifstream file ("dijkg"+s+"_"+filename);
					if (file) {
						file.close();
						return make_pair(distances[endingNode],path[endingNode]);
					}
			}

			float start = clock()/CLOCKS_PER_SEC;

			for (unsigned int i = 1; i <= getNodesNumber(); ++i) {
				nodes.push_back (&g_nodes[i]);
			}

			Node *starting = &g_nodes[startingNode];
			distances[starting->label] = 0;
			path[starting->label].push_back(starting);
			while (nodes.size() > 0){
				Node *node = *nodes.begin();
				nodes.pop_front();
				for (set<Edge *>::iterator it=node->edges.begin(); it!=node->edges.end(); it++){
					Node *node2;
					(**it).from->label == node->label? node2 = (**it).to : node2 = (**it).from;
					if (distances[node2->label] > distances[node->label] + (**it).weight){
						distances[node2->label] = distances[node->label] + (**it).weight;
						path[node2->label] = path[node->label];
						path[node2->label].push_back(node2);
					}
				}
				nodes.sort(sortByDistance());
			}

			float end = clock()/CLOCKS_PER_SEC;
			ofstream operationsFile (OPERATIONSFILE_NAME, ifstream::app);
			if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
			else {
				if(filename != "") 	operationsFile << "dijkG:" << filename << ":" << end - start << endl;
			}
			operationsFile.flush();
			operationsFile.close();

			if (filename != "") {
				stringstream ss; string s;
				ss << startingNode; s = ss.str();
				ofstream file ("dijkg"+s+"_"+filename, ofstream::out);
				file << startingNode << endl;
				for (unsigned int i = 1; i <= getNodesNumber(); i++){
					if ((path[i].size() > 0)&&(i != startingNode)){
						file << i << " " << distances[i] << " " << path[i][0]->label;
						for (unsigned int j = 1; j < path[i].size(); ++j) {
							file << "-" << path[i][j]->label;
						}
						file << endl;
					}
				}
				file.flush();
				file.close();
			}

			return make_pair(distances[endingNode],path[endingNode]);
		}



		pair<float, vector<Node *> > dijkstra(unsigned long startingNode, string filename){
			// For all nodes
			cout << ".";
			return dijkstra(startingNode, 0, filename);
		}

		pair<float, vector<Node *> > dijkstra(unsigned long startingNode, unsigned long endingNode){
			// For a specific node
			return dijkstra(startingNode, endingNode, "");
		}

		pair<float, vector<Node *> > path(unsigned long startingNode, string filename){
			// For all nodes
			if ((g_edges.begin()->weight == 1) && ((--g_edges.end())->weight == 1)){ // non-weighted graph: BFS-like algorithm
				vector <Node *> p = nonWeightedPath(startingNode, filename);
				return make_pair(p.size(),p);
			}
			else if (g_edges.begin()->weight > 0)
				return dijkstra (startingNode, filename); // weighted graph: Dijkstra algorithm
			else cout << "Weighted graph, but negative weights"; // we can't do anything
			return {};
		}

		pair<float, vector<Node *> > path(unsigned long startingNode, unsigned long endingNode){
			// For a specific node
			if ((g_edges.begin()->weight == 1) && ((--g_edges.end())->weight == 1)){ // non-weighted graph: BFS-like algorithm
				vector <Node *> p = nonWeightedPath(startingNode, endingNode);
				return make_pair(p.size(),p);
			}
			else if (g_edges.begin()->weight > 0)
				return dijkstra (startingNode, endingNode); // weighted graph: Dijkstra algorithm
			else cout << "Weighted graph, but negative weights"; // we can't do anything
			return {};
		}

		void prim (unsigned long startingNode, string filename){

			cout << ":: PRIM USING GRAPH ::" << endl;

			float start = clock()/CLOCKS_PER_SEC;

			// New Graph object represents set of edges and nodes already verified.
			Graph graph(getNodesNumber());
			graph.g_nodes.reserve(getNodesNumber());

			// Set represents border edges (sorted by weight)
			set<Edge *,Edge::comparePointers> cut;

			// Initialization: all node costs are infinity, and all edges are unmarked.
			for (unsigned int i = 1; i <= getNodesNumber(); ++i) {
				g_nodes[i].distance = numeric_limits<float>::infinity();
			}
			for (set <Edge>::iterator it = this->g_edges.begin(); it != this->g_edges.end(); it++){
				Edge edge = *it;
				edge.unflag();
			}

			// Initialization: starting node is marked, and all its edges are added to border.
			Node *starting = &g_nodes[startingNode];
			starting->flag();
			for (set <Edge *>::iterator it = starting->edges.begin(); it != starting->edges.end(); it++) {
				Edge *edge = *it;
				edge->flag();
				cut.insert(edge);
			}

			unsigned long nodes_n = 1;

			while (nodes_n <= getNodesNumber()){

				// Catch lightest border edge (first at ordered set)
				Edge *edge = *(cut.begin());
				cut.erase(cut.begin());

				// If it's still a border edge, add it at graph
				if ((edge->from->test() xor edge->to->test()))
				graph.g_edges.insert(*edge);

				// Go over border through this edge.
				// If that node is marked, it's already inside.
				Node* node2;
				edge->to->test() == true? node2 = edge->from : node2 = edge->to ;

				if (node2->distance > edge->weight){
					node2->flag();
					node2->distance = edge->weight;
					nodes_n++;

					for (set <Edge *>::iterator it = node2->edges.begin(); it != node2->edges.end(); ++it) {
						Edge *edge = *it;

						if (edge->test() == false) {
							edge->flag();
							cut.insert(edge);
						}
					}
				}
				if (nodes_n == getNodesNumber()) break;
			}

			float end = clock()/CLOCKS_PER_SEC;
			ofstream operationsFile (OPERATIONSFILE_NAME, ofstream::app);
			if (operationsFile.fail()) cout << "Error writing function infos :(" << endl;
			else operationsFile << "primG:" << filename << ":" << end - start << endl;
			operationsFile.flush();
			operationsFile.close();

			float total = .0;
			for (set<Edge>::iterator it =  graph.g_edges.begin(); it != graph.g_edges.end(); it++)
					total += (*it).weight;
			cout << "MST weight: " << total << endl;
			cout << "Edges: " << graph.getEdgesNumber() << endl;

			if (filename != "") {
				ofstream file ("primg_"+filename, ofstream::out);
				file << total << endl;
				for (set<Edge>::iterator it =  graph.g_edges.begin(); it != graph.g_edges.end(); it++)
						file << (*it).from->label << " " << (*it).to->label << " " << (*it).weight << endl;
				file.flush();
				file.close();
			}

			for (unsigned int i = 1; i <= g_nodes.size(); ++i) {
				g_nodes[i].unflag();
			}
		}
};

#endif
