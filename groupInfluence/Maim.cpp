/*
 * Maim.cpp
 *
 *  Created on: Feb 24, 2021
 *      Author: HP
 */

#include <iostream>
#include "Node.h"
#include "graph.h"
#include "string.h"
#include "RRsets.h"
#include <fstream>
#include "WeightedInfluenceMaximization.h"
#include <math.h>
#include "Processing.h"
#define alpha 0.63

using namespace std;

unordered_map<int,int> calcSpreadOf(RRsets &rr,Graph &g,unordered_set<int> &s1,int m)
{
	unordered_map<int,int> result;
	unordered_map<int,unordered_set<int>> r;
	unordered_map<int,unordered_set<int>> groupinfo=g.groupinfo;
	vector<unordered_set<int>> rrset=rr.getRRset();
	unordered_map<int,int> rmap=rr.getIndexofRRsets();
	unordered_map<int,vector<int>> groupMap=g.groupmap;
	int index=0;
	for(const auto &it:rrset)
	{
		int grpNode=rmap[index];
		unordered_set<int> crrset=it;
		for(const auto &es1:s1)
		{
			int a=es1;
			if(it.find(a)!=it.end())
			{
				vector<int> groups=groupMap[grpNode];
				for(const auto &gele:groups)
				{
					int crrgroup=gele;
					if(r.find(crrgroup)==r.end())
					{
						unordered_set<int> elements;
						elements.insert(grpNode);
						r[crrgroup]=elements;
					}
					else
					{
						r[crrgroup].insert(grpNode);
					}
				}

			}
		}
		index++;

	}
	cout<<"\n";
	int total=0;
	cout<<"Size of r is "<<r.size();

	for(const auto &[key,value]:r)
	{
		result[key]=value.size();
		total+=result[key];
		//cout<<"Spread "<<key<<" :"<<result[key]<<"\n";
	}
	for(int i=0;i<m;i++)
	{
		if(result.find(i)==result.end())
		{
			result[i]=0;
		}
	}
	cout<<"Total spread is :"<<total;
	return result;

}

unordered_map<int,float> assignGroupWeights(float scale[],const unordered_map<int,unordered_set<int>> &groupinfo)
{
	unordered_map<int,float> result;
	for (const auto & [ key, value ] : groupinfo)
			{
			   unordered_set<int> a=value;
				for (const auto& elem: a)
				{
					if(result.find(elem)==result.end())//Not found
					{
						result[elem]=scale[key];
					}
					else//found
					{
						result[elem]+=scale[key];
					}
				}
			}

	return result;
}

