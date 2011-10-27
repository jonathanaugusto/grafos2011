/**
 * Universidade Federal do Rio de Janeiro
 * COS242 - Teoria dos Grafos
 * @descr	Trabalho pratico da disciplina
 * @author	Bruno Tomas / Jonathan Augusto
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_

#pragma once  // Solve cross-reference (Node includes Edge, that includes Node...)

#define OPERATIONSFILE_SYNTAX	"# -- Syntax\n\
								 # operation:time:memory\n\
								 # -- Structure acronyms\n\
								 # F: file\n\
								 # G: graph object-oriented\n\
								 # L: adjacency list\n\
								 # M: adjacency matrix\n\
								 # -- Operations\n\
								 # buildAB: build from structure A to B\n\
								 # [b,d]fsA: bfs/dfs from structure A\n\
								 # ccA: found connected components from structure A"
#define OPERATIONSFILE_NAME "operations.txt"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <bitset>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <utility>
#include <ctime>
#include <limits>
#include <thread>

#include "Edge.h"
#include "Graph.h"

#endif /* INCLUDES_H_ */
