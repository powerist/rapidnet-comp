/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

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
#include "ns3/pktfwd-norm-dist-hlist-rmitm-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"
#include "ns3/helper-module.h"

#include "ns3/csma-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-address.h"


/* Links Connecting Hosts To switches*/
/*#define linkhr(host, sw)			\
  tuple(PktfwdNormProvEte::LINKHR,\
  attr("linkhr_attr1", Ipv4Value, host),  \
  attr("linkhr_attr2", Ipv4Value, sw))

#define insert_linkhr(host, sw)                                            \
  app(host) -> Insert(linkhr(addr(host), addr(sw)));
*/
/* Links connecting switches to other devices*/
#define link(sw, nei)\
  tuple(PktfwdNormDistHlistRmitm::LINK,\
  attr("link_attr1", Ipv4Value, sw),  \
  attr("link_attr2", Ipv4Value, nei))

#define insert_link(sw, nei)                                            \
  app(sw) -> Insert(link(addr(sw), addr(nei)));

/* Input packets */
#define initpacket(host, srcadd, dstadd, data)               \
  tuple(PktfwdNormDistHlistRmitm::INITPACKET,\
  attr("initPacket_attr1", Ipv4Value, host),  \
  attr("initPacket_attr2", Ipv4Value, srcadd), \
  attr("initPacket_attr3", Ipv4Value, dstadd),    \
  attr("initPacket_attr4", StrValue, data))

#define insert_packet(host, srcadd, dstadd, data)                            \
  app(host) -> Insert(initpacket(addr(host), addr(srcadd), addr(dstadd), data));


/* flow entry */
#define flowentry(sw, dst, next)		\
  tuple(PktfwdNormDistHlistRmitm::FLOWENTRY,\
	attr("flowEntry_attr1", Ipv4Value, sw),\
	attr("flowEntry_attr2", Ipv4Value, dst),         \
	attr("flowEntry_attr3", Ipv4Value, next))

#define insert_flowentry(sw, dst, next)				\
  app(sw) -> Insert(flowentry(addr(sw), addr(dst), addr(next)))

#define SWITCH 0
#define HOST 1
#define HOSTPERSWC 1 //Warning: This is not changeable under the current routing algorithm
#define DEFAULT_PKTNUM 20

#define PACKET_TIME 4
#define NUM_PACKETS 1000000

#define INSERT_TIME 0.001
using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::pktfwdnormdisthlistrmitm;

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


void Print ()
{
  PrintRelation (apps, PktfwdNormDistHlistRmitm::RECVPACKET);
  PrintRelation (apps, PktfwdNormDistHlistRmitm::RULEEXEC);
  PrintRelation (apps, PktfwdNormDistHlistRmitm::PROV);
}

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


/*void SetupDevices(AdjList* nodeArray, int totalNum)
{
  std::cout << "Set up devices" << endl;
  for (int i = 0; i < totalNum; i++)
    {
      int deviceID = i + 1; // Rapidnet's node ID starts from 1
      if (nodeArray[i].ntype == AdjList::TRANSIT || nodeArray[i].ntype == AdjList::STUB)
        {
          insert_device(deviceID, SWITCH);
        }
      else
        {
          insert_device(deviceID, HOST);
        }
    }
}*/

/* Obtain the topology from GT-ITM output */
/* \return total number of nodes*/
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


