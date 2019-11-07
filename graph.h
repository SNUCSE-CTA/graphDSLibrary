// Name		: graph.h
// Author	: Geonmo Gu
// Version	: 1.0
// Copyright: Apache License 2.0

#ifndef GRAPH_H
#define GRAPH_H

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <ctime> 

using namespace std;

struct Vertex
{
	long long id;
	long long label;
	bool operator<(const Vertex& a) const 
	{
		return label < a.label;
	}
	bool operator==(const Vertex& a) const
	{
		return id == a.id;
	}
};

enum class FileFormat
{
	IGRAPH,
	SNAP,
	SDF
};

//Undirected Graph
class Graph
{
protected:
	bool mUseIdxEdge = false;
public:
	//global variables
	long long numVertex; //#vertices
	long long numEdge;	 //#edges (in undirected)
	long long numLabel;  //the number of distinct labels
	long long largestLabel; //largest label value
	long long maxDegree; //maximum degree
	double avgDegree;	 //average degree (in undirected graph)

	long long** edge = NULL;  //adjacency array
							  //each edge[v] stores sorted neighbors of v in their labels order
	long long* degree = NULL; //degree[i]: degree of vertex i
	long long* label = NULL; //label[i]: (renamed) label of vertex i
	long long* renamedLabel = NULL; //this one-to-one mapping function maps 
									//a real label to a label between [0..numLabel-1]
	long long* realLabel = NULL; 	//this one-to-one mapping function maps 
	map<string, long long> chemLabel;
	map<long long, string> chemRealLabel;
									//a renamed label to the real label.
	long long* labelFrequency = NULL; //labelFrequency[l]: #vertices having label l
	long long* sortedVertex = NULL; //sortedVertex stores sorted vertices in their labels order
	long long* idxSortedVertex = NULL; //idxSortedVertex[l] returns ending position
									//where vertex labeled l appears in sortedVertex
	long long** idxEdge = NULL;	//idxEdge[v][l] has rightmost position
								//where vertex labeled l appears in edge[v]
								//requries O(numLabel * numVertex) space complexity


	//member functions
	Graph();
	Graph(string aFileName, FileFormat aFormat, bool useIdxEdge = false);
	~Graph();
	
	void clearMemory();
	void readIGraph(string aFileName); 
	void readSNAP(string aFileName);
	void readSDF(string aFileName);

	pair<long long, long long> idxNeighbor(long long aVertex, long long aLabel); 
	//return [start, end] index pair of edge[aVertex],
	//where label of u in edge[aVertex][start..end] is aLabel
	//if such u does no exist, start > end
	
	pair<long long, long long> idxVertex(long long aLabel);
	//return [start, end] index pair of sortedVertex
	//where label of u in sortedVertex[start..end] is aLabel
	//Note that such u always exists
};
#endif
