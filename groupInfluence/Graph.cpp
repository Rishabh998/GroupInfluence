/*
 * Graph.cpp
 *
 *  Created on: Feb 24, 2021
 *      Author: HP
 */

#include "Graph.h"
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <unordered_set>


Graph::Graph(string file,int numberOfGroups,float beta) {
	// TODO Auto-generated constructor stub

		nodes=0;
		edges=0;
		makegraph(file);
		this->gtranspose=transposeGraph();

		//RRsets r1(this->nodes,this->prob,this->gtranspose,this->nodes,this->groupnodes,r);

		this->numberOfGroups= numberOfGroups;

		//this->vi[0]=3.0;
		//this->vi[1]=3.0;
		assignGroups( numberOfGroups,1);
		assignVis(0.3);
      /*
		for(int i=0;i<nodes;i++)
		{
			int randNum = (rand() % 10) + 0;
			vector<int> p;
			p.push_back(0);
			groupmap[0]=p;
			if(i==0)
			{
				groupnodes.push_back(0);
				continue;
			}

					if(randNum>5)
					{
						vector<int> a;
						groupnodes.push_back(i);
						int randGroup = (rand() % groupSize);
						a.clear();
						a.push_back(randGroup);
						groupmap[i]=a;

					}

			vector<int> a;
			a.clear();
			if(i%2==0)
			{
				a.push_back(1);
			}
			else
			{
				a.push_back(0);
			}
			//groupmap[i]=a;
		}*/



		cout<<"loaded";
}

unordered_map<int,vector<int>> Graph::getGraph()
{
		return g;
}
unordered_map<int,unordered_set<int>> Graph::getGroups()
{
	return group;
}

unordered_map<string,float> Graph::getEdgeprob()
{
		return prob;
}
unordered_map<int,vector<int>> Graph::getTransposeGraph()
{
	return this->gtranspose;
}
unordered_map<int,vector<int>> Graph::transposeGraph()
{
	unordered_map<int,vector<int>> gt;
	int n=nodes;
	for(int i=0;i<n;i++)
	{
		vector<int> edges=g[i];
		for(int j=0;j<edges.size();j++)
		{
			gt[edges[j]].push_back(i);
		}
	}
	return gt;
}
void Graph::assignVis(float beta)
{
	for(int i=0;i<numberOfGroups;i++)
	{
		//this->vi[i]=beta*groupinfo[i].size();
		this->vi[i]=groupinfo[i].size()*0.5;
	}

}
void Graph::assignGroups(int m,float p)
{
	for(int i=0;i<nodes;i++)
	{
		int randNum = (rand() % 10) + 0;
		if(randNum<p*10)
		{
			groupnodes.push_back(i);
			int randGroup = (rand() % m);
			vector<int> a;
			a.clear();
			a.push_back(randGroup);

			if(groupinfo.find(randGroup)==groupinfo.end())
			{
				unordered_set<int> grpnodes;
				grpnodes.insert(i);
				groupinfo[randGroup]=grpnodes;
			}
			else
			{
				groupinfo[randGroup].insert(i);
			}
			this->groupmap[i]=a;
		}

	}


}
void Graph::printGraph(unordered_map<int,vector<int>> g)
{
	   for (const auto & [ key, value ] : g)  {
		   vector<int> a=value;
	   for (const auto& elem: a) {
	       /* ... process elem ... */
		   cout << key<<" " << elem;
	   }
	   cout<<"\n";
	   }


}


unordered_set<int> Graph::makeS1()
{
	unordered_set<int> result;


	cout<<"hello";
	//RRsets rr(this->nodes,this->prob,this->gtranspose,this->nodes,this->groupnodes,r);
	//vector<unordered_set<int>> rrset=rr.getRRset();
	//unordered_map<int,int> rmap=rr.getIndexofRRsets();
	int index=0;
	/*
	for(const auto&it:rrset)
	{
		int grpNode=rmap[index];
		vector<int> nodeBelongstoGroups=this->groupmap[grpNode];
		unordered_set<int> crrset=it;
		for(const auto&node:crrset)
		{
			for(const auto&groups:nodeBelongstoGroups)
			{
				if(nodeinfo.find(node)!=nodeinfo.end())
				{
					unordered_map<int,int> countMap=nodeinfo[node];
					if(countMap.find(groups)!=countMap.end())
					{
						countMap[groups]++;
					}
					else
					{
						countMap[groups]=1;
					}
				}
				else
				{
					unordered_map<int,int> countMap;
					countMap[groups]=1;
					nodeinfo[node]=countMap;
				}
			}
		}

	}
*/
	//Node info created . create list
	return result;
}
void Graph::makegraph(string s)
{
	std::ifstream file(s);
	std::string str;
	int c=0;
	//Graph g;
	while (std::getline(file, str)) {
		istringstream iss(str);
		vector<string> tokens{istream_iterator<string>{iss},
		                      istream_iterator<string>{}};
			if(c==0)
			{
				nodes=stoi(tokens[0]);
				edges=stoi(tokens[1]);
				/*for (auto& it : tokens) {
						        cout << it << "\n";
						    }*/

			}
			else
			{
				int u=stoi(tokens[0]);
				int v=stoi(tokens[1]);
				float p=0;
				if(tokens.size()>=3)
				{
					p=stof(tokens[2]);
					string s1=to_string(u)+','+to_string(v);
					//string srev=to_string(v)+','+to_string(u);
					prob[s1]=p;

				}
				if(g.find(u)==g.end()) //doesnot contains
				{
					vector<int> edges;
					edges.push_back(v);
					g[u]=edges;
				}
				else
				{
					g[u].push_back(v);
				}


			}
	  c++;
	}

}

