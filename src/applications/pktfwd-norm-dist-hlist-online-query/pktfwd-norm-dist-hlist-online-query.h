/* A RapidNet application. Generated by RapidNet compiler. */

#ifndef PKTFWDNORMDISTHLISTONLINEQUERY_H
#define PKTFWDNORMDISTHLISTONLINEQUERY_H

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
namespace pktfwdnormdisthlistonlinequery {

class PktfwdNormDistHlistOnlineQuery : public RapidNetApplicationBase
{
public:
  static const string BASEQUERY;
  static const string BASERETURN;
  static const string ERRETURN;
  static const string EPACKETCOUNT;
  static const string EPACKETTEMP;
  static const string EQUIHASHTABLE;
  static const string ERECVPACKETTEMP;
  static const string FLOWENTRY;
  static const string INITPACKET;
  static const string INITPACKETCOUNT;
  static const string INITPACKETHASH;
  static const string LINK;
  static const string PQLIST;
  static const string PRESULTTMP;
  static const string PRETURN;
  static const string PACKET;
  static const string PACKETPROV;
  static const string PROGRAMID;
  static const string PROVHASHTABLE;
  static const string PROVLINK;
  static const string PROV_RH2_5RECVPACKETSEND;
  static const string RITERATE;
  static const string RQLIST;
  static const string RRESULTTMP;
  static const string RRETURN;
  static const string RECVPACKET;
  static const string RECVPACKETNP;
  static const string RECVPACKETPROV;
  static const string RULEEXEC;
  static const string RULEQUERY;

  static TypeId GetTypeId (void);

  PktfwdNormDistHlistOnlineQuery ();

  virtual ~PktfwdNormDistHlistOnlineQuery ();

protected:

  virtual void DoDispose (void);

  virtual void StartApplication (void);

  virtual void StopApplication (void);

  virtual void InitDatabase (void);

  virtual void DemuxRecv (Ptr<Tuple> tuple);

  virtual void Prov_rs1_1_eca (Ptr<Tuple> packetProv);

  virtual void Prov_rs1_2_eca (Ptr<Tuple> epacketTemp);

  virtual void Prov_rs1_3_eca (Ptr<Tuple> epacketCount);

  virtual void Prov_rs1_4_eca (Ptr<Tuple> epacketTemp);

  virtual void Prov_rs1_5_eca (Ptr<Tuple> epacketTemp);

  virtual void Rs1_eca (Ptr<Tuple> packet);

  virtual void Prov_ri1Eca0Ins (Ptr<Tuple> initPacket);

  virtual void Prov_ri1Eca1Ins (Ptr<Tuple> programID);

  virtual void Prov_ri2_eca (Ptr<Tuple> initPacketHash);

  virtual void Prov_ri3_eca (Ptr<Tuple> initPacketCount);

  virtual void Rh1_eca (Ptr<Tuple> initPacketCount);

  virtual void Prov_rh1_1_eca (Ptr<Tuple> initPacketCount);

  virtual void Prov_rh2_1_eca (Ptr<Tuple> packetProv);

  virtual void Prov_rh2_2_eca (Ptr<Tuple> erecvPacketTemp);

  virtual void Prov_rh2_4_eca (Ptr<Tuple> erecvPacketTemp);

  virtual void Prov_rh2_5ECAMat (Ptr<Tuple> prov_rh2_5recvPacketsend);

  virtual void Prov_rh2_5_eca (Ptr<Tuple> erecvPacketTemp);

  virtual void Ro1Eca0Ins (Ptr<Tuple> recvPacket);

  virtual void Ro1Eca0Del (Ptr<Tuple> recvPacket);

  virtual void Ro2Eca0Ins (Ptr<Tuple> recvPacket);

  virtual void Ro2Eca0Del (Ptr<Tuple> recvPacket);

  virtual void Rh2_eca (Ptr<Tuple> packet);

  virtual void Rho3_eca (Ptr<Tuple> recvPacketNP);

  virtual void Edb1_eca (Ptr<Tuple> baseQuery);

  virtual void Idb1Eca0Ins (Ptr<Tuple> recvPacketProv);

  virtual void Idb1Eca1Ins (Ptr<Tuple> provHashTable);

  virtual void Idb2Eca2Ins (Ptr<Tuple> recvPacketProv);

  virtual void Idb2Eca2Del (Ptr<Tuple> recvPacketProv);

  virtual void Idb7_eca (Ptr<Tuple> rReturn);

  virtual void Idb9Eca0Ins (Ptr<Tuple> pResultTmp);

  virtual void Rv1_eca (Ptr<Tuple> ruleQuery);

  virtual void Rv2_eca (Ptr<Tuple> ruleQuery);

  virtual void Rv2_eca (Ptr<Tuple> ruleQuery);

  virtual void Rv3_eca (Ptr<Tuple> ruleQuery);

  virtual void Rv4_eca (Ptr<Tuple> ruleQuery);

  virtual void Rv5_eca (Ptr<Tuple> rIterate);

  virtual void Rv6_eca (Ptr<Tuple> rIterate);

  virtual void Rv7_eca (Ptr<Tuple> baseReturn);

  virtual void Rv8_eca (Ptr<Tuple> rReturn);

  virtual void Rv9Eca0Ins (Ptr<Tuple> rResultTmp);

  virtual void Rv9Eca1Ins (Ptr<Tuple> rQList);

  virtual void Rv10_eca (Ptr<Tuple> eRReturn);

};

} // namespace pktfwdnormdisthlistonlinequery
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMDISTHLISTONLINEQUERY_H
