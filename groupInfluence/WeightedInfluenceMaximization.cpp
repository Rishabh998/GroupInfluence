/*
 * WeightedInfluenceMaximization.cpp
 *
 *  Created on: Mar 17, 2021
 *      Author: HP
 */

#include "WeightedInfluenceMaximization.h"
#include "RRsets.h"
#include "Processing.h"

WeightedInfluenceMaximization::WeightedInfluenceMaximization(Graph &graph,unordered_set<int> &s1) {
	// TODO Auto-generated constructor stub

	this->edges=graph.edges;
	this->nodes=graph.nodes;
	unordered_map<string,float> prob=graph.getEdgeprob();
	unordered_map<int,vector<int>> graph_transpose=graph.getTransposeGraph();
	this->group=graph.getGroups();
	cout<<"Creating";
	//Processing p(graph);

	//unordered_set<int> s1=p.makeS1(graph);
	//s1.insert(45);


	rr=RRsets(graph,5000,s1,false);


}

vector<int> WeightedInfluenceMaximization::getTopSeed(int k,unordered_map<int,float> &weight)
{
	rr.changeInitalWeights(weight);
	vector<int> result=rr.seedOfWeightedInfluence(k, weight);
	return result;

}

vector<float> WeightedInfluenceMaximization::getFi(const vector<int> &X,int m)
{
	vector<float> result;
	return result;
	//return rr.calcSpreadOf(X, m);

}

