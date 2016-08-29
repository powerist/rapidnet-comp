#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"

#include "ns3/dns-prov-module.h"
#include "ns3/dns-prov-dhlist-layerhs-online-module.h"
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

using namespace ns3::rapidnet::dnsprov;
using namespace ns3::rapidnet::dnsprovdhlistlayerhsonline;
using namespace ns3::rapidnet::dnsprovdisthlistrmitm;


int totalNumNodes;
ApplicationContainer apps;
//MACROS

#define url_tuple_nocomp(src,url,host,requestID)\
  tuple(DnsProv::URL,\
	attr("url_attr1",Ipv4Value,src),\
	attr("url_attr2",StrValue,url),\
	attr("url_attr3",Ipv4Value,host),\
	attr("url_attr4",Int32Value,requestID))

#define name_server_tuple_nocomp(src,domain,server)\
  tuple(DnsProv::NAME_SERVER,\
	attr("name_server_attr1",Ipv4Value,src),\
	attr("name_server_attr2",StrValue,domain),\
	attr("name_server_attr3",StrValue,server))

#define address_record_tuple_nocomp(src,server,address)\
  tuple(DnsProv::ADDRESS_RECORD,\
	attr("address_record_attr1",Ipv4Value,src),\
	attr("address_record_attr2",StrValue,server),\
	attr("address_record_attr3",Ipv4Value,address))

#define result_tuple_nocomp(host,url,address)		\
  tuple(DnsProv::RESULT,\
	attr("result_attr1",Ipv4Value,src),\
	attr("result_attr2",StrValue,url),\
	attr("result_attr3",Ipv4Value,host))

#define insertURL_nocomp(src,url,host,requestID)\
  app(src)->Insert(url_tuple_nocomp(addr(src),url,addr(host),requestID))

#define insertNameServer_nocomp(src,domain,server)\
  app(src)->Insert(name_server_tuple_nocomp(addr(src),domain,server))

#define insertAddressRecord_nocomp(src,server,address)\
  app(src)->Insert(address_record_tuple_nocomp(addr(src),server,addr(address)))

#define insertResult_nocomp(host,url,address)					\
  app(host)->Insert(result_record_nocomp(addr(host),url,addr(address)))

//Online
#define url_tuple_online(src,url,host,requestID)\
  tuple(DnsProv::URL,\
	attr("url_attr1",Ipv4Value,src),\
	attr("url_attr2",StrValue,url),\
	attr("url_attr3",Ipv4Value,host),\
	attr("url_attr4",Int32Value,requestID))

#define name_server_tuple_online(src,domain,server)\
  tuple(DnsProv::NAME_SERVER,\
	attr("name_server_attr1",Ipv4Value,src),\
	attr("name_server_attr2",StrValue,domain),\
	attr("name_server_attr3",StrValue,server))

#define address_record_tuple_online(src,server,address)\
  tuple(DnsProv::ADDRESS_RECORD,\
	attr("address_record_attr1",Ipv4Value,src),\
	attr("address_record_attr2",StrValue,server),\
	attr("address_record_attr3",Ipv4Value,address))

#define result_tuple_online(host,url,address)		\
  tuple(DnsProv::RESULT,\
	attr("result_attr1",Ipv4Value,src),\
	attr("result_attr2",StrValue,url),\
	attr("result_attr3",Ipv4Value,host))

#define insertURL_online(src,url,host,requestID)\
  app(src)->Insert(url_tuple_online(addr(src),url,addr(host),requestID))

#define insertNameServer_online(src,domain,server)\
  app(src)->Insert(name_server_tuple_online(addr(src),domain,server))

#define insertAddressRecord_online(src,server,address)\
  app(src)->Insert(address_record_tuple_online(addr(src),server,addr(address)))

#define insertResult_online(host,url,address)					\
  app(host)->Insert(result_record_online(addr(host),url,addr(address)))


//RMITM

#define url_tuple_rmitm(src,url,host,requestID)\
  tuple(DnsProv::URL,\
	attr("url_attr1",Ipv4Value,src),\
	attr("url_attr2",StrValue,url),\
	attr("url_attr3",Ipv4Value,host),\
	attr("url_attr4",Int32Value,requestID))

#define name_server_tuple_rmitm(src,domain,server)\
  tuple(DnsProv::NAME_SERVER,\
	attr("name_server_attr1",Ipv4Value,src),\
	attr("name_server_attr2",StrValue,domain),\
	attr("name_server_attr3",StrValue,server))

#define address_record_tuple_rmitm(src,server,address)\
  tuple(DnsProv::ADDRESS_RECORD,\
	attr("address_record_attr1",Ipv4Value,src),\
	attr("address_record_attr2",StrValue,server),\
	attr("address_record_attr3",Ipv4Value,address))

#define result_tuple_rmitm(host,url,address)		\
  tuple(DnsProv::RESULT,\
	attr("result_attr1",Ipv4Value,src),\
	attr("result_attr2",StrValue,url),\
	attr("result_attr3",Ipv4Value,host))

