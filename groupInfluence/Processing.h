/*
 * Processing.h
 *
 *  Created on: Mar 24, 2021
 *      Author: HP
 */

#ifndef PROCESSING_H_
#define PROCESSING_H_
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <stack>
#include <set>
#include <iostream>
#include <algorithm>
#include "Graph.h"
#include "RRsets.h"
using namespace std;


class Processing {
public:
	Processing();
	Processing(Graph &g,RRsets &rr);
	int m;
	unordered_set<int> s1;
	unordered_set<int> makeS1(Graph &g,RRsets &rr);
	unordered_map<int,unordered_map<int,int>> nodeinfo;
	void printGroupInfo();
	unordered_set<int> cancelledRRset;
	bool isStillValid(int elem,float vi[],int size);
	unordered_set<int> getAffectedNodes(vector<int> &nodeBelongstoGroups,unordered_set<int> &nodesAboveThreshold,vector<unordered_set<int>> &rrset,int poppedNode);

};

#endif /* PROCESSING_H_ */
