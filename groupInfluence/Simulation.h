/*
 * Simulation.h
 *
 *  Created on: May 19, 2021
 *      Author: HP
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <stack>
#include <set>
#include <iostream>
#include "Graph.h"

class Simulation {
public:
	int groupSize;
	int nodes;
	vector<int> seedOfInfluence;
	unordered_map<string,float> edgeprob;
	unordered_map<int,vector<int>> g;
	unordered_map<int,vector<int>> groupmap;
	unordered_map<int,unordered_map<int,int>> nodeinfo;
	Simulation(Graph &g);
	bool hasEdge(int u,int v);
	unordered_set<int> InfluenceMaximization(unordered_map<int,float> &weights,float scale[],int k,int numSim);
	vector<int> &calcInf(unordered_set<int> &seed,unordered_map<int,float> &weights);
	vector<int> NtimescalcInf(unordered_set<int> &seed,unordered_map<int,float> &weights,int number);
};

#endif /* SIMULATION_H_ */
