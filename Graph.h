#ifndef GRAPH_H
#define GRAPH_H

#include "includes.h"

using namespace std;

class Graph{

	private:
		vector<Node> *nodes;
		vector< bitset<1> > **adjMatrix; // bitset<N> indica uma lista onde cada posicao contem N bits
		vector<Node> *adjList;

	public:
		// Setters e Getters (esses ultimos somente com ponteiros)
		void setNodes (vector<Node>);
		void setAdjMatrix (vector< bitset<1> >);
		void setAdjList (vector<Node>);
		void getNodes(vector<Node>*);
		void getAdjMatrix(bitset<1>**);
		void getAdjList(vector<Node>*);

		// Funcoes para construcao dos itens do grafo
		void buildGraph(Graph *, string);
		void buildInformationFile (string);
		void buildAdjMatrix(vector< bitset<1> >*);
		void buildAdjList(vector<Node> *);

		// Funcoes de busca (retornam ponteiro para a estrutura de dados)
		void dfs(int, string, queue<Node>*);
		void bfs(int, string, stack<Node>*);
      
};


void Graph::buildGraph (Graph *graph, string filename){
	// Entrada: string contendo o nome do arquivo do qual sera construido o grafo
	// "Saida": ponteiro para o grafo gerado (devolvido pelo ponteiro)

};

void buildInformationFile (string filename){
	// "Entrada": o proprio grafo que chama a funcao para si mesmo
	// "Saida": arquivo contendo as informacoes constantes do enunciado do trabalho (nada e' retornado pela funcao)

};

void buildAdjMatrix(vector< bitset<1> >*){
	// "Entrada": o proprio grafo que chama a funcao para si mesmo
	// Saida: "matriz de ponteiros" que aponta para a matriz de adjacencia do grafo

};

void bfs (int initialNode, string filename, queue<Node> *searchQueue){

};

void dfs (int initialNode, string filename, stack<Node> *searchStack){

};

#endif
