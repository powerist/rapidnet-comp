#include "stdlib.h"
#include "fstream"
#include <sstream>
#include <list>
#include <map>
#include <cstring>

#include "ns3/uinteger.h"
#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"

#include "ns3/pktfwd-norm-prov-ete-module.h"
#include "ns3/pktfwd-norm-dhlist-layerhs-online-module.h"
#include "ns3/pktfwd-norm-dist-hlist-rmitm-module.h"

#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"
#include "ns3/helper-module.h"

#include "ns3/csma-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-address.h"


/* Links Connecting Hosts To switches*/
#define linkhr_nocomp(host, sw)\
  tuple(PktfwdNormProvEte::LINKHR,\
  attr("linkhr_attr1", Ipv4Value, host),  \
  attr("linkhr_attr2", Ipv4Value, sw))

#define insert_linkhr_nocomp(host, sw)                                            \
  app(host) -> Insert(linkhr_nocomp(addr(host), addr(sw)));

/* Links connecting switches to other devices*/
#define link_nocomp(sw, nei)\
  tuple(PktfwdNormProvEte::LINK,\
  attr("link_attr1", Ipv4Value, sw),  \
  attr("link_attr2", Ipv4Value, nei))

#define insert_link_nocomp(sw, nei)                                            \
  app(sw) -> Insert(link_nocomp(addr(sw), addr(nei)));

//Online Compression

#define link_online(sw, nei)\
  tuple(PktfwdNormDhlistLayerhsOnline::LINK,\
  attr("link_attr1", Ipv4Value, sw),  \
  attr("link_attr2", Ipv4Value, nei))

#define insert_link_online(sw, nei)                                            \
  app(sw) -> Insert(link_online(addr(sw), addr(nei)));

/* Input packets */
#define initpacket_online(host, srcadd, dstadd, data)               \
  tuple(PktfwdNormDhlistLayerhsOnline::INITPACKET,\
  attr("initPacket_attr1", Ipv4Value, host),  \
  attr("initPacket_attr2", Ipv4Value, srcadd), \
  attr("initPacket_attr3", Ipv4Value, dstadd),    \
  attr("initPacket_attr4", StrValue, data))

#define insert_packet_online(host, srcadd, dstadd, data)                            \
  app(host) -> Insert(initpacket_online(addr(host), addr(srcadd), addr(dstadd), data));

/* flow entry */
#define flowentry_online(sw, dst, next)		\
  tuple(PktfwdNormDhlistLayerhsOnline::FLOWENTRY,\
	attr("flowEntry_attr1", Ipv4Value, sw),\
	attr("flowEntry_attr2", Ipv4Value, dst),         \
	attr("flowEntry_attr3", Ipv4Value, next))

#define insert_flowentry_online(sw, dst, next)				\
  app(sw) -> Insert(flowentry_online(addr(sw), addr(dst), addr(next)))

#define programID_online(node, dst, ID)\
  tuple(PktfwdNormDhlistLayerhsOnline::PROGRAMID,\
    attr("programID_attr1",Ipv4Value,node),\
    attr("programID_attr2",Ipv4Value,dst),\
        attr("programID_attr3",StrValue,ID))

#define insert_programID_online(node,dst,ID)\
  app(node)->Insert(programID_online(addr(node),addr(dst),ID))


/* Input packets */
#define initpacket_nocomp(host, srcadd, dstadd, data)               \
  tuple(PktfwdNormProvEte::INITPACKET,\
  attr("initPacket_attr1", Ipv4Value, host),  \
  attr("initPacket_attr2", Ipv4Value, srcadd), \
  attr("initPacket_attr3", Ipv4Value, dstadd),    \
  attr("initPacket_attr4", StrValue, data))

#define insert_packet_nocomp(host, srcadd, dstadd, data)                            \
  app(host) -> Insert(initpacket_nocomp(addr(host), addr(srcadd), addr(dstadd), data));

/* flow entry */
#define flowentry_nocomp(sw, dst, next)		\
  tuple(PktfwdNormProvEte::FLOWENTRY,\
	attr("flowEntry_attr1", Ipv4Value, sw),\
	attr("flowEntry_attr2", Ipv4Value, dst),         \
	attr("flowEntry_attr3", Ipv4Value, next))

#define insert_flowentry_nocomp(sw, dst, next)				\
  app(sw) -> Insert(flowentry_nocomp(addr(sw), addr(dst), addr(next)))



