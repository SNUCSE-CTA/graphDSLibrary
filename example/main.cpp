#include "../graph.h"
#include "../dag.h"
#include "util.h"

//global variables
long long* core = NULL; //core[i]: core number of vertex i (for coreDecomposition)

//definitions of functions
void test(Graph&, FileFormat);
	//debugging perpose only.
	//Perform following operations for a small example graph
	//	- neighbor(v): print neighbors of v
	//	- label(v): print label of v
	//	- degree(v): print degree of v
	//	- neighbor(v, l): print neighbors of v whose labels are l
	//	- frequency(l): print frequency of label l
	//	- vertex(l): print vertices labeled l

void testDAG(DAG&);

void coreDecomposition(Graph&); //solve core decomposition problem. (library test perpose)
void printCoreStat(Graph&);

int main(int argc, char* argv[])
{
	if( argc != 3 ) {
		cout << "Usage: ./program datagraph filetype(igraph, snap, sdf)" << endl;
		return -1;
	}

	FileFormat format;
	if( strcmp(argv[2], "igraph") == 0 )
		format = FileFormat::IGRAPH;
	else if( strcmp(argv[2], "snap") == 0 )
		format = FileFormat::SNAP;
	else if( strcmp(argv[2], "sdf") == 0 )
		format = FileFormat::SDF;

	Timer timer;
	timer.start();
	Graph graph(argv[1], format);
	cout << "Reading Graph Time: " << timer.end() << " sec" << endl;
	test(graph, format);

	coreDecomposition(graph);
	printCoreStat(graph);
	
	//free memory
	if(core != NULL) {
		delete[] core;
		core = NULL;
	}

	////////////////////////////////
	////////////////////////////////
	////////////////////////////////
	//test libdag.a
	
	DAG dag(argv[1], format);
	testDAG(dag);

	return 0;
}

//implementations
void test(Graph& aGraph, FileFormat aFormat)
{
	cout << "testing graph library" << endl;
	cout << "**************" << endl;
	cout << "1. neighbor(v)" << endl;
	for(long long v = 0; v < aGraph.numVertex; ++v) {
		cout << "N(" << v << "): ";
		for(long long i = 0; i < aGraph.degree[v]; ++i)
			cout << aGraph.edge[v][i] << " ";
		cout << endl;
	}

	cout << "**************" << endl;
	cout << "2. label(v)" << endl;
	if( aGraph.label != NULL) {
		for(long long v = 0; v < aGraph.numVertex; ++v) {
			if( aFormat == FileFormat::SDF )
				cout << "L(" << v << "): " << aGraph.chemRealLabel[ aGraph.label[v] ] << endl;
			else
				cout << "L(" << v << "): " << aGraph.realLabel[ aGraph.label[v] ] << endl;
		}
	}
		
	cout << "**************" << endl;
	cout << "3. degree(v)" << endl;
	for(long long v = 0; v < aGraph.numVertex; ++v)
		cout << "D(" << v << "): " << aGraph.degree[v] << endl;

	cout << "**************" << endl;
	cout << "4. neighbor(v, l): " << endl;
	if( aGraph.label != NULL) {
		for(long long v = 0; v < aGraph.numVertex; ++v) {
			for(long long l = 1; l <= aGraph.numLabel; ++l) {
				pair<long long, long long> p = aGraph.idxNeighbor(v, l);
				if(p.first > p.second)
					continue;
				if(aFormat == FileFormat::SDF)
					cout << "N(" << v << ", " << aGraph.chemRealLabel[l] << "): ";
				else
					cout << "N(" << v << ", " << aGraph.realLabel[l] << "): ";
				for(long long i = p.first; i <= p.second; ++i)
					cout << aGraph.edge[v][i] << " ";
				cout << endl;
			}
		}
	}

	cout << "**************" << endl;
	cout << "5. frequency(l)" << endl;
	if( aGraph.label != NULL ) {
		for(long long l = 1; l <= aGraph.numLabel; ++l) {
			if( aFormat == FileFormat::SDF)
				cout << "F(" << aGraph.chemRealLabel[l] << "): " << aGraph.labelFrequency[l] << endl;
			else
				cout << "F(" << aGraph.realLabel[l] << "): " << aGraph.labelFrequency[l] << endl;
			}
	}

	cout << "**************" << endl;
	cout << "6. vertex(l)" << endl;
	if( aGraph.label != NULL) {
		for(long long l = 1; l <= aGraph.numLabel; ++l) {
			if(aFormat == FileFormat::SDF)
				cout << "V(" << aGraph.chemRealLabel[l] << "): ";
			else
				cout << "V(" << aGraph.realLabel[l] << "): ";
			pair<long long, long long> p = aGraph.idxVertex(l);
			for(long long i = p.first; i <= p.second; ++i)
				cout << aGraph.sortedVertex[i] << " ";
			cout << endl;
		}
	}
}

