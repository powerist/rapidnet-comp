#include "stdlib.h"
#include "fstream"
#include <sstream>
#include <list>
#include <map>
#include <cstring>
#include <vector>
#include <ctime>
#include <algorithm>

#include "ns3/uinteger.h"
#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/dns-prov-dhlist-layerhs-online-module.h"
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
#define GRAPHIC_PREFIX STORAGE_PATH"dns_online_bandwidth_graphics/"



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

using namespace ns3::rapidnet::dnsprovdhlistlayerhsonline;


int totalNumNodes;
ApplicationContainer apps;
//MACROS

#define url_tuple(src,url,host,requestID)\
  tuple(DnsProvDhlistLayerhsOnline::URL,\
	attr("url_attr1",Ipv4Value,src),\
	attr("url_attr2",StrValue,url),\
	attr("url_attr3",Ipv4Value,host),\
	attr("url_attr4",Int32Value,requestID))

#define name_server_tuple(src,domain,server)\
  tuple(DnsProvDhlistLayerhsOnline::NAME_SERVER,\
	attr("name_server_attr1",Ipv4Value,src),\
	attr("name_server_attr2",StrValue,domain),\
	attr("name_server_attr3",StrValue,server))

#define address_record_tuple(src,server,address)\
  tuple(DnsProvDhlistLayerhsOnline::ADDRESS_RECORD,\
	attr("address_record_attr1",Ipv4Value,src),\
	attr("address_record_attr2",StrValue,server),\
	attr("address_record_attr3",Ipv4Value,address))

#define result_tuple(host,url,address)		\
  tuple(DnsProvDhlistLayerhsOnline::RESULTS,\
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

#define programID(node, url, ID)\
  tuple(DnsProvDhlistLayerhsOnline::PROGRAMID,\
    attr("programID_attr1",Ipv4Value,node),\
	attr("programID_attr2",StrValue,url),	\
        attr("programID_attr3",StrValue,ID))

#define insert_programID(node,url,ID)\
  app(node)->Insert(programID(addr(node),url,ID))


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
  std::ofstream outFile(STORAGE_PATH"pathFile_online.pt");
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



char alphanum[] =
"0123456789"
"!@#$%^&*"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "abcdefghijklmnopqrstuvwxyz";

int stringLength = sizeof(alphanum) - 1;

char genRandom()  // Random string generator function.
{
  return alphanum[rand() % stringLength];
}

int randomStringLength = 2;


string generateRandomString(int length)
{
  string ans="";
  for(int i=0;i<length;i++)
    {
      ans+=genRandom();
    }

  //cout<<ans<<" Generated"<<endl;
  return ans;
}

map<string,bool> generatedID;

void SetupProgramID(list<string> pathList, int rootNodeID)
{
  srand(0);
  for(list<string>::iterator iter = pathList.begin() ; iter != pathList.end() ; iter++)
    {
              
              string progID = generateRandomString(randomStringLength);
              while(generatedID.count(progID) != 0)
                {
                  progID = generateRandomString(randomStringLength);
                }
              generatedID[progID] = true;
	      insert_programID(rootNodeID,(*iter).c_str(),progID.c_str());
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
  PrintRelation(apps,DnsProvDhlistLayerhsOnline::RESULTS);
  cout<<endl;
  cout<<"RULE EXEC TABLE"<<endl;
  PrintRelation(apps,DnsProvDhlistLayerhsOnline::RULEEXEC);
  cout<<endl;


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

   srand(time(0));
  random_shuffle(urlPermutation.begin(),urlPermutation.end());
  
  int requestCount =0;
  for(vector<string>::iterator iter = urlPermutation.begin(); iter!=urlPermutation.end(); iter++)
    {
      Simulator::Schedule(Seconds(timer),insertURLTuple,rootNodeID,*iter,resultNodeID,requestCount);
      timer+=INSERTION_TIME;
      requestCount++;
    }

  srand(0);
    free(zdist);
}

void UpdateTable(int minDepth, int maxDepth, int minFanout, int maxFanout, int numRequests)
{
  int nextNodeID = 1;
  int rootNodeID = 1;
  string path = ".";
  list<string> pathList;

  int level = rand()%(maxDepth-minDepth)+minDepth;
 
  generateTree(&nextNodeID,path,&pathList,level,minFanout,maxFanout);
  SetupProgramID(pathList,rootNodeID);

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


void UpdateTableRandomTree(int numNodes, int minFanout, int maxFanout, int numRequests)
{
  int nextNodeID = 1;
  int rootNodeID = 1;
  string path = ".";
  list<string> pathList;
 
  generateRandomTree(&nextNodeID,path,&pathList,minFanout,maxFanout,numNodes);
  SetupProgramID(pathList,rootNodeID);
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
  relNames.push_back("equiHashTable");
  relNames.push_back("provHashTable");
  relNames.push_back("provLink");
  relNames.push_back("resultsQry");

  for (int i = 1; i <= totalNumNodes+1; i++)
    {
      int node = i;
      
      app(node) -> SerializeRel(relNames, node, storePath);
      
    }
}


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
  LogComponentEnable("DnsProvDHlistLayerhsOnline", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);
  

  string storePath = "/localdrive1/harshal/prov_test/";
  uint32_t depth = 3;
  uint32_t fanout = 2;
  uint32_t stopTime = 500;
  uint32_t numRequests = 10;
  uint32_t numNodes = -1;
  uint32_t timedRequests = 0;

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
  cmd.AddValue("timedRequests","Insert Requests wrt time",timedRequests);
  cmd.Parse(argc,argv);
  
  //  calculateNumNodes(depth,fanout,&totalNumNodes);
  totalNumNodes = numNodes;
  cout<<"CALCULATE NUM NODES "<<totalNumNodes<<endl;
  

  /*if(stopTime!=-1)
    numRequests = stopTime/INSERTION_TIME;
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
  */
  Ptr<RapidNetApplicationHelper> appHelper = Create<DnsProvDhlistLayerhsOnlineHelper> ();
  //apps = appHelper->Install (csmaNodes);
  

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
    Simulator::Schedule(Seconds(0.01),UpdateTableRandomTree,numNodes,fanout,fanout+1,numRequests);

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

  //apps.Start (Seconds (0.0));
  //apps.Stop (Seconds (SIMULATION_LENGTH));

  //Measure bandwidth
  string config_path = "/NodeList/*/DeviceList/*/$ns3::PointToPointNetDevice/PhyRxEnd";
  //string config_path = "/NodeList/107/DeviceList/*/$ns3::PointToPointNetDevice/PhyTxEnd";
  Config::Connect (config_path, MakeCallback (&MeasureBandwidth));

  // string config_path2 = "/NodeList/*/DeviceList/*/$ns3::PointToPointNetDevice/PhyTxEnd";
  // Config::Connect (config_path2, MakeCallback (&MeasureSize));

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
  apps.Stop (Seconds (500.0));

  Simulator::Schedule (Seconds(499.0000), SerializeProv, storePath);  
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
  /*apps.Start (Seconds (0.0));
  Simulator::Schedule(Seconds(0.01),UpdateTable,depth,depth+1,fanout,fanout+1,numRequests);
  //apps = InitRapidNetApps (800, Create<DnsProvHelper> ());
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (INSERT_TIME + stopTime));
 
  Simulator::Schedule (Seconds(INSERT_TIME + stopTime -1), SerializeProv, storePath);
  schedule (INSERT_TIME+stopTime, Print);
  Simulator::Run ();
  cout<<"Total Number Nodes "<<totalNumNodes<<endl;
  Simulator::Destroy ();
  return 0;*/
}
