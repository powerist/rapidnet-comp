/* A RapidNet application. Generated by RapidNet compiler. */

#ifndef PKTFWDSDNPROV_H
#define PKTFWDSDNPROV_H

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
namespace pktfwdsdnprov {

class PktfwdSdnProv : public RapidNetApplicationBase
{
public:
  static const string DEVICE;
  static const string EMATCHINGPACKET;
  static const string EMATCHINGPACKETTEMP;
  static const string EPACKET;
  static const string EPACKETTEMP;
  static const string ERECVPACKET;
  static const string ERECVPACKETTEMP;
  static const string FLOWENTRY;
  static const string INITPACKET;
  static const string LINK;
  static const string LINKHR;
  static const string MATCHINGPACKET;
  static const string MAXPRIORITY;
  static const string PACKET;
  static const string PROV;
  static const string RECVPACKET;
  static const string RULEEXEC;

  static TypeId GetTypeId (void);

  PktfwdSdnProv ();

  virtual ~PktfwdSdnProv ();

protected:

  virtual void DoDispose (void);

  virtual void StartApplication (void);

  virtual void StopApplication (void);

  virtual void InitDatabase (void);

  virtual void DemuxRecv (Ptr<Tuple> tuple);

  virtual void R00Eca1Ins (Ptr<Tuple> initPacket);

  virtual void R01Eca1Ins (Ptr<Tuple> device);

  virtual void R02Eca1Ins (Ptr<Tuple> maxPriority);

  virtual void R03Eca1Ins (Ptr<Tuple> flowEntry);

  virtual void Rs10_eca (Ptr<Tuple> packet);

  virtual void Rs11_eca (Ptr<Tuple> eMatchingPacketTemp);

  virtual void Rs12_eca (Ptr<Tuple> eMatchingPacketTemp);

  virtual void Rs13_eca (Ptr<Tuple> eMatchingPacket);

  virtual void Rs14_eca (Ptr<Tuple> eMatchingPacket);

  virtual void Rs20_eca (Ptr<Tuple> matchingPacket);

  virtual void Rs30_eca (Ptr<Tuple> matchingPacket);

  virtual void Rs31_eca (Ptr<Tuple> ePacketTemp);

  virtual void Fs32_eca (Ptr<Tuple> ePacketTemp);

  virtual void Rs33_eca (Ptr<Tuple> ePacket);

  virtual void Rs34_eca (Ptr<Tuple> ePacket);

  virtual void Rs40_eca (Ptr<Tuple> matchingPacket);

  virtual void Rh10Eca0Ins (Ptr<Tuple> device);

  virtual void Rh10Eca1Ins (Ptr<Tuple> initPacket);

  virtual void Rh10Eca2Ins (Ptr<Tuple> linkhr);

  virtual void Rh20_eca (Ptr<Tuple> packet);

  virtual void Rh21_eca (Ptr<Tuple> eRecvPacketTemp);

  virtual void Rh22_eca (Ptr<Tuple> eRecvPacketTemp);

  virtual void Rh23_eca (Ptr<Tuple> eRecvPacket);

  virtual void Rh24_eca (Ptr<Tuple> eRecvPacket);

};

} // namespace pktfwdsdnprov
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDSDNPROV_H
