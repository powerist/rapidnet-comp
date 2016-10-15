#include<iostream>
#include<map>
#include<vector>
#include<algorithm>
#include<cstdlib>

using namespace std;


void findNonConnectedPairs(vector< pair<int,int> >& nonConnectedPairs, map<int,int> rtables[],int table_size)
{
  for(int i=0; i<table_size; i++)
    {
      map<int,int> currentPair = rtables[i];
      map<int,int>::iterator iter = currentPair.begin();
      for(;iter!=currentPair.end();iter++)
	{
	  int dst = iter->first;
	  int next = iter->second;
	  
	  //Check if dst and next are not the same, which is same as sayinig i and dst are not neighbors
	  if(dst != next)
	    {
	      //Check if it is already present
	      bool found = false;

	      for(vector< pair<int,int> >::iterator iter = nonConnectedPairs.begin(); iter!=nonConnectedPairs.end(); iter++)
		{
		  pair<int,int> currentPair = *iter;
		  if(currentPair.first == dst && currentPair.second == i)
		    {
		      found = true;
		      break;
		    }
		}

	      if(!found)
		{
		  pair<int,int> newPair(i,dst);
		  nonConnectedPairs.push_back(newPair);
		}
	    }
	}
    }
}

#define TIME_STEP_FLOW_ENTRY 10
void changeFlowEntries(vector< pair<int,int> > nonConnectedTables, int numEntriesEntered)
{
  srand(0);
  random_shuffle(nonConnectedTables.begin(),nonConnectedTables.end());
  int counter = numEntriesEntered;
  int timer = 0;
  for(vector< pair<int,int> >::iterator iter = nonConnectedTables.begin(); iter != nonConnectedTables.end() && counter>0; iter++, counter--)
    {
      cout<<"INSERTING "<<iter->first<<" "<<iter->second<<" at "<<timer<<endl;
      timer+=TIME_STEP_FLOW_ENTRY;
    }
}
int main()
{
  vector< pair<int,int> > newPoints;
  int size=4;
  map<int,int> rtables[4];

  rtables[0][0]=0;
  rtables[0][1]=1;
  rtables[0][2]=1;
  rtables[0][3]=3;
  
  rtables[1][0]=0;
  rtables[1][1]=1;
  rtables[1][2]=2;
  rtables[1][3]=0;

  rtables[2][0]=1;
  rtables[2][1]=1;
  rtables[2][2]=2;
  rtables[2][3]=1;

  rtables[3][0]=0;
  rtables[3][1]=0;
  rtables[3][2]=0;
  rtables[3][3]=3;

  findNonConnectedPairs(newPoints,rtables,size);

  for(vector< pair<int,int> >::iterator iter= newPoints.begin() ; iter!=newPoints.end() ;iter++)
    {
      cout<<iter->first<<" "<<iter->second<<endl;
    }

  changeFlowEntries(newPoints,2);
}