vector<float> MWU(Graph &graph,RRsets &rr,unordered_set<int> &s1,int k1,float delta,float lamda[10],unordered_map<int,int> &spreadS1)
{
	int m=graph.numberOfGroups;
	int n=graph.nodes;
	vector<float> xt(n);
	double T=2.0*(log(m)/(delta*delta));
	WeightedInfluenceMaximization wm(graph,s1);
	int t=1;
		while(t<=T)
		{
			float scale[m];
			float mt[m];
			for(int i=0;i<m;i++)
				{
					scale[i]=(graph.vi[i]-spreadS1[i])!=0?(lamda[i]/(graph.vi[i]-spreadS1[i])):0;
					if(scale[i]<0)
					{
						scale[i]=0;
					}
					cout<<scale[i];
				}
			unordered_map<int,float> weights=assignGroupWeights(scale,graph.groupinfo);
			vector<int> X;
			X.clear();
			X=wm.getTopSeed(k1,weights);
			cout<<"\n";
			cout<<"Size of X is"<<X.size()<<"\n";
			unordered_set<int> seedofInfluence(X.begin(),X.end());
			unordered_map<int,int> spreadOfX=calcSpreadOf(rr,graph,seedofInfluence,m);
			for(int i=0;i<m;i++)
			{
				spreadOfX[i]=graph.vi[i]-spreadS1[i]!=0?(spreadOfX[i]/(graph.vi[i]-spreadS1[i])):0;
			}
			for(int i=0;i<m;i++)
			{
				mt[i]=spreadOfX[i]-alpha;
				lamda[i]=lamda[i]*(1-delta*mt[i]);
			}
			 for (auto& it : X) {
			  xt[it]=xt[it]+1;
				}
			 for (int i=0;i<n;i++)
				 {
					 xt[i]=xt[i]/T;
				 }
			cout<<"\n";
			cout<<"Value of T is"<<t<<"\n";
			t++;
		}
/*
	double T=2.0*(log(m)/(delta*delta));
	WeightedInfluenceMaximization wm(graph,s1);


	int t=1;
	while(t<=T)
	{
		float scale[m];
		float mt[m];
		for(int i=0;i<m;i++)
		{
			scale[i]=lamda[i]/(graph.vi[i]-spreadS1[i]);
		}
		unordered_map<int,float> weights=assignGroupWeights(scale,wm.group);
		vector<int> X  = wm.getTopSeed(k1,weights);
		vector<float> spreadofX=wm.getFi(X, m);
		unordered_set<int> seedofInfluence(X.begin(),X.end());
		unordered_map<int,int> spreadOfX=calcSpreadOf(rr,graph,seedofInfluence);
		for(int i=0;i<m;i++)
		{
			spreadofX[i]=spreadofX[i]/(graph.vi[i]-spreadS1[i]);
		}
		for(int i=0;i<m;i++)
		{
			mt[i]=spreadOfX[i]-alpha;
			lamda[i]=lamda[i]*(1-delta*mt[i]);
		}
		 for (auto& it : X) {
		   	xt[it]=xt[it]+1;
		   		}
		t++;
	}
	 for (int i=0;i<n;i++)
	 {
		 xt[i]=xt[i]/T;
	 }*/
	 return xt;
}



int main() {
   cout << "Starting..." << endl;

   Node n(1);
   string filename="in.txt";
   Graph graph(filename,10,0.5);
   //unordered_map<string,float> prob=graph.getEdgeprob();
  // unordered_map<int,vector<int>> gt=graph.transposeGraph();
   vector<int> g;
   unordered_map<int,float> weights;
   for(int i=0;i<3261;i++)
   {
	  // g.push_back(i);
	   weights[i]=1;
   }
   g=graph.groupnodes;

   unordered_map<string,float> prob=graph.getEdgeprob();
   unordered_map<int,vector<int>> gt=graph.transposeGraph();
   unordered_set<int> y2;
   RRsets rr(graph,4000,y2);
 // rr.printRRset();

  Processing p1(graph,rr);
  // unordered_set<int> s1;
  unordered_set<int> s1=p1.s1;
   cout<<"\n";
   cout<<"Size of s1 is"<<s1.size()<<"\n";
   for(const auto& elem:s1)
   {
	   cout<<"Element of s1 is"<<elem<<"\n";
   }

   unordered_map<int,int> spreadS1= calcSpreadOf(rr,graph,s1,graph.numberOfGroups);
   float lamda[10];
   for(int i=0;i<graph.numberOfGroups;i++)
   {
	   lamda[i]=1.0/(float)graph.numberOfGroups;
   }

   vector<float> r2=MWU(graph,rr,s1,100-s1.size(),0.5,lamda,spreadS1);

   for(const auto&elem:r2)
   {
	  cout<<elem<<"\n";
   }











   WeightedInfluenceMaximization wm(graph,y2);
 //  vector<int> result=wm.getTopSeed(50, weights);
   vector<int> result;
   for(int i=0;i<3621;i++)
     {
  	   g.push_back(i);
  	   weights[i]=1;
     }


  cout<<"\n";
  for(const auto&it:result)
  {
	  cout<<it<<"\n";
  }
  cout<<"End";
  vector<int> result1;
  cout<<"\n";
    for(const auto&it:result1)
    {
  	  cout<<it<<"\n";
    }
    cout<<"End";



   return 0;


}




