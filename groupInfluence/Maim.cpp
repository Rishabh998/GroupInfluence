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

unordered_map<int,float> calcSpreadOf(RRsets &rr,Graph &g,unordered_set<int> &s1,int m)
{
	unordered_map<int,float> result;
	unordered_map<int,vector<int>> r;
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
						//unordered_set<int> elements;
						vector<int> elements;
						elements.push_back(grpNode);
						r[crrgroup]=elements;
					}
					else
					{
						r[crrgroup].push_back(grpNode);
					}
				}

			}
		}
		index++;

	}
	cout<<"\n";
	int total=0;
	cout<<"Size of r is "<<r.size()<<"\n";

	for(const auto &[key,value]:r)
	{
		int R=groupinfo[key].size()*3;
		result[key]=(float)value.size()*((float)groupinfo[key].size()/(float)R);
		//result[key]=(float)value.size()*(float)(1.0/3.0);
		total+=result[key];
		//cout<<"Spread "<<key<<" :"<<result[key]<<"\n";
	}
	for(int i=0;i<m;i++)
	{
		if(result.find(i)==result.end())
		{
			result[i]=0;
		}
		cout<<i<<" =>"<<result[i]<<"\n";
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

vector<float> MWU(Graph &graph,RRsets &rr,unordered_set<int> &s1,int k1,float delta,float lamda[],unordered_map<int,float> &spreadS1)
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
			cout<<rr.getRRset().size()<<"\n";
			unordered_map<int,float> spreadOfX=calcSpreadOf(rr,graph,seedofInfluence,m);
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

			cout<<"\n";
			cout<<"Value of T is"<<t<<"\n";
			t++;
		}
		for (int i=0;i<n;i++)
		{
			xt[i]=xt[i]/T;
		}
	 return xt;
}



int main() {
   cout << "Starting..." << endl;

   Node n(1);
   string filename="in.txt";
   int NumberOfgroups=10;
   float beta=1.0;
   float epsilon=0.4;
   int k=150;

   Graph graph(filename,NumberOfgroups,0.5);
   //unordered_map<string,float> prob=graph.getEdgeprob();
  // unordered_map<int,vector<int>> gt=graph.transposeGraph();
   vector<int> g;

   g=graph.groupnodes;

   unordered_map<string,float> prob=graph.getEdgeprob();
   unordered_map<int,vector<int>> gt=graph.transposeGraph();
   unordered_set<int> y2;
   RRsets rr(graph,0,y2,true);
 // rr.printRRset();

  Processing p1(graph,rr,k);
  // unordered_set<int> s1;
  unordered_set<int> s1=p1.s1;
   cout<<"\n";
   cout<<"Size of s1 is"<<s1.size()<<"\n";
   for(const auto& elem:s1)
   {
	   cout<<"Element of s1 is"<<elem<<"\n";
   }
   cout<<"\n"<<"**calculating spread of s1"<<"\n";
   unordered_map<int,float> spreadS1= calcSpreadOf(rr,graph,s1,graph.numberOfGroups);
   float lamda[graph.numberOfGroups];
   for(int i=0;i<graph.numberOfGroups;i++)
   {
	   lamda[i]=1.0/(float)graph.numberOfGroups;
   }
   int k1=k-s1.size();
   vector<float> r2=MWU(graph,rr,s1,k1,0.5,lamda,spreadS1);
   float eta=1.0-(sqrt(log(k1)/(float)k1));
   unordered_set<int> s2;
   int startIndex=0;
   for(auto&elem:r2)
   {
	   //elem=elem*eta; //change eta here
	   elem=elem*1;
	   if(elem!=0)
	   {
		   cout<<elem<<"\n";
	   }

	  int randNum = (rand() % 1000) + 0;
	  float pgenerated=(float)randNum/1000;
	  if(pgenerated<=elem)
	  {
		  s2.insert(startIndex);
	  }
	  startIndex++;
   }
   cout<<"\n"<<"Elements of s2 are: "<<"\n";
   for(const auto& e2:s2)
   {
	   cout<<e2<<"\n";
   }
   unordered_set<int> s1unions2;
   set_union(s1.begin(), s1.end(),s2.begin(), s2.end(),std::inserter(s1unions2, s1unions2.begin()));
   calcSpreadOf(rr,graph,s1unions2,graph.numberOfGroups);
   cout<<"\n"<<"Size of selected nodes is :"<<s1unions2.size();


    cout<<"End";
    /*string myText;
    set<int> infseed;
    ifstream MyReadFile("seed.txt");
    while (getline (MyReadFile, myText)) {
        // Output the text from the file
        cout << myText;
        cout << "\n";
        infseed.insert(stoi(myText));

    }
    MyReadFile.close();
   // cout<<"\n"<<"Start Influence"<<"\n";
    calcSpreadOf(rr,graph,s1unions2,graph.numberOfGroups);*/



   return 0;


}




