/*
 * Simulation.cpp
 *
 *  Created on: May 19, 2021
 *      Author: HP
 */

#include "Simulation.h"
#include <random>
#include <ctime>

Simulation::Simulation(Graph &graph) {
	// TODO Auto-generated constructor stub
	this->edgeprob=graph.getEdgeprob();
	this->nodes=graph.nodes;
	this->g=graph.getGraph();
	this->groupSize=graph.numberOfGroups;
	this->nodeinfo=graph.nodeinfo;
	this->groupmap=graph.groupmap;
	srand(time(NULL));


}
bool Simulation::hasEdge(int u,int v)
{
	string s1=to_string(v)+','+to_string(u);
	float prob=edgeprob[s1];
	//cout<<prob;
	int randNum = (rand() % 1000) + 0;
	float pgenerated=(float)randNum/1000;
	//cout<<" "<<prob<<" "<<pgenerated<<" ";
	return pgenerated<prob;
}

unordered_set<int> Simulation::InfluenceMaximization(unordered_map<int,float> &weights,float scale[],int k,int numSim)
{
	unordered_set<int> seed;
	while(seed.size()<k)
	{
		float currentInfluenceOfNodes[nodes];
		for(int i=0;i<nodes;i++)
		{
			 currentInfluenceOfNodes[i]=0.0;
		}

	   for(int i=0;i<nodes;i++)
	   {
		   if(seed.find(i)==seed.end())
		   {
			   seed.insert(i);
			   vector<int> result=NtimescalcInf(seed,weights,numSim);
			   float totalSpreadOfSeed=0;
			   //cout<<"size of vector is"<<result.size()<<"\n";
			   for(int j=0;j<result.size();j++)
			   {
				   float crrSpreadofGroup=scale[j]*result[j];
				   if(crrSpreadofGroup>0)
				   {
					   totalSpreadOfSeed+=crrSpreadofGroup;
					   //cout<<"total Spread of "<<i<<" is: ";
					  // cout<<totalSpreadOfSeed<<"\n";
				   }
			   }
			   currentInfluenceOfNodes[i]=totalSpreadOfSeed;
			   seed.erase(i);
		   }
	   }
	   int maxindex=-1;
	   float max=0.0;
	   for(int i=0;i<nodes;i++)
	   	   {
		   	   if(max<=currentInfluenceOfNodes[i])
		   	   {
		   		   max=currentInfluenceOfNodes[i];
		   		   maxindex=i;
		   	   }
	   	   }
	   //cout<<"size of seed is"<<seed.size()<<"\n";
	  // cout<<"\n"<<"max seed is: "<<maxindex<<"\n";
	   if(seed.find(maxindex)==seed.end())
	   {
		   seed.insert(maxindex);
	   }
	   else
	   {
		   return seed;
	   }
	}
	return seed;
}

vector<int> Simulation::NtimescalcInf(unordered_set<int> &seed,unordered_map<int,float> &weights,int number)
{
	vector<int> res;
		for(int i=0;i<groupSize;i++)
		{
			res.push_back(0.0);
		}
	int start=0;
	while(start<number)
	{
		vector<int> answerOfCurrentSimulation=calcInf(seed,weights);
		for(int i=0;i<groupSize;i++)
		{
			res[i]+=answerOfCurrentSimulation[i];
		}
		start++;
	}
	for(int i=0;i<groupSize;i++)
	{
		res[i]=(float)res[i]/(float)number;
		//cout<<i<<"->"<<res[i]<<" "<<"\n";
	}

	return res;
}
vector<int> &Simulation::calcInf(unordered_set<int> &seed,unordered_map<int,float> &weights) {
	int result=0.0;
	vector<int> res;
	seedOfInfluence.clear();
	res.clear();
	for(int i=0;i<groupSize;i++)
	{
		res.push_back(0.0);
		seedOfInfluence.push_back(0.0);
	}
	unordered_set<int> visited;
	unordered_set<int> infSeedThathasWeight;
	stack<int> s;
	for(const auto&elem:seed)
	{
		s.push(elem);
	}
	while(!s.empty())
	{
		int crrnode=s.top();
		s.pop();
		visited.insert(crrnode);
		if(weights.find(crrnode)!=weights.end() && infSeedThathasWeight.find(crrnode)==infSeedThathasWeight.end())
		{
			infSeedThathasWeight.insert(crrnode);
			vector<int> groups;
			groups.clear();
			if(groupmap.find(crrnode)!=groupmap.end())
					{
						groups=groupmap[crrnode];

					}
					for(const auto&grp:groups)
					{
						if(weights.find(crrnode)!=weights.end())
						{
							res[grp]+=weights[crrnode];
							seedOfInfluence[grp]+=weights[crrnode];
						}
					}
		}
		vector<int> connectedNodes=g[crrnode];
		for(const auto&elem:connectedNodes)
		{
			if(visited.find(elem)==visited.end() && hasEdge(elem,crrnode))
			{
				s.push(elem);
			}
		}
	}
	/*
	for(const auto&elem:infSeedThathasWeight)
	{
		vector<int> groups;
		groups.clear();
		if(groupmap.find(elem)!=groupmap.end())
		{
			groups=groupmap[elem];

		}
		for(const auto&grp:groups)
		{
			if(weights.find(elem)!=weights.end())
			{
				res[grp]+=weights[elem];
			}
		}

	}*/
/*
	for(int i=0;i<groupSize;i++)
		{
			cout<<i<<"->"<<res[i]<<"\n";
		}
		*/


   // return res;
	return seedOfInfluence;
}