#define insertURL_rmitm(src,url,host,requestID)\
  app(src)->Insert(url_tuple_rmitm(addr(src),url,addr(host),requestID))

#define insertNameServer_rmitm(src,domain,server)\
  app(src)->Insert(name_server_tuple_rmitm(addr(src),domain,server))

#define insertAddressRecord_rmitm(src,server,address)\
  app(src)->Insert(address_record_tuple_rmitm(addr(src),server,addr(address)))

#define insertResult_rmitm(host,url,address)					\
  app(host)->Insert(result_record_rmitm(addr(host),url,addr(address)))


string global_mode;
void insertURLTuple(int src, string url, int host, int requestID, string mode)
{
  if(mode == "online")
    {
      insertURL_online(src,url,host,requestID);
    }
  else
    {
      if(mode == "rmitm")
	{
	  insertURL_rmitm(src,url,host,requestID);
	}
      else
	{
	  insertURL_nocomp(src,url,host,requestID);
	}
    }
}

/*
Function to generate a random tree with given level and the fanout chosen randomly. Recursively build each level in a Depth first fashion from left to right
*/
void generateTree(int *nextNodeID, string currentPath, list<string> *pathList, int level,\
		  int minFanout, int maxFanout,string mode)
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
			  //This case is for all internal nodes
				stringstream newPath;
				newPath<<"."<<*nextNodeID<<currentPath;	
				if(mode == "online")
				  {
				    insertNameServer_online(rootID,newPath.str(),newPath.str()+"server");
				    insertAddressRecord_online(rootID,newPath.str()+"server",*nextNodeID);
				  }
				else
				  {
				    if(mode == "rmitm")
				      {
					insertNameServer_rmitm(rootID,newPath.str(),newPath.str()+"server");
					insertAddressRecord_rmitm(rootID,newPath.str()+"server",*nextNodeID);
				      }
				    else
				      {
					insertNameServer_nocomp(rootID,newPath.str(),newPath.str()+"server");
					insertAddressRecord_nocomp(rootID,newPath.str()+"server",*nextNodeID);
				      }
				  }
				generateTree(nextNodeID,newPath.str(),pathList,level-1,minFanout,maxFanout,mode);
			}
			else
			{ 
			  // This case is for leaf nodes so the URL has a . added at the end.
				stringstream newPath;
				newPath<<*nextNodeID<<currentPath;
				if(mode == "online")
				  {
				    insertNameServer_online(rootID,newPath.str(),newPath.str()+"server");
				    insertAddressRecord_online(rootID,newPath.str()+"server",*nextNodeID);
				  }
				else
				  {
				    if(mode == "rmitm")
				      {
					insertNameServer_rmitm(rootID,newPath.str(),newPath.str()+"server");
					insertAddressRecord_rmitm(rootID,newPath.str()+"server",*nextNodeID);
				      }
				    else
				      {
					insertNameServer_nocomp(rootID,newPath.str(),newPath.str()+"server");
					insertAddressRecord_nocomp(rootID,newPath.str()+"server",*nextNodeID);
				      }
				  }
				
				generateTree(nextNodeID,newPath.str(),pathList,level-1,minFanout,maxFanout,mode);
			}	
		}
	}
	else
	{
	  //Add URL to a vector
		(*pathList).push_back(currentPath);
	}
}

/*This function generates a Random tree with fixed number of total nodes. Basically divide the nodes randomly in random number of subtrees to generate varying length URLs*/
void generateRandomTree(int *nextNodeID, string currentPath, list<string>* pathList, int minFanout, int maxFanout, int totalNumNodes, string mode)
{
  if(totalNumNodes == 1)
    {
      (*pathList).push_back(currentPath);
      return;
    }
  //Randomly generate the fanout
  int fanout = rand()%(maxFanout-minFanout)+minFanout;
  int *elementCounter = new int[fanout];
  
  int rootID = *nextNodeID;
  for(int i=0; i<fanout;i++)
    elementCounter[i]=0;

  //elementCounter is the number of nodes in each subtree
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
      //For each subtree recursively generate a random tree using elementCounter[i] nodes
      stringstream newPath;
      newPath<<"."<<*nextNodeID<<currentPath;
      if(mode == "online")
      {	
	insertNameServer_online(rootID,newPath.str(),newPath.str()+"server");
	insertAddressRecord_online(rootID,newPath.str()+"server",*nextNodeID);
      }
      else
	{
	  if(mode == "rmitm")
	    {
	      insertNameServer_rmitm(rootID,newPath.str(),newPath.str()+"server");
	      insertAddressRecord_rmitm(rootID,newPath.str()+"server",*nextNodeID);
	    }
	  else
	    {
	      insertNameServer_nocomp(rootID,newPath.str(),newPath.str()+"server");
	      insertAddressRecord_nocomp(rootID,newPath.str()+"server",*nextNodeID);
	    }
	}
				
      generateRandomTree(nextNodeID,newPath.str(),pathList,minFanout,maxFanout,elementCounter[i],mode);
    }
  delete elementCounter;
}

