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
#include "ns3/gnuplot.h"

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

#define NET_MASK "255.255.255.0"
#define BASE_ADDR "10."
#define ADDR_SUFFIX ".0"
#define SIMULATION_LENGTH 500
#define SAMPLE_INTERVAL 1
#define BANDWIDTH_LABEL "Avg Bandwidth"
#define MBPS_UNIT 1000000
#define KBPS_UNIT 1000
#define GRAPHIC_PREFIX "/localdrive1/harshal/bandwidth/pktfwd/"
#define PACKET_INIT_TIME 6.0000

const string plotTitle = "bandwidth usage";
const string dataTitle = "bandwidth_data";

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::pktfwdnormdisthlistrmitm;
using namespace ns3::rapidnet::pktfwdnormdhlistlayerhsonline;
using namespace ns3::rapidnet::pktfwdnormprovete;

ApplicationContainer apps;

int m_bytesTotal = 0;
int m_bytesExpr = 0;
bool counter_trigger = false;
int counter = 0;
#define COUNTER_MAX 10

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


void ShowAppAddr(ApplicationContainer& apps, int totalNum)
{
  std::ofstream ofile("addr_mapping.txt");
  ofile << "\n" << "Mapping: (Node ID -> Application address) " << "\n";
  for (int i = 0;i < totalNum;i++)
    {
      int nodeID = i;
      ofile << "Node ID: " << nodeID;
      ofile << " -> ";
      ofile << "App Ipv4ddress: " << app(nodeID+1)->GetAddress ();
      ofile << "\n";
    }
  ofile << "\n";
}


/* Get the routing path between the src and the dst*/
vector<int> GetPath(int src, int dst, map<int, int> rtables[MAX_NODE_NUM])
{
  vector<int> path;

  //Assert src should be in the range of hosts
  
  //path.push_back(src);
  //Get the direct switch of src
  int swc = src;//hostToSwc[src];
  //Push intermediate nodes
  int next = swc;
  while (next != dst)
    {
      path.push_back(next);
      next = rtables[next][dst];
    }

  path.push_back(dst);
  return path;
}

/* Print the routing path to a file*/
void PrintPathToFile(vector<int>& path)
{
  std::cout << "Path length: " << path.size() << std::endl;
  std::ofstream outFile("/localdrive1/harshal/bandwidth/pathFile_nocomp.pt");
  vector<int>::iterator itr;
  for (itr = path.begin(); itr != path.end(); itr++)
    {
      outFile << *itr << "\n";
    }
  outFile.close();
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
  else if(mode == "online")
    {
      insert_packet_online(deviceSrc, deviceSrc, deviceDst, data);
    }
  else
    {
      insert_packet_rmitm(deviceSrc, deviceSrc, deviceDst, data);
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


/* Schedule packet transmission*/
void SchedulePacketTrans(int totalNum, int totalSwcNum, int hostPairs, int packetNum, int pathLength, map<int, int> rtables[MAX_NODE_NUM],int dataSize, string mode)
{
  /* DEFAULT_PKTNUM of packet transmissions between a single pair of nodes */
  
  /* Setup: each host randomly picks another host and 
     send a series of packets to it*/
  double trigger_time = PACKET_INIT_TIME;
  int totalHops = 0;
  int dataCount = 0;
  srand(1);
  //Store the src and destination pairs in vectors so that they can be permutated later
  vector<int> srcVector,dstVector;
  //Positions store the random permutations of the indices of the src and dst vectors
  vector<int> positions;
  for(int i=0; i< hostPairs; i++)
    positions.push_back(i);

  //Randomly pick two node pairs and check if they are atleast pathLenght apart
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
    
	  //Check if destination is atleast pathLength long
	  int tempDst = src;
	  while(tempDst!=dst)
	    {
	      tempDst = rtables[tempDst][dst];
	      length++;
	    }
	}
      while( length != pathLength && pathLength!=-1);
     
      std::cout << "Communicating pair: (" << src << "," << dst << ")" << endl;
      srcVector.push_back(src);
      dstVector.push_back(dst);
    }
  srand(unsigned( time(0)));
  random_shuffle(positions.begin(),positions.end());
  //Reset the seed for the random function generator
  srand(1);
  
  for(vector<int>::iterator iter=positions.begin();iter != positions.end() ; iter++, trigger_time+=0.1)
    {
      int src = srcVector[*iter];
      int dst = dstVector[*iter];
      vector<int> path = GetPath(src, dst, rtables);
      totalHops += path.size ();
      PrintPathToFile(path);
      double insert_time = trigger_time;
      ostringstream ss;
      int dataCount = 0;
      for (int j = 0;j < packetNum;j++, insert_time += 0.0010, dataCount++)
        {
          ss.str("");
          ss << dataCount;
	  //Insert data packet into the network with the dataSize given
          string data = generateRandomString(dataSize);
          Simulator::Schedule (Seconds (insert_time), PacketInsertion, src, dst, data,mode);
        }
    }

  
   //Extra functionality: calculate the total number of hops and output to file
  std::ofstream hopCountFile;
  hopCountFile.open (GRAPHIC_PREFIX"hopCount_online.dat", ios::out | ios::app);
  hopCountFile << hostPairs << "\t" << totalHops << "\n";
  hopCountFile.close();
}

