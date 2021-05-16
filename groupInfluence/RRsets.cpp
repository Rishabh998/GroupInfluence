/*
 * RRsets.cpp
 *
 *  Created on: Feb 28, 2021
 *      Author: HP
 */

#include "RRsets.h"
#include <algorithm>
#include <random>
#include <ctime>


RRsets::RRsets(){}

RRsets::RRsets(Graph &graph,int R,const unordered_set<int> &s1,bool makeS1)
{
	// TODO Auto-generated constructor stub


	this->nodes=graph.nodes;
	this->edgeprob=graph.getEdgeprob();
	this->s1=s1;
	this->groupinfo=graph.groupinfo;
	//this->weight=weight;
	this->groupnodes=graph.groupnodes;
	g=graph.transposeGraph();
	if(makeS1)
	{
		makeRRsetsForS1();
	}
	else
	{
		makeRRsets(R);
	}
	//createTransposeRRsets();
	cout<<"Size of RRset is"<<rrsets.size();

}

unordered_map<int,unordered_set<int>> RRsets::getRRsets()
		{
			return rrset;
		}

vector<unordered_set<int>> RRsets::getRRset()
		{
			cout<<"\n"<<"Size is "<<rrsets.size();
			return this->rrsets;
		}
unordered_map<int,int> RRsets::getIndexofRRsets()
{
	return rrMaptoIndex;
}
bool RRsets::hasEdge(int u,int v)
{
	string s1=to_string(v)+','+to_string(u);
	float prob=edgeprob[s1];
	//cout<<prob;
	int randNum = (rand() % 1000) + 0;
	float pgenerated=(float)randNum/1000;
	//cout<<" "<<prob<<" "<<pgenerated<<" ";
	return pgenerated<prob;
}
void RRsets::printRRset()
{
	int index=0;
	for(const auto&elem:rrsets)
	{
		unordered_set<int> crrset=elem;
		cout<<rrMaptoIndex[index]<<" -> ";
		for(const auto&e:crrset)
		{
			cout<<e<<" ";
		}
		cout<<"\n";

		index++;
	}
}
void RRsets::changeInitalWeights(unordered_map<int,float> &weight)
{
	RRweights.clear();
	int index=0;
	for(const auto &it:rrsets)
	{
		unordered_set<int> crrset=it;
		//cout<<"Size is"<<it.size()<<"\n";
		float group_weight=weight[rrMaptoIndex[index]]<0?0:weight[rrMaptoIndex[index]];

		for (const auto& elem: crrset)
		{
			if(RRweights.find(elem)==RRweights.end())
			{
				RRweights[elem]=group_weight;
			}
			else
			{
			    RRweights[elem]+=group_weight;
			}
		}
	}

}
unordered_set<int> RRsets::makeSingleRRset(int bottomnode)
{
	unordered_set<int> result;

	stack<int> s;
	s.push(bottomnode);
	//float group_weight=weight[bottomnode];
	while(!s.empty())
	{
		int crrnode=s.top();
		//cout<<"current node is"<<crrnode;
		s.pop();
		vector<int> edges=g[crrnode];

		if(s1.find(crrnode)!=s1.end()) //This node is present in S1
		{
			this->cancelledBys1=true;
			return result;
		}

		//cout<<"size of edges is"<<edges.size();
		if(result.find(crrnode) == result.end())// result does not contains crrnode
		{
			for(int i=0;i<edges.size();i++)
				{
				//cout<<"uninserted into rrset"<<edges[i];
					if((result.find(edges[i]) == result.end())) //result does not contains edges[i]
					{
						if(hasEdge(crrnode,edges[i]))
						{
							s.push(edges[i]);
							//cout<<"inderted into rrset"<<edges[i];
						}
					}
				}
		}
		//cout<<"\n";

		result.insert(crrnode);
		/*
		if(RRweights.find(crrnode)==RRweights.end())
		{
			RRweights[crrnode]=group_weight;
			printf("not found :weight of %d is %f \n",crrnode,group_weight);
		}
		else
		{
			RRweights[crrnode]+=group_weight;
			printf("weight of %d is %f \n",crrnode,RRweights[crrnode]);
		}
		*/

	}

	return result;

}

float RRsets::calculateInfluence(const unordered_set<int> &seed)
{
	float result=0.0;
	unordered_set<int>::iterator itr;
	unordered_set<int> resultSet;
	 for (const auto& elem: seed)
	 {
		  int node=elem;

		   if(transpose_rrset.find(node)!=transpose_rrset.end())
		   {
			   unordered_set<int> reachableNodes=transpose_rrset[node];
			   for (const auto& elem2: reachableNodes)
			   {
				   resultSet.insert(elem2);
			   }
		   }
	   }
   return (float)(resultSet.size());

}