#define insert_device_rmitm(host, dvtype)                                            \
  app(host) -> Insert(device_rmitm(addr(host), dvtype));


/* Links connecting hosts to switches*/
#define linkhr_rmitm(host, sw)\
  tuple(PktfwdNormDistHlistRmitm::LINKHR,\
  attr("linkhr_attr1", Ipv4Value, host),  \
  attr("linkhr_attr2", Ipv4Value, sw))

#define insert_linkhr_rmitm(host, sw)                                            \
  app(host) -> Insert(linkhr_rmitm(addr(host), addr(sw)));

/* Links connecting switches to other devices*/
#define link_rmitm(sw, nei)\
  tuple(PktfwdNormDistHlistRmitm::LINK,\
  attr("link_attr1", Ipv4Value, sw),  \
  attr("link_attr2", Ipv4Value, nei))

#define insert_link_rmitm(sw, nei)				\
  app(sw) -> Insert(link_rmitm(addr(sw), addr(nei)));

/* Input packets */
#define initpacket_rmitm(host, srcadd, dstadd, data)	\
  tuple(PktfwdNormDistHlistRmitm::INITPACKET,\
  attr("initPacket_attr1", Ipv4Value, host),  \
  attr("initPacket_attr2", Ipv4Value, srcadd), \
  attr("initPacket_attr3", Ipv4Value, dstadd),    \
        attr("initPacket_attr4", StrValue, data))

#define insert_packet_rmitm(host, srcadd, dstadd, data)			\
  app(host) -> Insert(initpacket_rmitm(addr(host), addr(srcadd), addr(dstadd), data));

/* flow entry */
#define flowentry_rmitm(sw, dst, next)		\
  tuple(PktfwdNormDistHlistRmitm::FLOWENTRY,\
	attr("flowEntry_attr1", Ipv4Value, sw),\
	attr("flowEntry_attr2", Ipv4Value, dst),         \
	attr("flowEntry_attr3", Ipv4Value, next))

#define insert_flowentry_rmitm(sw, dst, next)				\
  app(sw) -> Insert(flowentry_rmitm(addr(sw), addr(dst), addr(next)))

#define SWITCH 0
#define HOST 1
#define HOSTPERSWC 1 //Warning: This is not changeable under the current routing algorithm
#define DEFAULT_PKTNUM 20

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::pktfwdnormdisthlistrmitm;
using namespace ns3::rapidnet::pktfwdnormdhlistlayerhsonline;
using namespace ns3::rapidnet::pktfwdnormprovete;

ApplicationContainer apps;

const char TOPO_FILE[] = "examples/ts100-0.alt";
const int MAX_NODE_NUM = 512;
const int MAX_LINE_SIZE = 256;
const int MAX_TOKEN_SIZE = 20;
const char* DELIMITER = " ";
const char verticeKey[] = "VERTICES";
const char edgeKey[] = "EDGES";

struct AdjNode
{
  int nodeID;
  AdjNode* next;
};

struct AdjList
{
  enum NodeType{TRANSIT, STUB, ENDPOINT, UNKNOWN};
  NodeType ntype;
  
  AdjNode* head;
};

void AddLink(AdjList* nodeArray, int i, int j)
{
  //  std::cout << "Add a link between (" << i << "," << j << ")" << endl;
  AdjNode* newNode = new AdjNode();
  newNode->nodeID = j;
  newNode->next = nodeArray[i].head;
  nodeArray[i].head = newNode;

  newNode = new AdjNode();
  newNode->nodeID = i;
  newNode->next = nodeArray[j].head;
  nodeArray[j].head = newNode;
}