struct probvals
{
  float prob;
  float cum_prob;
};

//Generate a zipfian distribution for number of URL's genrated by the random tree
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

/*
  Select URL's based on zipfian distribution
*/
void insertRandomURL(list<string> pathList, int numURL, string mode)
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
	  //If he cum probability is greater than the random number generated, insert this URL
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

	      Simulator::Schedule(Seconds(timer),insertURLTuple,rootNodeID,*iter,resultNodeID,requestID,mode);
	      break;
	    }
	  i--;
	}
      numURL--;
      timer+=INSERTION_TIME;
    }
    free(zdist);
}
//Main function to generate tree and randomly insert the URLs
void UpdateTable(int minDepth, int maxDepth, int minFanout, int maxFanout, int numRequests)
{
  int nextNodeID = 2;
  int rootNodeID = 2;
  string path = ".";
  list<string> pathList;
  string mode = global_mode;
  int level = rand()%(maxDepth-minDepth)+minDepth;
 
  generateTree(&nextNodeID,path,&pathList,level,minFanout,maxFanout,mode);
  totalNumNodes = nextNodeID;
 
  list<string>::iterator iter = pathList.begin();
  int resultNodeID = 1;
  int counter = 0;
  double timer = 0;
 
  insertRandomURL(pathList,numRequests,mode);
}

//Main function to generate tree based on fixed number of nodes
void UpdateTableRandomTree(int numNodes, int minFanout, int maxFanout, int numRequests, string mode)
{
  int nextNodeID = 2;
  int rootNodeID = 2;
  string path = ".";
  list<string> pathList;
 
  generateRandomTree(&nextNodeID,path,&pathList,minFanout,maxFanout,numNodes,mode);
  totalNumNodes = nextNodeID;
   cout<<"NUMBER OF URL's "<<pathList.size()<<endl;
  int height = 0;
  for(list<string>::iterator iter1 = pathList.begin(); iter1 != pathList.end(); iter1++)
    {
      if((*iter1).length() > height)
	{
	  height = (*iter1).length();
	}
    }
  cout<<"Height of Tree "<<height<<endl;
  list<string>::iterator iter = pathList.begin();
  int resultNodeID = 1;
  int counter = 0;
  double timer = 0;
  insertRandomURL(pathList,numRequests,mode);
}



void SerializeProv(string storePath,string mode)
{
  cout<<"NUM NODES "<<totalNumNodes<<endl;
  cout<<"STORE PATH "<<storePath<<endl;
  vector<string> relNames;

  if(mode == "nocomp" || mode == "rmitm")
    {
      relNames.push_back("ruleExec");
      relNames.push_back("prov");
    }
  else if(mode == "online")
    {
      relNames.push_back("ruleExec");
      relNames.push_back("provHashTable");
      relNames.push_back("equiHashTable");
      relNames.push_back("recvPacketQry");
      relNames.push_back("provLink");
    }
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
  string mode = "nocomp";
  CommandLine cmd;
  cmd.AddValue("storePth","The path to the directory for provenance storage",storePath);
  cmd.AddValue("depth","Depth of the tree",depth);
  cmd.AddValue("numNodes","Number of Nodes",numNodes);
  cmd.AddValue("fanout","Fanout of each node",fanout);
  cmd.AddValue("stopTime","Stop time of experiment",stopTime);
  cmd.AddValue("type", "Type of Experiment[nocomp,online,rmitm]", mode);
  cmd.Parse(argc,argv);
  

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
  
  Ptr<RapidNetApplicationHelper> appHelper;
  if(mode == "online")
    appHelper= Create<DnsProvDhlistLayerhsOnlineHelper> ();
  else if(mode == "rmitm")
    appHelper = Create<DnsProvDistHlistRmitmHelper>();
  else
    appHelper = Create<DnsProvHelper>();

  apps = appHelper->Install (csmaNodes);
  global_mode = mode;
   if(numNodes == -1)
     Simulator::Schedule(Seconds(0.01),UpdateTable,depth,depth+1,fanout,fanout+1,numRequests);
   else
     Simulator::Schedule(Seconds(0.01),UpdateTableRandomTree,numNodes,fanout,fanout+1,numRequests,mode);
  //apps = InitRapidNetApps (800, Create<DnsProvHelper> ());
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (INSERT_TIME + stopTime));
 
  Simulator::Schedule (Seconds(INSERT_TIME + stopTime -1), SerializeProv, storePath,mode);
  //  schedule (INSERT_TIME+stopTime, Print);
  Simulator::Run ();
  cout<<"Total Number Nodes "<<totalNumNodes<<endl;
  Simulator::Destroy ();
  return 0;
}