void MeasureBandwidth(std::string context, Ptr< const Packet > packet)
{
  std::cout << "New packet counted. Size: " << packet->GetSize () << std::endl; 
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
  Simulator::Schedule (Seconds (SAMPLE_INTERVAL), Throughput, dataset, totalNum, bandw_divisor);
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


void SetupProgramID(int switchNum,string mode)
{
  if(mode != "online")
    return;
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

int
main (int argc, char *argv[])
{
  LogComponentEnable("PktfwdLargeScaleDistHlistOnline", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  uint32_t hostPairs = 1;
  string storePath = "/localdrive1/harshal/bandwidth/pktfwd_online_storage/";
  uint32_t packetNum = 1;

    
  string graphicName = "bandwidth.pdf";
  string plotFileName = "bandwidth.pl";
  string bandwidthUnit = "Bps";

  string mode = "nocomp";
  uint32_t pathLength = -1;
  uint32_t dataSize = 2;

  uint32_t stopTime = 500;

  CommandLine cmd;
  cmd.AddValue("hostPairs", "Number of pairs of communicating hosts", hostPairs);
  cmd.AddValue("storePath", "The path to the directory for provenance storage", storePath);
  cmd.AddValue("packetNum", "Number of packets sent between each pair of hosts", packetNum);
  cmd.AddValue("graphicName", "File name of the bandwidth plot", graphicName);
  cmd.AddValue("plotFileName", "File name of the plot script", plotFileName);
  cmd.AddValue("bandwidthUnit", "The unit for displaying bandwidth", bandwidthUnit);
  cmd.AddValue("dataSize","The size of packet payload in characters",dataSize);
  cmd.AddValue("pathLength", "Number of hops", pathLength);
  cmd.AddValue("type", "Type of Experiment[nocomp,online,rmitm]", mode);
  cmd.AddValue("stopTime", "Stop Time for the Experiment", stopTime);  

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
  
  // Assign devices
  //Simulator::Schedule (Seconds(0.0010), SetupDevices, nodeArray, totalNum);
  
  // Assign flow entries
  Simulator::Schedule (Seconds(3.0000), SetupFlowTable, rtables, totalSwcNum,mode);
  if(mode == "online")
    Simulator::Schedule (Seconds(5.0000), SetupProgramID, totalSwcNum,mode);  

  // Schedule traffic
  SchedulePacketTrans(totalNum, totalSwcNum, hostPairs, packetNum, pathLength, rtables,dataSize,mode);

  /* Create RapidNet apps*/
  
  Ptr<RapidNetApplicationHelper> appHelper;
  if(mode == "online")
    appHelper= Create<PktfwdNormDhlistLayerhsOnlineHelper> ();
  else if(mode == "rmitm")
    appHelper = Create<PktfwdNormDistHlistRmitmHelper>();
  else
    appHelper = Create<PktfwdNormProvEteHelper>();
  //apps = appHelper->Install (csmaNodes);

  NodeContainer ptpNodes;
  ptpNodes.Create (totalNum);

  PointToPointHelper ptpHelper;
  std::vector<NetDeviceContainer> devices;
  //Create point-to-point links for neighbors
  for (int nid = 0;nid < totalNum;nid++)
  {
    AdjNode* curNode = nodeArray[nid].head;
    while (curNode != NULL)
    {
      if (curNode->nodeID < nid)
        {
          curNode = curNode->next;
          continue;
        }
      
      NodeContainer src = ptpNodes.Get(nid);
      NodeContainer dst = ptpNodes.Get(curNode->nodeID);
      NodeContainer pair = NodeContainer(src, dst);
      NetDeviceContainer ptpDevice;
      //Set up point-to-point net devices and its properties
      if ((nodeArray[nid].ntype == AdjList::TRANSIT 
           && nodeArray[curNode->nodeID].ntype == AdjList::TRANSIT))
        {
          ptpHelper.SetDeviceAttribute ("DataRate", StringValue ("1Gbps"));
          ptpHelper.SetChannelAttribute ("Delay", StringValue ("500ms"));
          ptpDevice = ptpHelper.Install (pair);
        }
      
      if ((nodeArray[nid].ntype == AdjList::TRANSIT 
           && nodeArray[curNode->nodeID].ntype == AdjList::STUB) | 
          (nodeArray[nid].ntype == AdjList::STUB 
           && nodeArray[curNode->nodeID].ntype == AdjList::TRANSIT))
        {
          ptpHelper.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
          ptpHelper.SetChannelAttribute ("Delay", StringValue ("10ms"));
          ptpDevice = ptpHelper.Install (pair);
        }

      if ((nodeArray[nid].ntype == AdjList::STUB
           && nodeArray[curNode->nodeID].ntype == AdjList::STUB) | 
          (nodeArray[nid].ntype == AdjList::STUB 
           && nodeArray[curNode->nodeID].ntype == AdjList::STUB))
        {
          ptpHelper.SetDeviceAttribute ("DataRate", StringValue ("50Mbps"));
          ptpHelper.SetChannelAttribute ("Delay", StringValue ("2ms"));
          ptpDevice = ptpHelper.Install (pair);
        }


      if ((nodeArray[nid].ntype == AdjList::STUB 
           && nodeArray[curNode->nodeID].ntype == AdjList::ENDPOINT) | 
          (nodeArray[nid].ntype == AdjList::ENDPOINT 
           && nodeArray[curNode->nodeID].ntype == AdjList::STUB))
        {
          ptpHelper.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
          ptpHelper.SetChannelAttribute ("Delay", StringValue ("1ms"));
          ptpDevice = ptpHelper.Install (pair);
        }
      
      devices.push_back(ptpDevice);
      
      curNode = curNode->next;
    }
  }
  
  InternetStackHelper stack;
  stack.Install (ptpNodes);

  Ipv4AddressHelper address;
  std::vector<NetDeviceContainer>::iterator itr;
  int lowNetworkID = 0;
  int highNetworkID = 0;
  int maxSegment = 256;
  std::cout << devices.size ();
  for (itr = devices.begin(); itr != devices.end(); itr++)
  {
    std::ostringstream baseAddr;
    baseAddr << BASE_ADDR << highNetworkID << "." << lowNetworkID << ADDR_SUFFIX;
    address.SetBase (baseAddr.str().c_str(), NET_MASK);
    address.Assign (*itr);    
    lowNetworkID++;
    if (lowNetworkID == maxSegment)
      {
        lowNetworkID = 0;
        highNetworkID++;
      }

    if (highNetworkID == maxSegment)
      {
        std::cerr << "Too many links!" << std::endl;
        return 0;
      }
  }

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  apps = appHelper->Install (ptpNodes);
  //Set max jitter of apps as 0
  SetMaxJitter (apps, 0);
  
  ShowAppAddr(apps, totalNum);

  //apps.Start (Seconds (0.0));
  //apps.Stop (Seconds (SIMULATION_LENGTH));

  //Measure provenance storage
  Simulator::Schedule (Seconds(SIMULATION_LENGTH), SerializeProv, totalNum, storePath,mode);  

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
  Simulator::Schedule (Seconds (SAMPLE_INTERVAL), Throughput, dataset, totalNum, bandw_divisor);

  
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (PACKET_INIT_TIME+stopTime));

  //Simulator::Schedule (Seconds(499.0000), SerializeProv, totalNum, storePath);  

  Simulator::Run ();
  Simulator::Destroy ();

  
   //Plot the graph
  plot.AddDataset (dataset);
  string absPathPlotFile = GRAPHIC_PREFIX + plotFileName;
  std:ofstream plotFile (absPathPlotFile.c_str());
  plot.GenerateOutput (plotFile);
  plotFile.close ();
  
  std::cout << "Total bandwidth: " << m_bytesExpr << "Bytes" << std::endl;
  
  DeleteLinks(nodeArray, totalNum);
  delete[] nodeArray;

  return 0;
}
