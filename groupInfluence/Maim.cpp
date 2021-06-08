/*
 * Maim.cpp
 *
 *  Created on: Feb 24, 2021
 *      Author: HP
 */

#include <iostream>
#include "Node.h"
#include "Graph.h"
#include "string.h"
#include "RRsets.h"
#include <fstream>
#include "WeightedInfluenceMaximization.h"
#include <math.h>
#include "Processing.h"
#include "Simulation.h"
#define alpha 0.63
#include <chrono>
using namespace std;
using namespace std::chrono;

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
				break;
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
unordered_map<int,float> SIMassignGroupWeights(int nodes,unordered_map<int,unordered_set<int>> groupinfo)
{
	unordered_map<int,float> result;
	for(int i=0;i<nodes;i++)
	{
		for(int j=0;j<groupinfo.size();j++)
		{
			unordered_set<int> nodesOfgroup=groupinfo[j];
			if(nodesOfgroup.find(i)!=nodesOfgroup.end())
			{
				result[i]=1.0;
				break;
			}
		}
		if(result.find(i)==result.end())
		{
			result[i]=0.0;
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


vector<float> SimulationMWU(Graph &graph,unordered_set<int> &s1,int k1,float delta,float lamda[],unordered_map<int,float> &spreadS1,int numSim)
{
	int m=graph.numberOfGroups;
	int n=graph.nodes;
	vector<float> xt(n);
	double T=2.0*(log(m)/(delta*delta));
	//WeightedInfluenceMaximization wm(graph,s1);
	Simulation sim(graph);
	unordered_map<int,float> weights=SIMassignGroupWeights(graph.nodes,graph.groupinfo);
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


			vector<int> X;
			unordered_set<int> seedX=sim.InfluenceMaximization(weights,scale,k1,numSim);
			X.clear();
			for(const auto&elem:seedX)
			{
				X.push_back(elem);
			}

			//copy(seedX.begin(), seedX.end(), X.begin());
			cout<<"\n";
			cout<<"Size of X is"<<X.size()<<"\n";

			vector<int> spreadOfX=sim.NtimescalcInf(seedX, weights, numSim);
			cout<<"\n";
			for(int i=0;i<spreadOfX.size();i++)
			   {
				   cout<<i<<"->"<<spreadOfX[i]<<" "<<"\n";
			   }


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



	// SPEED OF ONE INFLUENCE
	/*
	string filename="result5.txt";
	int k=50;
	int numberOfSimulation=50;
	Graph graph(filename,4,0.5);
	auto start = high_resolution_clock::now();

	Simulation sim(graph);
	unordered_map<int,float> weights;
	float scale[6];
	for(int i=0;i<6;i++)
	{
		scale[i]=1;
	}
	for(int i=0;i<=830;i++)
	{
		weights[i]=1.0;
	}
	unordered_set<int> seed= sim.InfluenceMaximization(weights,scale,k,numberOfSimulation);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout<<"\n"<<"Size of sim result is : "<<seed.size()<<"\n";

	cout << "Time taken by function: "
	        << duration.count() << " microseconds" << endl;
	        */



 //Using Simulation
   cout << "Starting..." << endl;
   auto start = high_resolution_clock::now();
   string filename="result5.txt";
   int k=50;
   int numberOfSimulation=1;
   cout<<"Value of k is :"<<k<<"\n";
   cout<<"Number os dimulations is :"<<numberOfSimulation<<"\n";
   Graph graph(filename,4,0.5);

   unordered_set<int> s1;
   //unordered_set<int> seed;
   //seed.insert(45);
   Simulation sim(graph);


   float lamda[graph.numberOfGroups];
   unordered_map<int,float> spreadS1; //MAKE A FUNCTION TO CALC SPREAD OF S1
   for(int i=0;i<graph.numberOfGroups;i++)
   {
	   lamda[i]=1.0/(float)graph.numberOfGroups;
	   spreadS1[i]=0.0;
   }
   int k1=k-s1.size();
   cout<<"caaling MWU function";
  vector<float> r2=SimulationMWU(graph,s1,k1,0.5,lamda,spreadS1,numberOfSimulation);
   cout<<"Function call done";
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
	  if(pgenerated<elem)
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

   unordered_map<int,float> weights=SIMassignGroupWeights(graph.nodes,graph.groupinfo);
   vector<int> res=sim.NtimescalcInf(s2, weights, numberOfSimulation);
   for(int i=0;i<res.size();i++)
   {
	   cout<<i<<"->"<<res[i]<<" "<<"\n";
   }
   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<microseconds>(stop - start);

   cout << "Time taken by function: "
        << duration.count() << " microseconds" << endl;
        //Simulation ends




 //******************************************USING RRSETS**********************************
/*
   Node n(1);
   string filename="result3.txt";
   int NumberOfgroups=4;
   float beta=1.0;
   float epsilon=0.4;
   int k=200;

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
	  if(pgenerated<elem)
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

   //RRsets ends*/


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




