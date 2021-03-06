/* A RapidNet application. Generated by RapidNet compiler. */

#ifndef PKTFWDONLINEBCASTSLOWCHANGEUPDATEQUERY_H
#define PKTFWDONLINEBCASTSLOWCHANGEUPDATEQUERY_H

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
namespace pktfwdonlinebcastslowchangeupdatequery {

class PktfwdOnlineBcastSlowchangeUpdateQuery : public RapidNetApplicationBase
{
public:
  static const string EPACKETCOUNT;
  static const string EPACKETTEMP;
  static const string EQUIHASHTABLE;
  static const string ERECVPACKETTEMP;
  static const string FLOWENTRY;
  static const string INITPACKET;
  static const string INITPACKETCOUNT;
  static const string INITPACKETHASH;
  static const string INITPACKETNOPROV;
  static const string INITPACKETPROV;
  static const string LINK;
  static const string PACKET;
  static const string PACKETPROV;
  static const string PROGRAMID;
  static const string PROVHASHTABLE;
  static const string PROVLINK;
  static const string PROVQUERY;
  static const string PROVREF;
  static const string PROVRESULT;
  static const string PROVSTR;
  static const string RQUERY;
  static const string RRETURN;
  static const string RRETURNSIG;
  static const string RECORDRULE;
  static const string RECVPACKET;
  static const string RECVPACKETNP;
  static const string RECVPACKETPROV;
  static const string RESETTRIGGER;
  static const string RULEEXEC;
  static const string RULEINFO;
  static const string SLOWCHANGEQUERY;
  static const string TQUERY;
  static const string TRETURN;
  static const string TRIGRECORD;
  static const string TRIGRECORDCOUNT;
  static const string TUPLEITR;
  static const string UPDATEROUTE;

  static TypeId GetTypeId (void);

  PktfwdOnlineBcastSlowchangeUpdateQuery ();

  virtual ~PktfwdOnlineBcastSlowchangeUpdateQuery ();

protected:

  virtual void DoDispose (void);

  virtual void StartApplication (void);

  virtual void StopApplication (void);

  virtual void InitDatabase (void);

  virtual void DemuxRecv (Ptr<Tuple> tuple);

  virtual void Rr01Eca0Ins (Ptr<Tuple> recordRule);

  virtual void Rr01Eca0Del (Ptr<Tuple> recordRule);

  virtual void Rr02Eca0Ins (Ptr<Tuple> recordRule);

  virtual void Rr02Eca0Del (Ptr<Tuple> recordRule);

  virtual void Rr03Eca0Ins (Ptr<Tuple> recordRule);

  virtual void Rr03Eca0Del (Ptr<Tuple> recordRule);

  virtual void Prov_r00Eca0Ins (Ptr<Tuple> initPacket);

  virtual void Prov_r00Eca0Del (Ptr<Tuple> initPacket);

  virtual void Prov_r01Eca0Ins (Ptr<Tuple> link);

  virtual void Prov_r01Eca0Del (Ptr<Tuple> link);

  virtual void Prov_r03Eca0Ins (Ptr<Tuple> flowEntry);

  virtual void Prov_r03Eca0Del (Ptr<Tuple> flowEntry);

  virtual void Prov_rs1_1_eca (Ptr<Tuple> packetProv);

  virtual void Prov_rs1_2_eca (Ptr<Tuple> epacketTemp);

  virtual void Prov_rs1_3_eca (Ptr<Tuple> epacketCount);

  virtual void Prov_rs1_4_eca (Ptr<Tuple> epacketTemp);

  virtual void Prov_rs1_5_eca (Ptr<Tuple> epacketTemp);

  virtual void Rs1_eca (Ptr<Tuple> packet);

  virtual void Prov_ri1Eca0Ins (Ptr<Tuple> initPacket);

  virtual void Prov_ri1Eca1Ins (Ptr<Tuple> programID);

  virtual void Prov_ri2_eca (Ptr<Tuple> initPacketHash);

  virtual void Prov_ri3_eca (Ptr<Tuple> initPacketHash);

  virtual void Prov_ri4_eca (Ptr<Tuple> initPacketHash);

  virtual void Prov_ri5_eca (Ptr<Tuple> initPacketProv);

  virtual void Prov_ri6_eca (Ptr<Tuple> initPacketCount);

  virtual void Rh1_eca (Ptr<Tuple> initPacketNoProv);

  virtual void Prov_rh1_1_eca (Ptr<Tuple> initPacketCount);

  virtual void Prov_rh1_2_eca (Ptr<Tuple> initPacketProv);

  virtual void Prov_rh2_1_eca (Ptr<Tuple> packetProv);

  virtual void Prov_rh2_2_eca (Ptr<Tuple> erecvPacketTemp);

  virtual void Prov_rh2_4_eca (Ptr<Tuple> erecvPacketTemp);

  virtual void Prov_rh2_5_eca (Ptr<Tuple> erecvPacketTemp);

  virtual void Ro1_eca (Ptr<Tuple> recvPacketProv);

  virtual void Ro2_eca (Ptr<Tuple> recvPacketProv);

  virtual void Ro3_eca (Ptr<Tuple> recvPacketProv);

  virtual void Rh2_eca (Ptr<Tuple> packet);

  virtual void Rho3_eca (Ptr<Tuple> recvPacketNP);

  virtual void Rho4_eca (Ptr<Tuple> recvPacketNP);

  virtual void Ru1Eca0Ins (Ptr<Tuple> updateRoute);

  virtual void Ru1Eca0Del (Ptr<Tuple> updateRoute);

  virtual void Ru2Eca0Ins (Ptr<Tuple> updateRoute);

  virtual void Ru3_eca (Ptr<Tuple> resetTrigger);

  virtual void Ru4_eca (Ptr<Tuple> trigRecordCount);

  virtual void Ru5_eca (Ptr<Tuple> trigRecordCount);

  virtual void Ru6_eca (Ptr<Tuple> trigRecordCount);

  virtual void Rqini_eca (Ptr<Tuple> provQuery);

  virtual void Rqev_eca (Ptr<Tuple> rQuery);

  virtual void Rqrec_eca (Ptr<Tuple> rQuery);

  virtual void Rqsc1_eca (Ptr<Tuple> rQuery);

  virtual void Rqsc2Eca0Ins (Ptr<Tuple> slowChangeQuery);

  virtual void Rqsc3_eca (Ptr<Tuple> tupleItr);

  virtual void Rqsc4_eca (Ptr<Tuple> tupleItr);

  virtual void Rqsc5_eca (Ptr<Tuple> tQuery);

  virtual void Rqrt1_eca (Ptr<Tuple> rQuery);

  virtual void Rqrt2_eca (Ptr<Tuple> tReturn);

  virtual void Rqrt3_eca (Ptr<Tuple> rReturn);

  virtual void Rqrt4Eca0Ins (Ptr<Tuple> provResult);

  virtual void Rqrt5_eca (Ptr<Tuple> rReturnSig);

};

} // namespace pktfwdonlinebcastslowchangeupdatequery
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDONLINEBCASTSLOWCHANGEUPDATEQUERY_H
