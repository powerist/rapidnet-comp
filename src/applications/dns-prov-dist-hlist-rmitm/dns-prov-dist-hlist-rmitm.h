/* A RapidNet application. Generated by RapidNet compiler. */

#ifndef DNSPROVDISTHLISTRMITM_H
#define DNSPROVDISTHLISTRMITM_H

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
namespace dnsprovdisthlistrmitm {

class DnsProvDistHlistRmitm : public RapidNetApplicationBase
{
public:
  static const string ADDRESS_RECORD;
  static const string EREQUESTTEMP;
  static const string ERESULTTEMP;
  static const string NAME_SERVER;
  static const string PROV;
  static const string PROV_R3_3RESULTSSEND;
  static const string REQUEST;
  static const string RESULTS;
  static const string RULEEXEC;
  static const string URL;

  static TypeId GetTypeId (void);

  DnsProvDistHlistRmitm ();

  virtual ~DnsProvDistHlistRmitm ();

protected:

  virtual void DoDispose (void);

  virtual void StartApplication (void);

  virtual void StopApplication (void);

  virtual void InitDatabase (void);

  virtual void DemuxRecv (Ptr<Tuple> tuple);

  virtual void Prov_r1_1Eca0Ins (Ptr<Tuple> url);

  virtual void Prov_r1_1Eca1Ins (Ptr<Tuple> name_server);

  virtual void Prov_r1_1Eca2Ins (Ptr<Tuple> address_record);

  virtual void Prov_r2_1_eca (Ptr<Tuple> request);

  virtual void Prov_r2_2_eca (Ptr<Tuple> eRequestTemp);

  virtual void Prov_r2_3_eca (Ptr<Tuple> eRequestTemp);

  virtual void Prov_r3_1_eca (Ptr<Tuple> request);

  virtual void Prov_r3_2_eca (Ptr<Tuple> eResultTemp);

  virtual void Prov_r3_3ECAMat (Ptr<Tuple> prov_r3_3resultssend);

  virtual void Prov_r3_3_eca (Ptr<Tuple> eResultTemp);

  virtual void Prov_r3_4Eca0Ins (Ptr<Tuple> results);

  virtual void Prov_r3_4Eca0Del (Ptr<Tuple> results);

};

} // namespace dnsprovdisthlistrmitm
} // namespace rapidnet
} // namespace ns3

#endif // DNSPROVDISTHLISTRMITM_H