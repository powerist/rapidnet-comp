#include<iostream>
#include<list>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sstream>

using namespace std;

void generateRandomTree(int *nextNodeID, string currentPath, list<string>* pathList, int minFanout, int maxFanout, int totalNumNodes)
{
  if(totalNumNodes == 1)
    {
      (*pathList).push_back(currentPath);
      return;
    }

  int fanout = rand()%(maxFanout-minFanout)+minFanout;
  int *elementCounter = new int[fanout];
  
  int rootID = *nextNodeID;
  for(int i=0; i<fanout;i++)
    elementCounter[i]=0;

  for(int i=0;i<totalNumNodes-1; i++)
    {
      int index = rand()%fanout;
      elementCounter[index]++;
    }

  for(int i=0; i <fanout;i++)
    {
      if(elementCounter[i]==0)
	continue;
      (*nextNodeID)++;
      cout<<"Adding at "<<rootID<<" "<<"."<<*nextNodeID<<currentPath<<endl;
      stringstream newPath;
      newPath<<"."<<*nextNodeID<<currentPath;	
      generateRandomTree(nextNodeID,newPath.str(),pathList,minFanout,maxFanout,elementCounter[i]);
    }
  delete elementCounter;
}

void generateTree(int *nextNodeID, string currentPath, list<string> *pathList, int level,\
			int minFanout, int maxFanout)
{
	if(level > 0)
	{
		int fanout = rand()%(maxFanout-minFanout)+minFanout;
		cout<<"FANOUT "<<fanout<<endl;
		int i=0;
		int rootID = *nextNodeID;

		for(i=0;i<fanout;i++)
		{
			(*nextNodeID)++;
			if(level-1 > 0)
			{
				cout<<"Adding at "<<rootID<<" "<<"."<<*nextNodeID<<currentPath<<endl;
				stringstream newPath;
				newPath<<"."<<*nextNodeID<<currentPath;	
				generateTree(nextNodeID,newPath.str(),pathList,level-1,minFanout,maxFanout);
			}
			else
			{ 
				cout<<"Adding at "<<rootID<<" "<<*nextNodeID<<currentPath<<endl;
				stringstream newPath;
				newPath<<*nextNodeID<<currentPath;	
				generateTree(nextNodeID,newPath.str(),pathList,level-1,minFanout,maxFanout);
			}	
		}
	}
	else
	{
		(*pathList).push_back(currentPath);
	}
}

int main(int argc, char* argv[])
{

	srand(time(NULL));

	if(argc == 4)
	{
		int nextNodeID = 2;
		string path = ".";
		list<string> pathList;

		int numNodes = atoi(argv[1]);
		
		int minFanout = atoi(argv[2]);
		int maxFanout = atoi(argv[3]);
		generateRandomTree(&nextNodeID,path,&pathList,minFanout,maxFanout,numNodes);
		cout<<"TOTAL NUMBER OF NODES : "<<nextNodeID<<endl;
		list<string>::iterator iter = pathList.begin();
		for(;iter!=pathList.end();iter++)
			cout<<"URL "<<*iter<<endl;
	}
	else
	{
		cout<<"USAGE : treeGenerator totalNumNodes minFanout maxFanout"<<endl;
	}
	return 0;
}