void testDAG(DAG& aDag)
{
	cout << "root: " << aDag.root << endl;
	for(long long i = 0; i < aDag.numVertex; ++i) {
		cout << "Parent(" << i << "): ";
		for(long long j = 0; j < aDag.numParent[i]; ++j) {
			cout << aDag.parent[i][j] << " ";
		}
		cout << endl;
	}
	for(long long i = 0; i < aDag.numVertex; ++i) {
		cout << "Children(" << i << "): ";
		for(long long j = 0; j < aDag.numChild[i]; ++j) {
			cout << aDag.child[i][j] << " ";
		}
		cout << endl;
	}
}

void coreDecomposition(Graph& aGraph)
{
	Timer timer;
	timer.start();

	long long numVertex = aGraph.numVertex;
	long long* bin = new long long[numVertex];
	memset(bin, 0, sizeof(long long) * numVertex);
	long long* pos = new long long[numVertex];
	long long* vert = new long long[numVertex];

	if( core != NULL ) {
		delete[] core;
	}
	core = new long long[numVertex];

	for(long long i = 0; i < numVertex; ++i) {
		//core[i] = edge[i].size();
		core[i] = aGraph.degree[i];
		++bin[ core[i] ];
	}

	long long start = 0;
	long long num;

	for(long long d = 0; d <= aGraph.maxDegree; ++d) {
		num = bin[d];
		bin[d] = start;
		start += num;
	}

	for(long long i = 0; i < numVertex; ++i) {
		pos[i] = bin[ core[i] ];
		vert[ pos[i] ] = i;
		++bin[ core[i] ];
	}

	for(long long d = aGraph.maxDegree; d > 0; --d) {
		bin[d] = bin[d-1];
	}
	bin[0] = 0;

	for(long long i = 0; i < numVertex; ++i) {
		long long v = vert[i];
		//for(long j = 0; j < edge[v].size(); ++j) {
		for(long long j = 0; j < aGraph.degree[v]; ++j) {
			long long u = aGraph.edge[v][j];
			if( core[u] > core[v] ) {
				long long du = core[u];
				long long pu = pos[u];
				long long pw = bin[du];
				long long w = vert[pw];
				if( u != w ) {
					pos[u] = pw;
					pos[w] = pu;
					vert[pu] = w;
					vert[pw] = u;
				}
				++bin[du];
				--core[u];
			}
		}
	}

	delete[] bin;
	delete[] pos;
	delete[] vert;

	cout << "core decomposition time: " << timer.end() << " sec" << endl;
}

void printCoreStat(Graph& aGraph)
{
	vector<long long> numCore(aGraph.numVertex);
	for(long long i = 0; i < aGraph.numVertex; ++i) {
		numCore[i] = 0;
	}

	long long maxCore = 0;
	for(long long i = 0; i < aGraph.numVertex; ++i) {
		++numCore[ core[i] ];
		if( maxCore < core[i] ) {
			maxCore = core[i];
		}
	}

	for(long long i = 0; i <= maxCore; ++i) {
		if( numCore[i] != 0 ) {
			cout << "# " << i << " core: " << numCore[i] << "\t";
			cout << numCore[i]/(double)aGraph.numVertex*100 << "%" << endl;
		}
	}
}
