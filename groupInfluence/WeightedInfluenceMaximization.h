/*
 * WeightedInfluenceMaximization.h
 *
 *  Created on: Mar 17, 2021
 *      Author: HP
 */

#ifndef WEIGHTEDINFLUENCEMAXIMIZATION_H_
#define WEIGHTEDINFLUENCEMAXIMIZATION_H_

#include <iostream>
#include "Node.h"
#include "Graph.h"
#include "string.h"
#include "RRsets.h"
#include <fstream>

class WeightedInfluenceMaximization {

public:
	RRsets rr;
	int nodes;
	int edges;

	unordered_map<int,unordered_set<int>> group;
	WeightedInfluenceMaximization(Graph &graph,unordered_set<int> &s1);
	vector<int> getTopSeed(int k,unordered_map<int,float> &weight);
	vector<float> getFi(const vector<int> &X,int m);
};

#endif /* WEIGHTEDINFLUENCEMAXIMIZATION_H_ */