int RRsets::getTopElement()
{
	float max=0;
	int maxindex=-1;
	 for(int i=0;i<this->nodes;i++)
	 {

	  if(RRweights.find(i)!=RRweights.end())
	  {
		  int key=i;
		  float value=RRweights[i];
		 if(value>max)
		 {
			 //cout<<"value is"<< value;
			 max=value;
			 maxindex=key;
		 }
	  }
	 }
	 RRweights[maxindex]=0;
	 return maxindex;
}
void RRsets::updateAffectedWeights(int topWeightedElement,unordered_set<int> &cancelledRRSetIndex,unordered_map<int,float> &weight)
{
	int index=0;
	for(const auto&it:rrsets)
	{
		if(cancelledRRSetIndex.find(index)==cancelledRRSetIndex.end()) //RR set is not cancelled
		{
			unordered_set<int> crrset=it;
			if(crrset.find(topWeightedElement)!=crrset.end()) //This RRset needs to be cancelled
			{
				cancelledRRSetIndex.insert(index);
				for(const auto&elem:crrset) //Update Weights
				{
					if(RRweights.find(elem)!=RRweights.end())
					{
						float w=-1.0;
						if(rrMaptoIndex.find(index)!=rrMaptoIndex.end() && weight.find(rrMaptoIndex[index])!=weight.end())
						{
							w=weight[rrMaptoIndex[index]];
						}

						if(w>=0)
						{
						RRweights[elem]=RRweights[elem]-w;
						}
					}

				}
			}
		}
		index++;
	}

}
vector<int> RRsets::seedOfWeightedInfluence(int k,unordered_map<int,float> &weight)
{
	vector<int> result;
	unordered_set<int> cancelledRRSetIndex;
	cancelledRRSetIndex.clear();
	while(k>0)
	{
		int topWeightedElement=getTopElement();
		if(topWeightedElement==-1)
		{
			return result;
		}
		cout<<"," <<topWeightedElement;//<<" : "<<RRweights[topWeightedElement];
		result.push_back(topWeightedElement);
		//unordered_set<int> reachable = transpose_rrset[topWeightedElement];
		updateAffectedWeights(topWeightedElement,cancelledRRSetIndex,weight);
		k--;
	}
	return result;
}

void RRsets::createTransposeRRsets()
{

	 for (const auto & [ key, value ] : rrset)
		{
		  if(rrsetsCancelledByS1.find(key)==rrsetsCancelledByS1.end()) //Do no remove this RRset
		  {
			 unordered_set<int> a=value;
			for (const auto& elem: a)
			{
				transpose_rrset[elem].insert(key);
			}
		  }

		}

}
vector<int> RRsets::chooseRRnode(vector<int> nodes,int R)
{
	vector<int> result;
	int sizeOfgroupNodes=nodes.size();
	for (int i = 1; i <= R; i++)
	{
	    int index= 0 + (rand() % sizeOfgroupNodes);
	    result.push_back(index);
	}
	return result;
}
void RRsets::makeRRsetsForS1()
{
	vector<int> randomlyChosenNodes;
	int i=0;
	for(const auto&[key,value]:groupinfo)
	{
		unordered_set<int> groupNodes=value;
		vector<int> groupNodeVector(groupNodes.begin(), groupNodes.end());
		vector<int> ChosenNodes;
		ChosenNodes.clear();
		int R=groupNodes.size()*3;
		ChosenNodes=chooseRRnode(groupNodeVector,R); //Change size of R here
		randomlyChosenNodes.insert(randomlyChosenNodes.end(), ChosenNodes.begin(), ChosenNodes.end());
	}
	for(const auto& it:randomlyChosenNodes)
		{
			unordered_set<int> rrSetOfNode=makeSingleRRset(it);
			rrsets.push_back(rrSetOfNode);
			rrMaptoIndex[i]=it;
			i++;
		}

}
void RRsets::makeRRsets(int R)
{

	int i=0;
	srand(time(NULL));
	vector<int> nodes=groupnodes;

	vector<int> randomlyChosenNodes=chooseRRnode(nodes,R);
	for(const auto& it:randomlyChosenNodes)
	{
		//cout<<it<<"\n";
		this->cancelledBys1=false;
		unordered_set<int> rrSetOfNode=makeSingleRRset(it);
		if(this->cancelledBys1==false)
		{
			rrsets.push_back(rrSetOfNode);
			//cout<<rrSetOfNode.size();
			rrMaptoIndex[i]=it;
			i++;
		}
	}

}
/*
vector<float> RRsets::calcSpreadOf(const vector<int> &X,int m)
{
	vector<float> result(m);
	 for (const auto& elem: X)
	 {
		 unordered_set<int> reachable=transpose_rrset[elem];
		 for (const auto& innerelem: reachable)
		 {
			 result[groupOf[innerelem]]++;
		 }

     }

	return result;

}
*/
