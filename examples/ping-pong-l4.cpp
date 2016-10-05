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
#include "ns3/pingpong-l4-module.h"
#include "ns3/rapidnet-module.h"
#include "ns3/values-module.h"
#include "ns3/l4-platform-helper.h"
#include "ns3/uinteger.h"
#include <iostream>

#define tlink(src, next) \
  tuple (PingpongL4::TLINK, \
    attr ("tLink_attr1", StrValue, src), \
    attr ("tLink_attr2", StrValue, next))

#define insertlink(from, to) \
  app(from)->Insert (tlink (app(from)->GetLocalLocSpec(), app(to)->GetLocalLocSpec())); \
  app(to)->Insert (tlink (app(to)->GetLocalLocSpec(), app(from)->GetLocalLocSpec()));

#define deletelink(from, to) \
  app(from)->Delete (tlink (app(from)->GetLocalLocSpec(), app(to)->GetLocalLocSpec())); \
  app(to)->Delete (tlink (app(to)->GetLocalLocSpec(), app(from)->GetLocalLocSpec()));

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::pingpongl4;

ApplicationContainer apps;
string destLocSpec = "", localPort = "";

void
UpdateLinks1 ()
{
  app(1)->Insert (tlink (app(1)->GetLocalLocSpec(), destLocSpec));
}

int
main (int argc, char *argv[])
{
  // Use realtime simulator
  GlobalValue::Bind ("SimulatorImplementationType",
                    StringValue ("ns3::RealtimeSimulatorImpl"));

  LogComponentEnable("PingpongL4", LOG_LEVEL_INFO);
  LogComponentEnable("RapidNetApplicationBase", LOG_LEVEL_INFO);

  uint16_t port;

  CommandLine cmd;
  cmd.AddValue ("dest", "Destination LocSpec IP:Port", destLocSpec);
  cmd.AddValue ("localPort", "Local RapidNet port", localPort);
  cmd.Parse (argc, argv);

  if (localPort != "")
  {
    std::istringstream sin (localPort);
    sin >> port;
  } 
  else 
  {
    port = 11111;
  }
 
  // Install L4-Platform
  NodeContainer nodeContainer;
  nodeContainer.Create (1);
  L4PlatformHelper platform;
  platform.Install (nodeContainer);
  PingpongL4Helper helper = PingpongL4Helper();
  helper.SetL4Platform (true);
  helper.SetAttribute ("RapidNetPort", UintegerValue (port));
  // Install ping pong
  apps = helper.Install (nodeContainer);

ofstream* decorator_out = InstallDecorator(apps, ".", 2000, 2000);

  // Set default value
  if (destLocSpec == "")
    destLocSpec = app (1)->GetLocalLocSpec ();
  
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (50.0));

  schedule (0.0001, UpdateLinks1);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