int ParseTopology(const char* fname, AdjList* nodeArray)
{
  int totalSwcNum = 0;

  ifstream fin;
  fin.open(fname);
  if (!fin.good())
    {
      std::cout << "The file could not be opened!" << endl;
      return 0;
    }

  bool verticeFlag = false;
  bool edgeFlag = false;
  while (!fin.eof())
    {
      char buffer[MAX_LINE_SIZE];
      fin.getline(buffer, MAX_LINE_SIZE);

      char* token[MAX_TOKEN_SIZE];
      token[0] = strtok(buffer, DELIMITER);

      /* Parse the edges*/
      if (edgeFlag == true)
        {
          if (token[0])
            {
              int src = atoi(token[0]);
              int dst;
              token[1] = strtok(NULL, DELIMITER);
              if (token[1])
                {
                  dst = atoi(token[1]);
                }
              else
                {
                  std::cout << "Error parsing edges!" << endl;
                  return 0;
                }
              AddLink(nodeArray, src, dst);
            }
        }
      else
        {
          if (verticeFlag == true)
            {
              /* Handle a blank line*/
              if (!token[0])
                {
                  continue;
                }

              /* Handle the line indicating edge processing*/              
              if (strcmp(token[0], edgeKey) == 0)
                {
                  edgeFlag = true;
                  continue;
                }

              /* Parse the vertices*/
              /* Obtain node ID*/
              int nodeID = atoi(token[0]);
              /* Obtain node type*/
              token[1] = strtok(NULL, DELIMITER);
              if (!token[1])
                {
                  std::cout << "Error parsing!" << endl;
                  return 0;
                }
              if (token[1][0] == 'T')
                {
                  // A transit node
                  nodeArray[nodeID].ntype = AdjList::TRANSIT;
                }
              else
                {
                  if (token[1][0] == 'S')
                    {
                      // A stub node
                      nodeArray[nodeID].ntype = AdjList::STUB;
                    }
                  else
                    {
                      std::cout << "Error parsing node type!" << endl;
                      return 0;
                    }
                }
              totalSwcNum++;
            }
          else
            {
              /* Jump through the file header*/
              std::cout << "File header line" << endl;

              if (!token[0])
                {
                  continue;
                }

              if (strcmp(token[0], verticeKey) == 0)
                {
                  verticeFlag = true;
                }
            }
        }
    }
  return totalSwcNum;
}

/* Create host nodes that are connected to stub nodes*/
/* \return total number of nodes*/
int AddHostNodes(AdjList* nodeArray, int totalSwcNum, int hostPerSwc, int* swcToHost)
{
  int curIdx = totalSwcNum; // Current index of the added host
  /* For each stub node, create hostPerSwc number of hosts connected to it*/
  for (int swcID = 0; swcID < totalSwcNum;swcID++)
    {
      if (nodeArray[swcID].ntype == AdjList::STUB)
        {
          /* Create hosts*/
          for (int j = 0; j < hostPerSwc; j++)
            {
              nodeArray[curIdx].ntype = AdjList::ENDPOINT;
              AddLink(nodeArray, swcID, curIdx);
              swcToHost[swcID] = curIdx;
              curIdx++;
            }
        }
    }

  return curIdx;
}

void PrintTopology(AdjList* nodeArray, int totalNum)
{
  std::cout << endl;

  for (int i = 0; i < totalNum; i++)
    {
      //Print node ID
      std::cout << "node " << i;

      //Print node type
      std::cout << "(";
      switch (nodeArray[i].ntype)
        {
        case AdjList::TRANSIT: std::cout << "Transit"; break;
        case AdjList::STUB: std::cout << "STUB"; break;
        case AdjList::ENDPOINT: std::cout << "ENDPOINT"; break;
        default: std::cout << "UNKNOWN"; break;
        }
      std::cout << ")";

      //Print neighboring info
      AdjNode* curNode = nodeArray[i].head;
      while (curNode != NULL)
        {
          std::cout << curNode->nodeID << ",";
          curNode = curNode->next;
        }
      std::cout << endl;
    }

  std::cout << endl;
}

void InsertLinkTables(AdjList* nodeArray, int totalNum,string mode)
{
  std::cout << "Insert link tables" << endl;
  for (int src = 0; src < totalNum; src++)
    {
      int deviceSrc = src + 1; //Rapidnet's node ID starts from 1
      AdjNode* curNode = nodeArray[src].head;
      while (curNode != NULL)
        {
          int dst = curNode->nodeID;
          int deviceDst = dst + 1; //Rapidnet's node ID starts from 1
	  if(mode == "nocomp")
            {
	      //No compression mode
              insert_link_nocomp(deviceSrc, deviceDst);
            }
	  else if(mode == "online")
	    {
	      //Online mode
	      insert_link_online(deviceSrc,deviceDst);
	    }
	  else{
	    insert_link_rmitm(deviceSrc,deviceDst);
	  }
          curNode = curNode->next;
        }
    }

}


/* Set up flow tables at each router.
 * Use breadth-first search for routing.
 * Shortest path is only computed between stub nodes
*/

