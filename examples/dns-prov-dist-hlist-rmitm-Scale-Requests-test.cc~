#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/dns-prov-dist-hlist-rmitm-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"


#include<iostream>
#include<list>
#include<vector>
#include<time.h>
#include<sstream>
#include "fstream"
#include "stdlib.h"
#include "math.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;

using namespace ns3::rapidnet::dnsprovdisthlistrmitm;


int totalNumNodes;
ApplicationContainer apps;
//MACROS

#define url_tuple(src,url,host,requestID)\
  tuple(DnsProvDistHlistRmitm::URL,\
	attr("url_attr1",Ipv4Value,src),\
	attr("url_attr2",StrValue,url),\
	attr("url_attr3",Ipv4Value,host),\
	attr("url_attr4",Int32Value,requestID))

#define name_server_tuple(src,domain,server)\
  tuple(DnsProvDistHlistRmitm::NAME_SERVER,\
	attr("name_server_attr1",Ipv4Value,src),\
	attr("name_server_attr2",StrValue,domain),\
	attr("name_server_attr3",StrValue,server))

#define address_record_tuple(src,server,address)\
  tuple(DnsProvDistHlistRmitm::ADDRESS_RECORD,\
	attr("address_record_attr1",Ipv4Value,src),\
	attr("address_record_attr2",StrValue,server),\
	attr("address_record_attr3",Ipv4Value,address))

#define result_tuple(host,url,address)		\
  tuple(DnsProvDistHlistRmitm::RESULTS,\
	attr("result_attr1",Ipv4Value,src),\
	attr("result_attr2",StrValue,url),\
	attr("result_attr3",Ipv4Value,host))

#define insertURL(src,url,host,requestID)\
  app(src)->Insert(url_tuple(addr(src),url,addr(host),requestID))

#define insertNameServer(src,domain,server)\
  app(src)->Insert(name_server_tuple(addr(src),domain,server))

#define insertAddressRecord(src,server,address)\
  app(src)->Insert(address_record_tuple(addr(src),server,addr(address)))

#define insertResult(host,url,address)					\
  app(host)->Insert(result_record(addr(host),url,addr(address)))


void insertURLTuple(int src, string url, int host, int requestID)
{
  insertURL(src,url,host,requestID);
}


void generateTree(int *nextNodeID, string currentPath, list<string> *pathList, int level,\
			int minFanout, int maxFanout)
{
	if(level > 0)
	{
		int fanout = rand()%(maxFanout-minFanout)+minFanout;
		int i=0;
		int rootID = *nextNodeID;

		for(i=0;i<fanout;i++)
		{
			(*nextNodeID)++;
			if(level-1 > 0)
			{
			  //cout<<"Adding at "<<rootID<<" "<<"."<<*nextNodeID<<currentPath<<endl;
				stringstream newPath;
				newPath<<"."<<*nextNodeID<<currentPath;	
				insertNameServer(rootID,newPath.str(),newPath.str()+"server");
				insertAddressRecord(rootID,newPath.str()+"server",*nextNodeID);
				generateTree(nextNodeID,newPath.str(),pathList,level-1,minFanout,maxFanout);
			}
			else
			{ 
			  //cout<<"Adding at "<<rootID<<" "<<*nextNodeID<<currentPath<<endl;
				stringstream newPath;
				newPath<<*nextNodeID<<currentPath;
				insertNameServer(rootID,newPath.str(),newPath.str()+"server");
				insertAddressRecord(rootID,newPath.str()+"server",*nextNodeID);
				generateTree(nextNodeID,newPath.str(),pathList,level-1,minFanout,maxFanout);
			}	
		}
	}
	else
	{
		(*pathList).push_back(currentPath);
	}
}





void Print()
{
  //PrintRelation(apps,Dns::ADDRESS_RECORD);
  //	cout<<endl;
  //	PrintRelation(apps,Dns::NAME_SERVER);
  //	cout<<endl;
  //	PrintRelation(apps,Dns::URL);
  cout<<"RESULT TABLE"<<endl;
  PrintRelation(apps,DnsProvDistHlistRmitm::RESULTS);
  cout<<endl;
  cout<<"RULE EXEC TABLE"<<endl;
  PrintRelation(apps,DnsProvDistHlistRmitm::RULEEXEC);
  cout<<endl;
  cout<<"PROV TABLE"<<endl;
  PrintRelation(apps,DnsProvDistHlistRmitm::PROV);
  cout<<endl;
}



struct probvals
{
  float prob;
  float cum_prob;
};

void get_zipf(float theta, int N, struct probvals *zdist)
{
   float sum = 0.0;
  float c = 0.0;
  float expo;
  float sumc = 0.0;

  int i=0;
  
  expo = 1- theta;
  
  for(i=1; i<=N;i++)
    sum+=1.0/(float)pow((double)i,(double)expo);

  c = 1.0/sum;

  for(i=0; i<N; i++)
    {
      zdist[i].prob = c/(float)pow((double)(i+1),(double)(expo));
      sumc += zdist[i].prob;
      zdist[i].cum_prob = sumc;
    }
}

#define INSERT_TIME 2
#define INSERTION_TIME 0.001

void insertRandomURL(list<string> pathList, int numURL)
{
  
  
  int rootNodeID = 2;
  int resultNodeID = 1;
  struct probvals *zdist;

  int N = pathList.size();
  zdist = (struct probvals *)malloc(N*sizeof(struct probvals));

  get_zipf(0.001,pathList.size(),zdist);

  double timer = INSERT_TIME;
  map<string,int> urlCounter;
  srand(0);
  while(numURL > 0)
    {
      double probRand = ((double) rand() / (RAND_MAX));
      
      int i=0;

      for(;i<pathList.size();i++)
	{
	  if(zdist[i].cum_prob >= probRand)
	    {
      	      break;
	    }
	}

      list<string>::iterator iter = pathList.begin();

      for(;iter!=pathList.end();iter++)
	{
	  if(i == 0)
	    {
	      int requestID = 0;
	      if(urlCounter.count(*iter) > 0)
		requestID = urlCounter[*iter];
	      else
		urlCounter[*iter] = 0;
	      
	      urlCounter[*iter]++;

	      Simulator::Schedule(Seconds(timer),insertURLTuple,rootNodeID,*iter,resultNodeID,requestID);
	      break;
	    }
	  i--;
	}
      numURL--;
      timer+=INSERTION_TIME;
    }
    free(zdist);
}


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
      // cout<<"Adding at "<<rootID<<" "<<"."<<*nextNodeID<<currentPath<<endl;
      stringstream newPath;
      newPath<<"."<<*nextNodeID<<currentPath;	
      insertNameServer(rootID,newPath.str(),newPath.str()+"server");
      insertAddressRecord(rootID,newPath.str()+"server",*nextNodeID);
      generateRandomTree(nextNodeID,newPath.str(),pathList,minFanout,maxFanout,elementCounter[i]);
    }
  delete elementCounter;
}


void UpdateTable(int minDepth, int maxDepth, int minFanout, int maxFanout, int numRequests)
{
  int nextNodeID = 2;
  int rootNodeID = 2;
  string path = ".";
  list<string> pathList;

  int level = rand()%(maxDepth-minDepth)+minDepth;
 
  generateTree(&nextNodeID,path,&pathList,level,minFanout,maxFanout);
  totalNumNodes = nextNodeID;
  
  list<string>::iterator iter = pathList.begin();
  int resultNodeID = 1;
  int counter = 0;
  double timer = 0;
  /*for(;counter<50;counter++)
    {
      int numRequests = 0;
      for(;numRequests < 10;numRequests++,timer+=0.01)
	{
	   Simulator::Schedule(Seconds(timer),insertURLTuple,rootNodeID,*iter,resultNodeID,numRequests);
  
	}
      iter++;
      }*/
  insertRandomURL(pathList,numRequests);
}


void UpdateTableRandomTree(int numNodes, int minFanout, int maxFanout, int numRequests)
{
  int nextNodeID = 2;
  int rootNodeID = 2;
  string path = ".";
  list<string> pathList;
 
  generateRandomTree(&nextNodeID,path,&pathList,minFanout,maxFanout,numNodes);
  totalNumNodes = nextNodeID;
  
  list<string>::iterator iter = pathList.begin();
  int resultNodeID = 1;
  int counter = 0;
  double timer = 0;
  insertRandomURL(pathList,numRequests);
}



void SerializeProv(string storePath)
{
  cout<<"NUM NODES "<<totalNumNodes<<endl;
  cout<<"STORE PATH "<<storePath<<endl;
  vector<string> relNames;
  relNames.push_back("ruleExec");
  relNames.push_back("prov");

  for (int i = 2; i < totalNumNodes+1; i++)
    {
      int node = i;
      
      app(node) -> SerializeRel(relNames, node, storePath);
      
    }
}


int main(int argc,char *argv[])
{
  LogComponentEnable("DnsProv", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);
  

  string storePath = "/localdrive1/harshal/prov_test/";
  uint32_t depth = 3;
  uint32_t fanout = 2;
  uint32_t stopTime = 500;
  uint32_t numRequests = 10;
  uint32_t numNodes = -1;
  uint32_t timedRequests = 0;

  CommandLine cmd;
  cmd.AddValue("storePth","The path to the directory for provenance storage",storePath);
  cmd.AddValue("depth","Depth of the tree",depth);
  cmd.AddValue("numNodes","Number of Nodes",numNodes);
  cmd.AddValue("fanout","Fanout of each node",fanout);
  cmd.AddValue("stopTime","Stop time of experiment",stopTime);
  cmd.AddValue("numRequests","Number of Requests",numRequests);
  cmd.AddValue("timedRequests","Insert Requests wrt time",timedRequests);
  cmd.Parse(argc,argv);
  
  if(stopTime!=-1 && timedRequests == 1)
    {
      numRequests = stopTime/INSERTION_TIME;
    }

  NodeContainer csmaNodes;
  csmaNodes.Create (5000);

  CsmaHelper csma;
  csma.SetDeviceAttribute ("EncapsulationMode", StringValue ("Dix"));
  csma.SetDeviceAttribute ("FrameSize", UintegerValue (64000));

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

  InternetStackHelper stack;
  stack.Install (csmaNodes);

  Ipv4AddressHelper address;

  address.SetBase ("192.0.0.0", "255.0.0.0");
  address.Assign (csmaDevices);

  Ptr<RapidNetApplicationHelper> appHelper = Create<DnsProvDistHlistRmitmHelper> ();
  apps = appHelper->Install (csmaNodes);

  
  if(numNodes == -1)
    Simulator::Schedule(Seconds(0.01),UpdateTable,depth,depth+1,fanout,fanout+1,numRequests);
  else
    Simulator::Schedule(Seconds(0.01),UpdateTableRandomTree,numNodes,fanout,fanout+1,numRequests);
  
  //apps = InitRapidNetApps (800, Create<DnsProvHelper> ());
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (INSERT_TIME + stopTime));
 
  Simulator::Schedule (Seconds(INSERT_TIME + stopTime -1), SerializeProv, storePath);
  schedule (INSERT_TIME+stopTime, Print);
  Simulator::Run ();
  cout<<"Total Number Nodes "<<totalNumNodes<<endl;
  Simulator::Destroy ();
  return 0;
}
