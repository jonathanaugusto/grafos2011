#ifndef GRAPH_H
#define GRAPH_H

#include "Includes.h"

typedef struct vector< vector<bool> > AdjacencyMatrix;
typedef struct vector< set<unsigned long long> > AdjacencyList;

using namespace rel_ops;
using namespace std;

bool sortBySize (set<unsigned long long> set1, set<unsigned long long> set2){
	if  (set1.size() > set2.size()) return true;
	return false;
}


class Graph{

	public:
		vector<Node> g_nodes;
		vector<Edge> g_edges;
		AdjacencyMatrix *adjMatrix;
		AdjacencyList *adjList;

		Graph(){
			g_nodes.reserve(0);
			g_edges.reserve(0);
		}

		Graph (unsigned long long nodes_n, unsigned long long edges_n){
			g_nodes.reserve(nodes_n);
			g_edges.reserve(edges_n);
		}

		unsigned long long getEdgesNumber(){
			return g_edges.size();
		}

		unsigned long long getNodesNumber(){
			return g_nodes.size()-1;
		}

		float getMediumDegree(){
			unsigned long long medium_d = 0;
			for (unsigned long long i = 1; i <= g_nodes.size(); i++) medium_d += g_nodes[i].edges.size();
			return (medium_d / getNodesNumber());
		}

		void getEmpiricDistribution(float* node_d){

			for (unsigned long long i = 1; i <= getNodesNumber()-1; i++)
				node_d[i] = 0.0;
			for (unsigned long long i = 1; i <= getNodesNumber(); i++)
				node_d[g_nodes[i].edges.size()] ++;
			for (unsigned long long i = 1; i <= getNodesNumber()-1; i++)
				node_d[i] /= getNodesNumber();

		}

		bool operator== (Graph graph){
			for (unsigned int i = 1; i <= getNodesNumber(); i++)
				if (g_nodes[i] != graph.g_nodes[i]) return false;
			for (unsigned int i = 0; i < getEdgesNumber(); i++)
				if (g_edges[i] != graph.g_edges[i]) return false;
			return true;
		}

		// Funcoes para construcao dos itens do grafo

		void buildGraph(string filename){
			// Entrada: string contendo o nome do arquivo do qual sera construido o grafo
			// "Saida": o proprio grafo alterado

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

		void buildInformationFile (string filename){
			// "Entrada": o proprio grafo que chama a funcao para si mesmo
			// "Saida": arquivo contendo as informacoes constantes do enunciado do trabalho (nada e' retornado pela funcao)


			ofstream file ("info_"+filename, ifstream::out);
			file << "# n = " << getEdgesNumber() << endl;
			file << "# m = " << getNodesNumber() << endl;
			file << "# d_medio = " << getMediumDegree() << endl;

			float nodeDegrees[getNodesNumber()];
			getEmpiricDistribution(nodeDegrees);

			for (unsigned long long i = 1; i <= getNodesNumber()-1; i++)
				file << i << " " << nodeDegrees[i] << endl;

			file.flush();
			file.close();
		}
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
			adjMatrix = &adjm;
			return adjm;
		}

		bool testAdjacency (AdjacencyMatrix adjm, unsigned long long node1, unsigned long long node2){
			if (node1 == node2){
				cout << "Nodes are same" << endl;
				return false;
			}
			return adjm[node1][node2];
		}

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
			adjList = &adjl;
			return adjl;
		}


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


		// Funcoes de busca

		void bfs(unsigned long long initialNode, string filename){



		}

		set<unsigned long long> bfs(AdjacencyMatrix &adjm, unsigned long long initialNode, string filename){

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

			cout << "Initial node: " << initialNode << endl;

			adjm[initialNode][0] = true;
			set[initialNode][1] = 0;
			searchQueue.push(initialNode);

			file << "n: " << initialNode << "\troot\tlevel: " << set[initialNode][1] << endl;

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

		set<unsigned long long> bfs(AdjacencyList &adjl, unsigned long long initialNode, string filename){

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

			//cout << "Initial node: " << initialNode << endl;

			set[initialNode][2] = 1;
			set[initialNode][1] = 0;
			searchQueue.push(initialNode);

			file << "n: " << initialNode << "\troot\tlevel: " << set[initialNode][1] << endl;

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

		set<unsigned long long> dfs(AdjacencyMatrix &adjm, unsigned long long initialNode, string filename){


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

			//cout << "Initial node: " << initialNode << endl;

			set[initialNode][1] = 0;
			searchStack.push(initialNode);

			file << "n: " << initialNode << "\troot\tlevel: " << set[initialNode][1] << endl;

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

		set<unsigned long long> dfs(AdjacencyList &adjl, unsigned long long initialNode, string filename){

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

			cout << "Initial node: " << initialNode << endl;

			set[initialNode][1] = 0;
			searchStack.push(initialNode);

			file << "n: " << initialNode << "\troot\tlevel: " << set[initialNode][1] << endl;

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

		void foundConnectedComponents (AdjacencyList adjl, string filename){

			cout << ":: FOUND CONNECTED COMPONENTS ::" << endl;
			ofstream file ("cc_"+filename, ifstream::out);
			string fn;
			set <set<unsigned long long> > components;
			set<unsigned long long> component{};
			list <set<unsigned long long> > sortedComponents;

			time_t start, end;

			time(&start);

			for(unsigned long long i = 1; i <= getNodesNumber(); i++){
				for (set<unsigned long long>::iterator it = component.find(i); i == *it; i++, *it++);
				if (i > getNodesNumber()) break;
				component = bfs(adjl,i,filename);
				components.insert(component);
				if (!(components.size() % 200)) cout << components.size() <<" components found\n";
			}
			cout << components.size() <<" components found\n";

			time(&end);
			cout << "Time: " << difftime (end, start) << " seconds" << endl;

			/*for (set <set<unsigned long long> >::iterator it = components.begin(); it != components.end(); it++){
				cout << "Component > ";
				for (set<unsigned long long>::iterator it2 = it->begin(); it2 != it->end(); it2++)
					cout << *it2 << " ";
				cout << endl;
			}*/

			for (set <set<unsigned long long> >::iterator it = components.begin(); it != components.end(); it++){
				sortedComponents.push_back(*it);
				sortedComponents.sort(sortBySize);
			}

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
