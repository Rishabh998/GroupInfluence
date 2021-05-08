/*
 * Graph.h
 *
 *  Created on: Feb 24, 2021
 *      Author: HP
 */

#ifndef GRAPH_H_
#define GRAPH_H_
#include "Node.h"
//#include "RRsets.h"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
using namespace std;

class Graph {
//RRsets rrnew;
private:

	unordered_map<int,vector<int>> g;
	unordered_map<int,float> weight;
	unordered_set<int> s1;


public:

	int nodes;
	int edges;
	int numberOfGroups;
	vector<int> groupnodes;
	unordered_map<int,unordered_set<int>> group;
	vector<float> vi;
	Graph(string file,int groupSize,float beta);
	void makegraph(string file);
	unordered_map<string,float> prob;
	unordered_map<int,unordered_map<int,int>> nodeinfo;
	unordered_map<int,unordered_set<int>> groupinfo; //G1-N1,N2
	unordered_map<int,vector<int>> groupmap;
	unordered_map<int,vector<int>> gtranspose;
	unordered_map<int,vector<int>> transposeGraph();
	void assignVis(float beta);
	unordered_map<int,vector<int>> getGraph();
	unordered_map<string,float> getEdgeprob();
	void assignGroups(int m,float p);
	void printGraph(unordered_map<int,vector<int>> g);
	unordered_map<int,unordered_set<int>> getGroups();
	unordered_set<int> makeS1();
	unordered_map<int,vector<int>> getTransposeGraph();
};

#endif /* GRAPH_H_ */
