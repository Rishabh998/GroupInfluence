/*
 * RRsets.h
 *
 *  Created on: Feb 28, 2021
 *      Author: HP
 */

#ifndef RRSETS_H_
#define RRSETS_H_
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <stack>
#include <set>
#include <iostream>
#include "Graph.h"
using namespace std;


class RRsets {
private:

	unordered_map<int,unordered_set<int>> rrset;
	unordered_map<int,unordered_set<int>> transpose_rrset;
	unordered_set<int> rrsetsCancelledByS1;
	unordered_set<int> s1;
	unordered_map<int,vector<int>> g; //transpose graph
	vector<int> groupnodes;
	unordered_map<int,int> rrMaptoIndex;
	bool cancelledBys1=false;
	vector<unordered_set<int>> rrsets;
	unordered_map<string,float> edgeprob;
	//unordered_map<int,float> weight;
	unordered_map<int,float> RRweights;
	int nodes;
public:
	RRsets();
	RRsets(Graph &graph,int R,const unordered_set<int> &s1);
	void makeRRsets(int R);
	unordered_set<int> makeSingleRRset(int bottomnode);
	bool hasEdge(int u,int v);
	unordered_map<int,unordered_set<int>> getRRsets();
	void createTransposeRRsets();
	vector<int> seedOfWeightedInfluence(int k,unordered_map<int,float> &weight);
	void changeInitalWeights(unordered_map<int,float> &weight);
	float calculateInfluence(const unordered_set<int> &seed);
	int getTopElement();
	vector<int> chooseRRnode(vector<int> nodes,int R);
	void updateAffectedWeights(int topWeightedElement,unordered_set<int> &cancelledRRSetIndex,unordered_map<int,float> &weight);
	vector<unordered_set<int>> getRRset();
	unordered_map<int,int> getIndexofRRsets();
	void printRRset();
	//vector<float> RRsets::calcSpreadOf(const vector<int> &X,int m);
};

#endif /* RRSETS_H_ */
