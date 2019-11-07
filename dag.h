#ifndef DAG_H
#define DAG_H
#include <iostream>
#include "graph.h" // FileType
using namespace std;

class DAG : public Graph
{
private:
	long long selectRoot();
public:
//member variables for directed acyclic graph
	long long root = -1;
	long long** parent = NULL; //parent[i] is an array containing  parents of vertex i
	long long* numParent = NULL; //numParent[i] contains the number of parent of vertex i
	long long** child = NULL; //child[i] is an array containing children of vertex i
	long long* numChild = NULL; //numChild[i] contains the number of children of vertex i

	DAG();
	~DAG();
	DAG(string aFileName, FileFormat aFormat, long long aRoot = -1, bool aUseIdxEdge = false);
	
	void clearMemory();
	void buildDAG(long long aRoot);
};

#endif
