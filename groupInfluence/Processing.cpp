/*
 * Processing.cpp
 *
 *  Created on: Mar 24, 2021
 *      Author: HP
 */

#include "Processing.h"
#include "RRsets.h"

Processing::Processing() {
	// TODO Auto-generated constructor stub
	this->m=0;

}
Processing::Processing(Graph &g,RRsets &rr) {
	this->m=g.numberOfGroups;
	this->s1=makeS1(g,rr);

}
unordered_set<int> Processing::makeS1(Graph &g,RRsets &rr)
{
	unordered_set<int> result;
	vector<unordered_set<int>> rrset=rr.getRRset();
	unordered_map<int,int> rmap=rr.getIndexofRRsets();
	int index=0;
	float vi[m];
	for(int i=0;i<m;i++)
	{
		vi[i]=g.vi[i];
	}
	unordered_set<int> nodesAboveThreshold;
	unordered_map<int,vector<int>> groupMap=g.groupmap;

	for(const auto&it:rrset)
	{
		int grpNode=rmap[index];
		vector<int> nodeBelongstoGroups;
		if(groupMap.find(grpNode)!=groupMap.end())
		{
			nodeBelongstoGroups=groupMap[grpNode];
		}
		unordered_set<int> crrset=it;
		//cout<<grpNode<<" Belongs to group "<<nodeBelongstoGroups[0]<<"\n";
		for(const auto&node:crrset)
		{
			unordered_map<int,int> countMap;
			countMap.clear();
			if(nodeinfo.find(node)!=nodeinfo.end())
			{
				countMap=nodeinfo[node];
			}
			for(const auto&groups:nodeBelongstoGroups)
			{
					if(countMap.find(groups)!=countMap.end())
					{
						countMap[groups]=countMap[groups]+1;
					}
					else
					{
						countMap[groups]=1;
					}
					if(countMap[groups]>=vi[groups])
					{
						cout<<"Insertes into above threshold is "<<node<<"value is"<<vi[groups]<<" "<<groups<<"\n";
						nodesAboveThreshold.insert(node);
					}
			}
			nodeinfo[node]=countMap;
		}
		index++;

	}

	cout<<"Node that are above threshold are : \n";
	for(const auto&e:nodesAboveThreshold)
	{
		cout<<e<<"\n";
	}
	cout<<"Node above threshold ends : \n";
	cout<<"Size of nodeinfo is: "<<nodeinfo.size()<<"\n";
	//printGroupInfo();

	unordered_set<int> erasedFromVector;
	vector<char> currNodes(nodesAboveThreshold.begin(), nodesAboveThreshold.end());
		 while (!currNodes.empty())
		  {
		    int poppedNode=currNodes.back();
		    if(nodesAboveThreshold.find(poppedNode)!=nodesAboveThreshold.end())
		    {
		    	result.insert(poppedNode);
		    }

		    cout<<"\n"<<"Inserted to s1 "<<poppedNode<<"\n";
		    currNodes.pop_back();
		    if(erasedFromVector.find(poppedNode)!=erasedFromVector.end())
		    {
		    	continue;
		    }
		    erasedFromVector.insert(poppedNode);
		    nodesAboveThreshold.erase(poppedNode);
		    vector<int> nodeBelongsto;
		    if(groupMap.find(poppedNode)!=groupMap.end())
		    {
		    	nodeBelongsto=groupMap[poppedNode];
		    }
		    unordered_set<int> affectedNode=getAffectedNodes(nodeBelongsto,nodesAboveThreshold,rrset,poppedNode);
		    for(const auto&elem:affectedNode)
		    {
		    	cout<<"Affected node is "<<elem<<"\n";
		    	//firstNodeMinusSecond(elem,poppedNode);
		    	if(!isStillValid(elem,vi,m)) ///////Change GROUP Size here
		    	{
		    		nodesAboveThreshold.erase(elem);
		    		//currNodes.erase(std::remove(currNodes.begin(), currNodes.end(), elem), currNodes.end());
		    		erasedFromVector.insert(elem);
		    		cout<<"\n"<<"************This node is deleted**********"<<elem<<"\n";
		    	}
		    }

		  }

		// cout<<"Node after update are"<<"\n";
		 //printGroupInfo();
	//Node info created . create list
	return result;
}
void Processing::printGroupInfo()
{
	for(const auto&[key,value]:nodeinfo)
	{
		int crrnode=key;
		cout<<"\n";
		cout<<"Nodeinfo of "<<crrnode<<"\n";
		unordered_map<int,int> crrmap=value;
		for(const auto&[k,v]:crrmap)
		{
			cout<<k<<" : "<<v<<"\n";
		}
	}



}

bool Processing::isStillValid(int elem,float vi[],int size)
{
	bool res=false;
	unordered_map<int,int> countMap=nodeinfo[elem];
	//cout<<"\n"<<"Checking valididty of "<<elem<<"\n";
	for(const auto&[key,value]:countMap)
	{
		//cout<<"\n"<<"key is "<<key<<"\n";
		//cout<<"value is "<<value<<"\n";
		if(key>=0 && key<size && vi[key]<=value)
		{
			//cout<<"Valid,coz of group"<<key;
			return true;
		}
	}
	//cout<<"not valid";


	return false;

}



unordered_set<int> Processing::getAffectedNodes(vector<int> &nodeBelongstoGroups,unordered_set<int> &nodesAboveThreshold,vector<unordered_set<int>> &rrset,int poppedNode)
{
	unordered_set<int> result;
	//vector<int> nodeBelongstoGroups=g.groupmap[poppedNode];
	int index=0;
	for(const auto&it:rrset)
	{
		if(cancelledRRset.find(index)==cancelledRRset.end())
		{
		unordered_set<int> crrset=it;
		if(crrset.find(poppedNode)!=crrset.end())
		{
			cancelledRRset.insert(index);
			for(const auto&elem:crrset) //Traverse Affected Nodes
			{
				if(nodesAboveThreshold.find(elem)!=nodesAboveThreshold.end()) //Update only nodes that were above threshold
				{
					unordered_map<int,int> countMap=nodeinfo[elem]; //Get node info of a nodes that is affected and above threshold
					for(const auto& groups:nodeBelongstoGroups)
					{
						if(countMap.find(groups)!=countMap.end())
						{
							countMap[groups]--;
						}
					}
					nodeinfo[elem]=countMap;
					result.insert(elem);
				}
			}
		}
	}
		index++;
	}

	return result;
}

