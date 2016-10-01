#include "stdlib.h"
#include "fstream"
#include <sstream>
#include <list>
#include <map>
#include <cstring>
#include <vector>
#include<algorithm>
#include<ctime>

#include "ns3/uinteger.h"
#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"

#include "ns3/dns-prov-module.h"
#include "ns3/dns-prov-dhlist-layerhs-online-module.h"
#include "ns3/dns-prov-dist-hlist-rmitm-module.h"

#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"
#include "ns3/helper-module.h"
#include "ns3/gnuplot.h"

#include "ns3/csma-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-address.h"


#define NET_MASK "255.255.255.0"
#define BASE_ADDR "10."
#define ADDR_SUFFIX ".0"
#define SIMULATION_LENGTH 500
#define SAMPLE_INTERVAL 1
#define BANDWIDTH_LABEL "Avg Bandwidth"
#define MBPS_UNIT 1000000
#define KBPS_UNIT 1000

#define STORAGE_PATH "/localdrive1/harshal/dns/bandwidth/"
#define GRAPHIC_PREFIX STORAGE_PATH"dns_nocomp_bandwidth_graphics/"



int m_bytesTotal = 0;
int m_bytesExpr = 0;
bool counter_trigger = false;
int counter = 0;
#define COUNTER_MAX 10


const string plotTitle = "bandwidth usage";
const string dataTitle = "bandwidth_data";

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

void ShowAppAddr(ApplicationContainer& apps, int totalNum)
{
  std::ofstream ofile("addr_mapping.txt");
  ofile << "\n" << "Mapping: (Node ID -> Application address) " << "\n";
  for (int i = 1;i <= totalNum+1;i++)
    {
      int nodeID = i;
      ofile << "Node ID: " << nodeID;
      ofile << " -> ";
      ofile << "App Ipv4ddress: " << app(nodeID)->GetAddress ();
      ofile << "\n";
    }
  ofile << "\n";
}


/* Print the routing path to a file*/
void PrintPathToFile(vector<int>& path)
{
  std::cout << "Path length: " << path.size() << std::endl;
  std::ofstream outFile(STORAGE_PATH"pathFile_nocomp.pt");
  vector<int>::iterator itr;
  for (itr = path.begin(); itr != path.end(); itr++)
    {
      outFile << *itr << "\n";
    }
  outFile.close();
}


void MeasureBandwidth(std::string context, Ptr<const Packet> packet)
{
  std::cout << "New request counted. Size: " << packet->GetSize () << std::endl; 
  m_bytesTotal += packet->GetSize ();
  m_bytesExpr += packet->GetSize ();
}

void Throughput(Gnuplot2dDataset& dataset, int totalNum, int bandw_divisor)
{
  if (Simulator::Now () > Seconds (SIMULATION_LENGTH))
    {
      return;
    }

  if (m_bytesTotal != 0)
    {
      counter_trigger = true;
    }

  //Set a counter so that the throughput calculation stops 
  //when no more traffic goes through the network
  if (m_bytesTotal == 0 && counter_trigger == true)
    {
      counter++;
    }
  else
    {
      counter = 0;
    }

  if (counter > COUNTER_MAX)
    {
      return;
    }

  double seconds = Simulator::Now().GetSeconds ();
  dataset.Add (seconds, m_bytesTotal/SAMPLE_INTERVAL/bandw_divisor);
  std::cout << std::endl << "Throughput: " << m_bytesTotal/SAMPLE_INTERVAL/bandw_divisor << std::endl;
  m_bytesTotal = 0;
  Simulator::Schedule (Seconds (SAMPLE_INTERVAL), Throughput, dataset, totalNumNodes, bandw_divisor);
}