void InsertLinkTables(AdjList* nodeArray, int totalNum, int src, int dst, map<int,int> rtables[MAX_NODE_NUM])
{
  std::cout << "Insert link tables" << endl;
   int tempDst = src;
   while(tempDst != dst)
    {
      int tempDstInserted = tempDst+1;
      int linkDstInserted = rtables[tempDst][dst]+1;
      insert_link(tempDstInserted,linkDstInserted);
      tempDst = rtables[tempDst][dst];
    }
   /*
  for (int src = 0; src < totalNum; src++)
    {
      int deviceSrc = src + 1; //Rapidnet's node ID starts from 1
      AdjNode* curNode = nodeArray[src].head;
      while (curNode != NULL)
        {
          int dst = curNode->nodeID;
          int deviceDst = dst + 1; //Rapidnet's node ID starts from 1
          //if (nodeArray[src].ntype == AdjList::ENDPOINT)
          //{
          //insert_linkhr(deviceSrc, deviceDst);
              //}
            //else
            //{
              insert_link(deviceSrc, deviceDst);
              //}

          curNode = curNode->next;
        }
    }
   */
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
      cout<<"INSERTING IN "<<dst<<" "<<hostNode<<" "<<hostNode<<endl;
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
              cout<<"INSERTING IN "<<nodeID<<" "<<hostNode<<" "<<curNode<<endl;
              rtables[nodeID].insert(std::pair<int, int>(hostNode, curNode));
              nei = nei->next;
            }
        }
    }
  //std::cout<<"Finished Routing"<<endl;
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
void SetupFlowTable(map<int, int> rtables[MAX_NODE_NUM], int switchNum, int src, int dst)
{
  std::cout << "Set up flow tables "<<switchNum << endl;
    int tempDst = src;
  while(tempDst != dst)
    {
      int tempSrcInserted = tempDst+1;
      int tempDstInserted = dst+1;
      int nextInserted = rtables[tempDst][dst]+1;
      insert_flowentry(tempSrcInserted,tempDstInserted,nextInserted);
      tempDst = rtables[tempDst][dst];
    }
  /*for(int i=0; i < switchNum; i++)
    {
      map<int,int>::iterator iter = rtables[i].begin();
      for(;iter!=rtables[i].end();iter++)
        {
          cout<<"Rounting entry for "<<i<<" "<<iter->first<<" "<<iter->second<<endl;
        }
    }
  for (int swc = 0; swc < switchNum; swc++)
    {
      int deviceSwc = swc + 1; //Rapidnet's node ID starts from 1
      int tablesize = rtables[swc].size();
      cout<<"TABLE SIZE FOR "<<swc<<" "<<tablesize<<endl;
      map<int, int>::iterator itr;
      int priority = 1;
      for (itr = rtables[swc].begin();itr != rtables[swc].end();itr++)
        {
          cout<<"TABLE VALUE "<<itr->first<<" "<<itr->second<<endl;
          int dst = itr->first;
          int deviceDst = dst + 1;
          int next = itr->second;
          int deviceNext = next + 1;
          insert_flowentry(deviceSwc, deviceDst, deviceNext);
        }
    }
  */
}

