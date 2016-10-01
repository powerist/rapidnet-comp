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

#include "ns3/core-module.h"
#include "ns3/simulator-module.h"
#include "ns3/node-module.h"
#include "ns3/pktfwd-norm-no-device-module.h"
#include "ns3/pktfwd-norm-nodev-provquery-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"
#include "ns3/helper-module.h"

/* Links connecting network devices*/
#define link(sw, nei)\
  tuple(PktfwdNormNoDevice::LINK,\
  attr("link_attr1", Ipv4Value, sw),  \
  attr("link_attr2", Ipv4Value, nei))

#define insert_link(sw, nei)                                            \
  app(sw) -> Insert(link(addr(sw), addr(nei)));

/* Input packets */
#define initpacket(host, srcadd, dstadd, data)         \
  tuple(PktfwdNormNoDevice::INITPACKET,\
  attr("initPacket_attr1", Ipv4Value, host),  \
  attr("initPacket_attr2", Ipv4Value, srcadd), \
  attr("initPacket_attr3", Ipv4Value, dstadd), \
  attr("initPacket_attr4", StrValue, data))

#define insert_packet(host, srcadd, dstadd, data)                            \
  app(host) -> Insert(initpacket(addr(host), addr(srcadd), addr(dstadd), data));

/* flow entry */
#define flowentry(sw, dst, next)		\
  tuple(PktfwdNormNoDevice::FLOWENTRY,\
	attr("flowEntry_attr1", Ipv4Value, sw),\
	attr("flowEntry_attr2", Ipv4Value, dst),         \
	attr("flowEntry_attr3", Ipv4Value, next))

#define insert_flowentry(sw, dst, next)				\
  app(sw) -> Insert(flowentry(addr(sw), addr(dst), addr(next)));

/* Tuple to query*/
#define queryTuple(ret, name, loc, src, dst, data) \
  tuple(PktfwdNormNodevProvquery::TUPLE,\
  attr("tuple_attr1", Ipv4Value, ret),  \
  attr("tuple_attr2", StrValue, name),  \
  attr("tuple_attr3", Ipv4Value, loc), \
  attr("tuple_attr4", Ipv4Value, src), \
  attr("tuple_attr5", Ipv4Value, dst),\
  attr("tuple_attr6", StrValue, data))


#define insert_queryTuple(name, loc, src, dst, data) \
  qnode->Insert(queryTuple(qnode->GetAddress(), name, addr(loc), addr(src), addr(dst), data));

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::pktfwdnormnodevice;
using namespace ns3::rapidnet::pktfwdnormnodevprovquery;

ApplicationContainer apps;
ApplicationContainer queryApps;

void Print ()
{
  PrintRelation (apps, PktfwdNormNoDevice::RECVPACKET);
  PrintRelation (apps, PktfwdNormNoDevice::PROV);
  PrintRelation (apps, PktfwdNormNoDevice::RULEEXEC);

  PrintRelation (queryApps, PktfwdNormNodevProvquery::RECORDS);
}

/*
 * Topology: 
 * Nodes: 1 -- 2 -- 3
 */
void BuildTopology()
{
  
  /* Connect switches to hosts and each other*/
  insert_link(1, 2);
  insert_link(2, 1);
  insert_link(2, 3);    
  insert_link(3, 2);
}

void SetupFlowTable()
{
  /* Set up the flow entry for switch 5*/
  insert_flowentry(1, 3, 2);
  insert_flowentry(2, 3, 3);
}

void PacketInsertion()
{
  insert_packet(1, 1, 3, "data");
}

void QueryInsertion()
{
  Ptr<RapidNetApplicationBase> qnode = queryApps.Get(0)->GetObject<RapidNetApplicationBase>();
  insert_queryTuple("recvPacket", 3, 1, 3, "data");
}

void InitApps(int nodeNum)
{
  NodeContainer mainNodes;
  NodeContainer queryNode;

  mainNodes.Create(nodeNum);
  queryNode.Create(1);

  NodeContainer csmaNodes;
  csmaNodes.Add(mainNodes);
  csmaNodes.Add(queryNode);

  CsmaHelper csma;

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install(csmaNodes);

  InternetStackHelper stack;
  stack.Install(csmaNodes);

  Ipv4AddressHelper address;
  Ipv4Address base = "10.1.1.0";

  address.SetBase(base, "255.255.255.0");
  address.Assign(csmaDevices);

  apps.Add(Create<PktfwdNormNoDeviceHelper>()->Install(mainNodes));
  queryApps.Add(Create<PktfwdNormNodevProvqueryHelper>()->Install(queryNode));

  SetMaxJitter (apps, 0.001);
  SetMaxJitter (queryApps, 0.001);
}

int
main (int argc, char *argv[])
{
  LogComponentEnable("PktfwdNormNoDevice", LOG_LEVEL_INFO);
  LogComponentEnable("PktfwdNormNodevProvquery", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  int nodeNum = 3;
  InitApps(nodeNum);

  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (500.0));
  queryApps.Start (Seconds(0.0));
  queryApps.Stop (Seconds (500.0));

  schedule (0.0001, BuildTopology);
  schedule (3.0000, SetupFlowTable);
  schedule (4.0000, PacketInsertion);
  schedule (400.0000, QueryInsertion);
  schedule (499.0000, Print);

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}