void calculateNumNodes(int level, int fanout, int *nextNodeID)
{
  if(level > 0)
	{
		int i=0;
		int rootID = *nextNodeID;
		
		for(i=0;i<fanout;i++)
		{
			(*nextNodeID)++;
			if(level > 0)
			{

			  calculateNumNodes(level-1,fanout,nextNodeID);
			}
	
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
			  //cout<<"Adding at "<<rootID<<" "<<"."<<*nextNodeID<<currentPath<<endl;
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
			  //cout<<"Adding at "<<rootID<<" "<<*nextNodeID<<currentPath<<endl;
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

void insertRandomURL(list<string> pathList, int numURL, string mode)
{
  
  
  int rootNodeID = 1;
  int resultNodeID = 1;
  struct probvals *zdist;

  int N = pathList.size();
  zdist = (struct probvals *)malloc(N*sizeof(struct probvals));

  get_zipf(0.001,pathList.size(),zdist);

  double timer = INSERT_TIME;
  map<string,int> urlCounter;
  srand(0);

  vector<string> urlPermutation;
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

	      urlPermutation.push_back(*iter);
	      //Simulator::Schedule(Seconds(timer),insertURLTuple,rootNodeID,*iter,resultNodeID,requestID);
	      break;
	    }
	  i--;
	}
      numURL--;
      //timer+=INSERTION_TIME;
    }
  int requestCount=0;
  srand(time(0));
  random_shuffle(urlPermutation.begin(),urlPermutation.end());
  for(vector<string>::iterator iter = urlPermutation.begin(); iter!=urlPermutation.end(); iter++)
    {
      Simulator::Schedule(Seconds(timer),insertURLTuple,rootNodeID,*iter,resultNodeID,requestCount,mode);
      timer+=INSERTION_TIME;
      requestCount++;
    }
  srand(0);
    free(zdist);
}


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


/*
  Select URL's based on zipfian distribution
*/
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
  //relNames.push_back("ruleExec");
  //relNames.push_back("prov");
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

//This function is required to set up PTP network
void setBandwidthParameters(int *nextNodeID, int currentParent, int level,int fanout, PointToPointHelper& ptpHelper, vector<NetDeviceContainer>& devices, NodeContainer& ptpNodes)
{
	if(level > 0)
	{
		int i=0;
		int rootID = *nextNodeID;

		for(i=0;i<fanout;i++)
		{
			(*nextNodeID)++;
			
			  NodeContainer src = ptpNodes.Get(rootID);
			  NodeContainer dst = ptpNodes.Get(*nextNodeID);
			  cout<<"PAIR "<<rootID<<" "<<*nextNodeID<<endl;
			  NodeContainer pair = NodeContainer(src,dst);
			  NetDeviceContainer ptpDevices;
			   ptpHelper.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
			   ptpHelper.SetChannelAttribute ("Delay", StringValue ("10ms"));
			   ptpDevices = ptpHelper.Install (pair);
			   devices.push_back(ptpDevices);
			   setBandwidthParameters(nextNodeID,*nextNodeID,level-1, fanout,ptpHelper,devices,ptpNodes);
			
			
		}
	}
}



