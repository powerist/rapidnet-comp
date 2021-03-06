/* A RapidNet application. Generated by RapidNet compiler. */

#ifndef PKTFWDSDNPROVCOMP_H
#define PKTFWDSDNPROVCOMP_H

#include <string>
#include <iostream>
#include "ns3/ptr.h"
#include "ns3/event-id.h"
#include "ns3/ipv4-address.h"
#include "ns3/rapidnet-header.h"
#include "ns3/relation-base.h"
#include "ns3/database.h"
#include "ns3/rapidnet-application-base.h"
#include "ns3/aggregator.h"
#include "ns3/aggwrap.h"

using namespace std;
using namespace ns3;

namespace ns3 {

class Socket;
class Packet;

namespace rapidnet {
namespace pktfwdsdnprovcomp {

class PktfwdSdnProvComp : public RapidNetApplicationBase
{
public:
  static const string DEVICE;
  static const string EMATCHINGPACKET;
  static const string EMATCHINGPACKETCOUNT;
  static const string EMATCHINGPACKETTEMP;
  static const string EPACKET;
  static const string EPACKETCOUNT;
  static const string EPACKETTEMP;
  static const string ERECVPACKET;
  static const string ERECVPACKETCOUNT;
  static const string ERECVPACKETTEMP;
  static const string FLOWENTRY;
  static const string HASHTABLE;
  static const string INITPACKET;
  static const string LINK;
  static const string LINKHR;
  static const string MATCHINGPACKET;
  static const string MAXPRIORITY;
  static const string PACKET;
  static const string RECVAUXPKT;
  static const string RECVPACKET;
  static const string RULEEXEC;

  static TypeId GetTypeId (void);

  PktfwdSdnProvComp ();

  virtual ~PktfwdSdnProvComp ();

protected:

  virtual void DoDispose (void);

  virtual void StartApplication (void);

  virtual void StopApplication (void);

  virtual void InitDatabase (void);

  virtual void DemuxRecv (Ptr<Tuple> tuple);

  virtual void Rs10_eca (Ptr<Tuple> packet);

  virtual void Rs11_eca (Ptr<Tuple> eMatchingPacketTemp);

  virtual void Rs12_eca (Ptr<Tuple> eMatchingPacketTemp);

  virtual void Rs13_eca (Ptr<Tuple> eMatchingPacketCount);

  virtual void Rs15_eca (Ptr<Tuple> eMatchingPacket);

  virtual void Rs20_eca (Ptr<Tuple> matchingPacket);

  virtual void Rs30_eca (Ptr<Tuple> matchingPacket);

  virtual void Rs31_eca (Ptr<Tuple> ePacketTemp);

  virtual void Rs32_eca (Ptr<Tuple> ePacketTemp);

  virtual void Rs33_eca (Ptr<Tuple> ePacketCount);

  virtual void Rs34_eca (Ptr<Tuple> ePacket);

  virtual void Rs40_eca (Ptr<Tuple> matchingPacket);

  virtual void Rh10Eca0Ins (Ptr<Tuple> device);

  virtual void Rh10Eca1Ins (Ptr<Tuple> initPacket);

  virtual void Rh10Eca2Ins (Ptr<Tuple> linkhr);

  virtual void Rh20_eca (Ptr<Tuple> packet);

  virtual void Rh21_eca (Ptr<Tuple> eRecvPacketTemp);

  virtual void Rh22_eca (Ptr<Tuple> eRecvPacketTemp);

  virtual void Rs23_eca (Ptr<Tuple> eRecvPacketCount);

  virtual void Rh24_eca (Ptr<Tuple> eRecvPacket);

  virtual void Rh25Eca0Ins (Ptr<Tuple> recvPacket);

  virtual void Rh25Eca0Del (Ptr<Tuple> recvPacket);

};

} // namespace pktfwdsdnprovcomp
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDSDNPROVCOMP_H