/* Insert packets for experiments*/
void PacketInsertion(int src, int dst, string data)
{
  /* Send DEFAULT_PKTNUM of packets to from src to dst*/
  int deviceSrc = src + 1;
  int deviceDst = dst + 1;

  insert_packet(deviceSrc, deviceSrc, deviceDst, data);
  
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


/* Schedule packet transmission*/
void SchedulePacketTrans(int totalNum, int totalSwcNum, int hostPairs, int packetNum, int pathLength, map<int, int> rtables[MAX_NODE_NUM],int dataSize, int *src, int *dst)
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
  cout<<"SchedulePacket Trans started"<<endl;
  double trigger_time = PACKET_TIME;
  srand(1);
  //hostPairs = 1;
  //int srcArray[] = {0,0,55};
  //int dstArray[] = {36,64,53};

  for (int i = 0; i < hostPairs; i++, trigger_time += 0.1)
    {
        int length=0;
      do
	{
	  length=0;
	  *src = (rand() % (totalSwcNum));
	  do
	    {
	      *dst = (rand() % (totalSwcNum));
	    }
	  while (*dst == *src);
	  //src = srcArray[i];
	  //dst = dstArray[i];
	  //Check if destination is atleast pathLength long
	  int tempDst = *src;
	  while(tempDst!=*dst)
	    {
	      tempDst = rtables[tempDst][*dst];
	      length++;
	    }
	}
      while( length != pathLength && pathLength!=-1);
      //src = srcArray[i];
      //dst = dstArray[i];
      std::cout << "Communicating pair: (" << *src << "," << *dst << ")" << endl;
      double insert_time = trigger_time;
      ostringstream ss;
      int dataCount = 0;
      for (unsigned int j = 0;j < packetNum;j++, insert_time += INSERT_TIME, dataCount++)
        {
          ss.str("");
          ss << dataCount;
          string data = generateRandomString(dataSize);
          Simulator::Schedule (Seconds (insert_time), PacketInsertion, *src, *dst, data);
        }
    }
  
  std::cout<<"SchedulePacketTrans Done"<<endl;
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

void SerializeProv(int totalNum, string storePath)
{
  vector<string> relNames;
  relNames.push_back("ruleExec");
  relNames.push_back("prov");

  for (int i = 0; i < totalNum; i++)
    {
      int node = i + 1;
      app(node) -> SerializeRel(relNames, node, storePath);
    }
}

int
main (int argc, char *argv[])
{
  LogComponentEnable("PktfwdNormDistHlistRmitm", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  uint32_t hostPairs = 100;
  string storePath = "/localdrive1/harshal/pktfwd_dist_hlist_rmitm_storage/";
  uint32_t packetNum = 10;

  uint32_t pathLength = 10;
  uint32_t dataSize = 500;
  uint32_t stopTime = 6;
  CommandLine cmd;
  cmd.AddValue("hostPairs", "Number of pairs of communicating hosts", hostPairs);
  cmd.AddValue("storePath", "The path to the directory for provenance storage", storePath);
  cmd.AddValue("packetNum", "Number of packets sent between each pair of hosts", packetNum);
  cmd.AddValue("pathLength", "Number of hops", pathLength);
  cmd.AddValue("dataSize", "Size of payload data", dataSize);
  cmd.AddValue("stopTime", "Stop Time", stopTime);
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
  int totalNum = totalSwcNum;
  //int totalNum = AddHostNodes(nodeArray, totalSwcNum, HOSTPERSWC, swcToHost);
  //  PrintTopology(nodeArray, totalNum);

  // Set up flow entry table
  Routing(nodeArray, totalSwcNum, swcToHost, rtables);
  //  PrintRoutingTabxle(rtables, totalSwcNum);
  packetNum = stopTime/INSERT_TIME;
  int src,dst;
  SchedulePacketTrans(totalNum, totalSwcNum, hostPairs, packetNum, pathLength, rtables, dataSize,&src,&dst);
  // Insert linking information to the database
  Simulator::Schedule (Seconds(0.0001), InsertLinkTables, nodeArray, totalNum, src, dst, rtables);  
  
  // Assign devices
  //Simulator::Schedule (Seconds(0.0010), SetupDevices, nodeArray, totalNum);
  
  // Assign flow entries
  Simulator::Schedule (Seconds(3.0000), SetupFlowTable, rtables, totalSwcNum, src, dst);  

  // Schedule traffic

  /* Create RapidNet apps*/
  //apps = InitRapidNetApps (totalNum, Create<PktfwdNormProvHelper> ());
  /* CSMA device model*/
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

  Ptr<RapidNetApplicationHelper> appHelper = Create<PktfwdNormDistHlistRmitmHelper> ();
  apps = appHelper->Install (csmaNodes);
  std::cout<<"HERE BEFORE STARTING NS3"<<endl;
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (PACKET_TIME+stopTime));

  Simulator::Schedule (Seconds(PACKET_TIME+stopTime-1), SerializeProv, totalNum, storePath);  
  Simulator::Schedule(Seconds(500.0000),Print);
  Simulator::Run ();
  Simulator::Destroy ();

  DeleteLinks(nodeArray, totalNum);
  delete[] nodeArray;

  return 0;
}


// /* Ensure the switch graph is connected*/
// void EnsureReach(AdjList* nodeArray, int switchNum)
// {
//   /* Breadth first search*/
//   int startNode = 0;
//   bool *visited = new bool[switchNum];
//   for (int i = 0; i < switchNum; i++)
//     {
//       visited[i] = false;
//     }

//   list<int> queue;
//   visited[startNode] = true;
//   queue.push_back(startNode);

//   while (!queue.empty())
//     {
//       int curNode = queue.front();
//       queue.pop_front(head);

//       AdjNode* neighbor = nodeArray[curNode].head;
//       while (neighbor != NULL)
//         {
//           int id = neighbor.nodeID;
//           if (!visited[id])
//             {
//               queue.push_back(id);
//               visited[id] = true;
//             }
//           neighbor = neighbor.next;
//         }
//     }

//   /* Make the graph connected if it's not*/
//   for (int i = 0; i < switchNum; i++)
//     {
//       if (!visited[i])
//         {
//           /* Add an edge to the connected component*/
//           cout << "Add an extra link to the connected graph" << endl;
//           for (int j = 0; j < switchNum; j++)
//             {
//               if (visited[j])
//                 {
//                   AddLink(nodeArray, i, j);
//                   visited[i] = true;
//                   break;
//                 }
//             }
//         }
//     }

//   delete[] visited;
// }


// /*\param prob: an interger between 0 - 100. Represents the probability (prob%) that 
// * an edge could exist between switches.
// */
// void CreateEdges(AdjList* nodeArray, int switchNum, int totalSwcNum, int prob)
// {
//   /* Create links between switches and hosts*/
//   /* Switch (i) is connected to host (switchNum + 2i) and host (switchNum + 2i + 1)*/
//   for (int i = 0; i < switchNum; i++)
//     {
//       /* Link from switches to hosts*/
//         insert_link(i, 2*i);
//         insert_link(i, 2*i+1);
//       /* Link from hosts to switches*/
//         insert_linkhr(2*i, i);
//         insert_linkhr(2*i+1, i);
//     }

//   //srand();
//   /* Create links among switches themselves*/
//   for (int i = 0; i < switchNum; i++)
//     {
//       for (int j = i+1; j < switchNum; j++)
//         {
//           bool edge = (rand() % 100) < prob;
//           if (edge == true)
//             {
//               /* Insert Links in tables*/
//               insert_link(i, j);
//               insert_link(j, i);

//               /* Create links between i and j in adjacency table*/
//               AddLink(nodeArray, i, j);
//             }
//         }
//     }
// }