int GetDivisor(string bandwidthUnit)
{
  if (bandwidthUnit == "Bps")
    {
      return 1;
    }

  if (bandwidthUnit == "KBps")
    {
      return 1000;
    }

  if (bandwidthUnit == "MBps")
    {
      return 1000 * 1000;
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
  string mode ="nocomp";
  string graphicName = "bandwidth.pdf";
  string plotFileName = "bandwidth.pl";
  string bandwidthUnit = "Bps";

  CommandLine cmd;
  cmd.AddValue("storePth","The path to the directory for provenance storage",storePath);
  cmd.AddValue("depth","Depth of the tree",depth);
  cmd.AddValue("numNodes","Number of Nodes",numNodes);
  cmd.AddValue("fanout","Fanout of each node",fanout);
  cmd.AddValue("stopTime","Stop time of experiment",stopTime);
  cmd.AddValue("numRequests","Number of Requests",numRequests);
  cmd.AddValue("type", "Type of Experiment[nocomp,online,rmitm]", mode);
  cmd.Parse(argc,argv);
  
  totalNumNodes = numNodes;
  cout<<"CALCULATE NUM NODES "<<totalNumNodes<<endl;

  Ptr<RapidNetApplicationHelper> appHelper;
  if(mode == "online")
    appHelper= Create<DnsProvDhlistLayerhsOnlineHelper> ();
  else if(mode == "rmitm")
    appHelper = Create<DnsProvDistHlistRmitmHelper>();
  else
    appHelper = Create<DnsProvHelper>();
  
  NodeContainer ptpNodes;
  ptpNodes.Create(500);
  PointToPointHelper ptpHelper;
  std::vector<NetDeviceContainer> devices;
  int childNodeID = 0;
  int currentParent =0;
  setBandwidthParameters(&childNodeID,currentParent,depth,fanout, ptpHelper,devices,ptpNodes);

    if(numNodes == -1)
    Simulator::Schedule(Seconds(0.01),UpdateTable,depth,depth+1,fanout,fanout+1,numRequests);
  else
    Simulator::Schedule(Seconds(0.01),UpdateTableRandomTree,numNodes,fanout,fanout+1,numRequests,mode);
    //Simulator::Schedule(Seconds(0.01),UpdateTable,depth,depth+1,fanout,fanout+1,numRequests);
  //apps = InitRapidNetApps (800, Create<DnsProvHelper> ());
   InternetStackHelper stack;
  stack.Install (ptpNodes);

  Ipv4AddressHelper address;
  std::vector<NetDeviceContainer>::iterator itr;
  int lowNetworkID = 0;
  int mediumNetworkID = 0;
  int highNetworkID = 0;
  int maxSegment = 256;
  std::cout <<"DEVICE SIZE "<< devices.size ()<<endl;
  for (itr = devices.begin(); itr != devices.end(); itr++)
  {
    std::ostringstream baseAddr;
    baseAddr << BASE_ADDR << highNetworkID << "."<< lowNetworkID<<".0";
    
    address.SetBase (baseAddr.str().c_str(), NET_MASK);
    address.Assign (*itr);    
    lowNetworkID++;
    if (lowNetworkID == maxSegment)
      {
        lowNetworkID = 0;
        //mediumNetworkID++;
	//if(mediumNetworkID == maxSegment)
	//{
	//  mediumNetworkID = 0;
	    highNetworkID++;
	    //}
      }

    if (highNetworkID == maxSegment)
      {
        std::cerr << "Too many links!" << std::endl;
        return 0;
      }
  }
  
  cout<<"BEFORE IPV4Helper thing\n";
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  cout<<"Before INstalling ptpNodes"<<endl;
  apps = appHelper->Install (ptpNodes);
  cout<<"After INstalling ptpNodes"<<endl;
  //Set max jitter of apps as 0
  SetMaxJitter (apps, 0);
  
  cout<<"Showing app addresses";
  
  ShowAppAddr(apps, totalNumNodes);

  //Measure bandwidth
  string config_path = "/NodeList/*/DeviceList/*/$ns3::PointToPointNetDevice/PhyRxEnd";
  //string config_path = "/NodeList/107/DeviceList/*/$ns3::PointToPointNetDevice/PhyTxEnd";
  Config::Connect (config_path, MakeCallback (&MeasureBandwidth));


  //Draw the graph
  string graphicPath = GRAPHIC_PREFIX + graphicName;
  Gnuplot plot (graphicPath);
  plot.SetTitle (plotTitle);
  string xLabel = "Time (s)";
  string yLabel = BANDWIDTH_LABEL + bandwidthUnit;
  plot.SetLegend (xLabel, yLabel);
  plot.SetTerminal ("pdf");

  //Create dataset
  Gnuplot2dDataset dataset;
  dataset.SetTitle (dataTitle);
  dataset.SetStyle (Gnuplot2dDataset::LINES_POINTS);

  int bandw_divisor = GetDivisor(bandwidthUnit);
  Simulator::Schedule (Seconds (SAMPLE_INTERVAL), Throughput, dataset, totalNumNodes, bandw_divisor);

  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (INSERT_TIME+stopTime));

  //  Simulator::Schedule (Seconds(499.0000), SerializeProv, storePath,mode);  
  Simulator::Run ();
  Simulator::Destroy ();

   //Plot the graph
  plot.AddDataset (dataset);
  string absPathPlotFile = GRAPHIC_PREFIX + plotFileName;
  std:ofstream plotFile (absPathPlotFile.c_str());
  plot.GenerateOutput (plotFile);
  plotFile.close ();
  
  std::cout << "Total bandwidth: " << m_bytesExpr << "Bytes" << std::endl;
  return 0;
 
}