/* Note: routing is performed based on the identity of switches,
 * but when the flow tables are set up, the identity of the stub switch should
 * be replaced by its corresponding host address. This effectively restricts
 * the number of hosts at each switch to just one. To allow more hosts per switch,
 * the original NDLog program should be fixed.
 */
void Routing(AdjList* nodeArray, int switchNum, const int* swcToHost, map<int, int> (&rtables)[MAX_NODE_NUM])
{
  std::cout << "Routing..." << endl;
  list<int> queue;
  bool* visited = new bool[MAX_NODE_NUM];

  for (int dst = 0; dst < switchNum; dst++)
    {
      /*if (nodeArray[dst].ntype != AdjList::STUB)
        {
          continue;
          }*/

      for (int i = 0; i < MAX_NODE_NUM; i++)
        {
          visited[i] = false;
        }

      int hostNode = dst;//swcToHost[dst];
      //Install the flow entry towards the local host
      rtables[dst].insert(std::pair<int, int>(hostNode, hostNode));      

      //Breadth-first search
      queue.push_back(dst);
      visited[dst] = true;
      while (!queue.empty())
        {
          int curNode = queue.front();
          queue.pop_front();

          AdjNode* nei = nodeArray[curNode].head;
          while (nei != NULL)
            {
              int nodeID = nei->nodeID;
              
              if (visited[nodeID] || nodeArray[nodeID].ntype == AdjList::ENDPOINT)
                {
                  nei = nei->next;
                  continue;
                }

              visited[nodeID] = true;
              
              //              std::cout << "New node encountered: " << nodeID << endl;
              queue.push_back(nodeID);

              // Change stub switch address to host address when setting up flow entries
              rtables[nodeID].insert(std::pair<int, int>(hostNode, curNode));
              nei = nei->next;
            }
        }
    }
  
  delete[] visited;
}

void PrintRoutingTable(map<int, int> (&rtables)[MAX_NODE_NUM], int switchNum)
{
  std::cout << endl;

  for (int i = 0;i < switchNum;i++)
    {
      std::cout << "Routing Table for node " << i << endl;
      map<int, int>::iterator itrRt;
      for (itrRt = rtables[i].begin();itrRt != rtables[i].end(); itrRt++)
        {
          std::cout << "dst: " << itrRt->first << "; next hop: " << itrRt->second << endl;
          std::cout << endl;
        }
      std::cout << endl;
    }

  std::cout << endl;
}

/* We omit the default flow entry here*/
void SetupFlowTable(map<int, int> rtables[MAX_NODE_NUM], int switchNum, string mode)
{
  std::cout << "Set up flow tables" << endl;
  for (int swc = 0; swc < switchNum; swc++)
    {
      int deviceSwc = swc + 1; //Rapidnet's node ID starts from 1
      int tablesize = rtables[swc].size();

      map<int, int>::iterator itr;
      for (itr = rtables[swc].begin();itr != rtables[swc].end();itr++)
        {
          int dst = itr->first;
          int deviceDst = dst + 1;
          int next = itr->second;
          int deviceNext = next + 1;
	  if(mode == "nocomp")
	    insert_flowentry_nocomp(deviceSwc, deviceDst, deviceNext);	    
	  else if(mode == "online")
	    insert_flowentry_online(deviceSwc, deviceDst, deviceNext);
	  else
	    insert_flowentry_rmitm(deviceSwc, deviceDst, deviceNext);
        }
    }
}

