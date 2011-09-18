#ifndef GRAPH_H
#define GRAPH_H

#include "Includes.h"

using namespace std;

class Graph{

	public:
		vector<Node> g_nodes;
		vector<Edge> g_edges;
		vector< bitset<1> > adjMatrix; // bitset<N> indica uma lista onde cada posicao contem N bits
		vector< list<int> > adjList;

		Graph(){
			g_nodes.reserve(0);
			g_edges.reserve(0);
		}

		Graph (int nodes_n, int edges_n){
			g_nodes.reserve(nodes_n);
			g_edges.reserve(edges_n);
		}

		int getEdgesNumber(){
			return g_edges.size();
		}

		int getNodesNumber(){
			return g_nodes.size()-1;
		}

		float getMediumDegree(){
			int medium_d = 0;
			for (unsigned int i = 1; i <= g_nodes.size(); i++) medium_d += g_nodes[i].edges.size();
			return (medium_d / getNodesNumber());
		}

		void getEmpiricDistribution(float* node_d){

			for (int i = 1; i <= getNodesNumber()-1; i++)
				node_d[i] = 0.0;
			for (int i = 1; i <= getNodesNumber(); i++)
				node_d[g_nodes[i].edges.size()] ++;
			for (int i = 1; i <= getNodesNumber()-1; i++)
				node_d[i] /= getNodesNumber();
			for (int i = 1; i <= getNodesNumber()-1; i++)
				cout << i << " " << node_d[i] << endl;

		}
		// Funcoes para construcao dos itens do grafo

		void buildGraph(string filename){
			// Entrada: string contendo o nome do arquivo do qual sera construido o grafo
			// "Saida": o proprio grafo alterado

			int nodes_n, edges_n, node1_n, node2_n;
			time_t start, end;

			ifstream file (filename, ifstream::in);
			if (file.fail()) cout << "Read error :(" << endl;

			time(&start);

			file >> nodes_n;

			cout << "File: " << filename << endl;

			Node node0(0);
			g_nodes.push_back(node0); // g_nodes[0] empty

			for (int i = 1; i <= nodes_n; i++){
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

		};

		void buildInformationFile (string filename){
			// "Entrada": o proprio grafo que chama a funcao para si mesmo
			// "Saida": arquivo contendo as informacoes constantes do enunciado do trabalho (nada e' retornado pela funcao)


			ofstream file ("info_"+filename, ifstream::out);
			file << "# n = " << getEdgesNumber() << endl;
			file << "# m = " << getNodesNumber() << endl;
			file << "# d_medio = " << getMediumDegree() << endl;

			float nodeDegrees[getNodesNumber()];
			getEmpiricDistribution(nodeDegrees);

			for (unsigned int i = 1; i <= getNodesNumber()-1; i++)
				file << i << " " << nodeDegrees[i] << endl;

		}
		vector< bitset<1> > buildAdjMatrix(){
			// "Entrada": o proprio grafo que chama a funcao para si mesmo
			// Saida:  matriz de adjacencia do grafo

		}

		void buildAdjList(vector<int> *);

		// Funcoes de busca (retornam ponteiro para a estrutura de dados)
		void dfs(int, string, queue<Node>*);
		void bfs(int, string, stack<Node>*);

};


void buildAdjList(vector< list<int> >*){
	// "Entrada": o proprio grafo que chama a funcao para si mesmo
	// Saida: "vector de lists" que aponta para a lista de adjacencia do grafo

};

void bfs (int initialNode, string filename, queue<Node> *searchQueue){

};

void dfs (int initialNode, string filename, stack<Node> *searchStack){

};

#endif