/* Insert packets for experiments*/
void PacketInsertion(int src, int dst, string data,string mode)
{
  /* Send DEFAULT_PKTNUM of packets to from src to dst*/
  int deviceSrc = src + 1;
  int deviceDst = dst + 1;
  if(mode == "nocomp")
    {
      insert_packet_nocomp(deviceSrc, deviceSrc, deviceDst, data);
    }
  else 
    {
      if(mode == "online")
	{
	  insert_packet_online(deviceSrc, deviceSrc, deviceDst, data);
	}
      else
	{
	  insert_packet_rmitm(deviceSrc, deviceSrc, deviceDst, data);
	}
    }
  // ostringstream ss;
  // int dataCount = 0;
  // for (int i = 0; i < DEFAULT_PKTNUM; i++, dataCount++)
  //   {
  //     ss.str("");
  //     ss << dataCount;
  //     string data = ss.str();
  //     insert_packet(deviceSrc, deviceSrc, deviceDst, data);
  //   }
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

int randomStringLength = 21;


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


#define INSERTION_TIME 6.000
/* Schedule packet transmission*/
void SchedulePacketTrans(int totalNum, int totalSwcNum, int hostPairs, int packetNum, int pathLength, map<int, int> rtables[MAX_NODE_NUM],int dataSize, string mode)
{
  /* DEFAULT_PKTNUM of packet transmissions between a single pair of nodes */
  // double insert_time = 4.0000;
  // ostringstream ss;
  // int dataCount = 0;
  // for (int i = 0;i < DEFAULT_PKTNUM;i++, insert_time += 0.0010, dataCount++)
  //   {
  //     ss.str("");
  //     ss << dataCount;
  //     string data = ss.str();
  //     Simulator::Schedule (Seconds (insert_time), PacketInsertion, 131, 190, data);
  //   }

  /* Setup: each host randomly picks another host and 
     send a series of packets to it*/
  double trigger_time = INSERTION_TIME;
  int totalHops = 0;
  int dataCount = 0;
  srand(1);
   vector<int> srcVector,dstVector;
  vector<int> positions;
  for(int i=0; i< hostPairs; i++)
    positions.push_back(i);
  //hostPairs =1;
  //int srcArray[] = {0,0,55};
  //int dstArray[] = {36,64,53};
  for (int i = 0; i < hostPairs; i++)
    {
       int src,dst,length=0;
      do
	{
	  length=0;
	  src = (rand() % (totalSwcNum));
	  do
	    {
	      dst = (rand() % (totalSwcNum));
	    }
	  while (dst == src);
	  //src = srcArray[i];
	  //dst = dstArray[i];
	  //Check if destination is atleast pathLength long
	  int tempDst = src;
	  while(tempDst!=dst)
	    {
	      tempDst = rtables[tempDst][dst];
	      length++;
	    }
	}
      while( length != pathLength && pathLength!=-1);
      //src = srcArray[i];
      //dst = dstArray[i];
      std::cout << "Communicating pair: (" << src << "," << dst << ")" << endl;
      srcVector.push_back(src);
      dstVector.push_back(dst);
    }
  //srand(unsigned( time(0)));
  //random_shuffle(positions.begin(),positions.end());
  //srand(1);
  for(vector<int>::iterator iter=positions.begin();iter != positions.end() ; iter++, trigger_time+=0.1)
    {
      int src = srcVector[*iter];
      int dst = dstVector[*iter];
      
      double insert_time = trigger_time;
      ostringstream ss;
      int dataCount = 0;
      for (int j = 0;j < packetNum;j++, insert_time += 0.0010, dataCount++)
        {
          ss.str("");
          ss << dataCount;
          string data = generateRandomString(dataSize);//ss.str();
          Simulator::Schedule (Seconds (insert_time), PacketInsertion, src, dst, data,mode);
        }
    }

  
}


void DeleteLinks(AdjList* nodeArray, int totalSwcNum)
{
  for (int i = 0; i < totalSwcNum; i++)
    {
      AdjNode* curNode = nodeArray[i].head;
      AdjNode* nextNode = curNode->next;
      while (curNode != NULL)
        {
          nextNode = curNode->next;
          delete curNode;
          curNode = nextNode;
        }
    }
}

map<string,bool> generatedID;

void SetupProgramID(int switchNum)
{
  srand(0);
  for(int swc =0 ; swc < switchNum; swc++)
    {
      for(int destSwc = 0; destSwc < switchNum ; destSwc++)
	{
	  if(swc!=destSwc)
	    {
              
              string progID = generateRandomString(randomStringLength);
              while(generatedID.count(progID) != 0)
                {
                  progID = generateRandomString(randomStringLength);
                }
              generatedID[progID] = true;
	      insert_programID_online(swc+1,destSwc+1,progID.c_str());
	    }
	}
    }
}

void SerializeProv(int totalNum, string storePath, string mode)
{
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
  for (int i = 0; i < totalNum; i++)
    {
      int node = i + 1;
      app(node) -> SerializeRel(relNames, node, storePath);
    }
}

int
main (int argc, char *argv[])
{
  LogComponentEnable("PktfwdLargeScaleDistHlistOnline", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  uint32_t hostPairs = 1;
  string storePath = "/localdrive1/harshal/pktfwd_dhlist_layerhs_online_storage/";
  uint32_t packetNum = 1;
  uint32_t pathLength = -1;
  uint32_t dataSize = 5;
  uint32_t stopTime = 500;
  string mode = "nocomp";
  CommandLine cmd;
  cmd.AddValue("hostPairs", "Number of pairs of communicating hosts", hostPairs);
  cmd.AddValue("storePath", "The path to the directory for provenance storage", storePath);
  cmd.AddValue("packetNum", "Number of packets sent between each pair of hosts", packetNum);  
  cmd.AddValue("dataSize", "Payload Data Size", dataSize);
  cmd.AddValue("pathLength", "Number of hops", pathLength);
  cmd.AddValue("type", "Type of Experiment[nocomp,online,rmitm]", mode);
  cmd.AddValue("stopTime", "Stop Time for the Experiment", stopTime);
  cmd.AddValue("mode", "Mode of the Experiment", mode);
  cmd.Parse(argc, argv);

  AdjList* nodeArray = new AdjList[MAX_NODE_NUM];
  int swcToHost[MAX_NODE_NUM];
  for (int i = 0; i < MAX_NODE_NUM; i++)
    {
      nodeArray[i].ntype = AdjList::UNKNOWN;
      nodeArray[i].head = NULL;

      swcToHost[i] = -1;
    }
  map<int, int> rtables[MAX_NODE_NUM];

  int totalSwcNum = ParseTopology(TOPO_FILE, nodeArray);
  int totalNum = totalSwcNum;//AddHostNodes(nodeArray, totalSwcNum, HOSTPERSWC, swcToHost);
  //  PrintTopology(nodeArray, totalNum);

  // Set up flow entry table
  Routing(nodeArray, totalSwcNum, swcToHost, rtables);
  //  PrintRoutingTable(rtables, totalSwcNum);

  // Insert linking information to the database
   Simulator::Schedule (Seconds(0.0001), InsertLinkTables, nodeArray, totalNum,mode);  
  
  // // Assign devices
  // //Simulator::Schedule (Seconds(0.0010), SetupDevices, nodeArray, totalNum);
  
  // // Assign flow entries
   Simulator::Schedule (Seconds(3.0000), SetupFlowTable, rtables, totalSwcNum,mode);
   if(mode == "online")
     Simulator::Schedule (Seconds(5.0000), SetupProgramID, totalSwcNum);  

  // // Schedule traffic
  // //SchedulePacketTrans(totalNum, totalSwcNum, hostPairs, packetNum,dataSize);
     SchedulePacketTrans(totalNum, totalSwcNum, hostPairs, packetNum, pathLength, rtables,dataSize,mode);
  // /* Create RapidNet apps*/
  // //apps = InitRapidNetApps (totalNum, Create<PktfwdNormProvCompOnlineHelper> ());
  // /* CSMA device model*/
   NodeContainer csmaNodes;
   csmaNodes.Create (totalNum);

   CsmaHelper csma;
   csma.SetDeviceAttribute ("EncapsulationMode", StringValue ("Dix"));
   csma.SetDeviceAttribute ("FrameSize", UintegerValue (64000));

   NetDeviceContainer csmaDevices;
   csmaDevices = csma.Install (csmaNodes);

   InternetStackHelper stack;
   stack.Install (csmaNodes);

   Ipv4AddressHelper address;

   address.SetBase ("10.1.1.0", "255.255.255.0");
   address.Assign (csmaDevices);

  // //Ptr<RapidNetApplicationHelper> appHelper = Create<PktfwdNormDhlistLayerhsOnlineHelper> ();
  
  Ptr<RapidNetApplicationHelper> appHelper;
  if(mode == "online")
    appHelper= Create<PktfwdNormDhlistLayerhsOnlineHelper> ();
  else if(mode == "rmitm")
    appHelper = Create<PktfwdNormDistHlistRmitmHelper>();
  else
    appHelper = Create<PktfwdNormProvEteHelper>();
  apps = appHelper->Install (csmaNodes);
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (INSERTION_TIME+stopTime));

  Simulator::Schedule (Seconds(INSERTION_TIME+stopTime-1), SerializeProv, totalNum, storePath,mode);  

  Simulator::Run ();
  Simulator::Destroy ();
  
  DeleteLinks(nodeArray, totalNum);
  delete[] nodeArray;
  
  return 0;
}
