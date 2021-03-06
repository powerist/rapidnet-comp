/* A RapidNet application. Generated by RapidNet compiler. */

#include "pktfwd-norm-dist-hlist-rmitm-auxtables.h"
#include <cstdlib>
#include "ns3/nstime.h"
#include "ns3/simulator.h"
#include "ns3/type-ids.h"
#include "ns3/rapidnet-types.h"
#include "ns3/rapidnet-utils.h"
#include "ns3/assignor.h"
#include "ns3/selector.h"
#include "ns3/rapidnet-functions.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;
using namespace ns3::rapidnet::pktfwdnormdisthlistrmitmauxtables;

const string PktfwdNormDistHlistRmitmAuxtables::EPACKET = "epacket";
const string PktfwdNormDistHlistRmitmAuxtables::EPACKETTEMP = "epacketTemp";
const string PktfwdNormDistHlistRmitmAuxtables::ERECVPACKET = "erecvPacket";
const string PktfwdNormDistHlistRmitmAuxtables::ERECVPACKETTEMP = "erecvPacketTemp";
const string PktfwdNormDistHlistRmitmAuxtables::FLOWENTRY = "flowEntry";
const string PktfwdNormDistHlistRmitmAuxtables::INITPACKET = "initPacket";
const string PktfwdNormDistHlistRmitmAuxtables::LINK = "link";
const string PktfwdNormDistHlistRmitmAuxtables::PACKET = "packet";
const string PktfwdNormDistHlistRmitmAuxtables::PROV = "prov";
const string PktfwdNormDistHlistRmitmAuxtables::PROVSTR = "provStr";
const string PktfwdNormDistHlistRmitmAuxtables::RECORDRULE = "recordRule";
const string PktfwdNormDistHlistRmitmAuxtables::RECVPACKET = "recvPacket";
const string PktfwdNormDistHlistRmitmAuxtables::RULEEXEC = "ruleExec";
const string PktfwdNormDistHlistRmitmAuxtables::RULEINFO = "ruleInfo";

NS_LOG_COMPONENT_DEFINE ("PktfwdNormDistHlistRmitmAuxtables");
NS_OBJECT_ENSURE_REGISTERED (PktfwdNormDistHlistRmitmAuxtables);

TypeId
PktfwdNormDistHlistRmitmAuxtables::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::rapidnet::pktfwdnormdisthlistrmitmauxtables::PktfwdNormDistHlistRmitmAuxtables")
    .SetParent<RapidNetApplicationBase> ()
    .AddConstructor<PktfwdNormDistHlistRmitmAuxtables> ()
    ;
  return tid;
}

PktfwdNormDistHlistRmitmAuxtables::PktfwdNormDistHlistRmitmAuxtables()
{
  NS_LOG_FUNCTION_NOARGS ();
}

PktfwdNormDistHlistRmitmAuxtables::~PktfwdNormDistHlistRmitmAuxtables()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
PktfwdNormDistHlistRmitmAuxtables::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  RapidNetApplicationBase::DoDispose ();
}

void
PktfwdNormDistHlistRmitmAuxtables::StartApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  RapidNetApplicationBase::StartApplication ();
  RAPIDNET_LOG_INFO("PktfwdNormDistHlistRmitmAuxtables Application Started");
}

void
PktfwdNormDistHlistRmitmAuxtables::StopApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();

  RapidNetApplicationBase::StopApplication ();
  RAPIDNET_LOG_INFO("PktfwdNormDistHlistRmitmAuxtables Application Stopped");
}

void
PktfwdNormDistHlistRmitmAuxtables::InitDatabase ()
{
  //RapidNetApplicationBase::InitDatabase ();

  AddRelationWithKeys (FLOWENTRY, attrdeflist (
    attrdef ("flowEntry_attr2", IPV4)));

  AddRelationWithKeys (INITPACKET, attrdeflist (
    attrdef ("initPacket_attr2", IPV4),
    attrdef ("initPacket_attr3", IPV4),
    attrdef ("initPacket_attr4", STR)));

  AddRelationWithKeys (LINK, attrdeflist (
    attrdef ("link_attr2", IPV4)));

  AddRelationWithKeys (PROV, attrdeflist (
    attrdef ("prov_attr2", ID),
    attrdef ("prov_attr3", IPV4),
    attrdef ("prov_attr4", ID)));

  AddRelationWithKeys (PROVSTR, attrdeflist (
    attrdef ("provStr_attr2", ID)));

  AddRelationWithKeys (RECORDRULE, attrdeflist (
    attrdef ("recordRule_attr1", IPV4)));

  AddRelationWithKeys (RECVPACKET, attrdeflist (
    attrdef ("recvPacket_attr2", IPV4),
    attrdef ("recvPacket_attr3", IPV4),
    attrdef ("recvPacket_attr4", STR)));

  AddRelationWithKeys (RULEEXEC, attrdeflist (
    attrdef ("ruleExec_attr2", ID),
    attrdef ("ruleExec_attr5", IPV4),
    attrdef ("ruleExec_attr6", ID)));

  AddRelationWithKeys (RULEINFO, attrdeflist (
    attrdef ("ruleInfo_attr2", STR)));

}

void
PktfwdNormDistHlistRmitmAuxtables::DemuxRecv (Ptr<Tuple> tuple)
{
  RapidNetApplicationBase::DemuxRecv (tuple);

  if (IsInsertEvent (tuple, RECORDRULE))
    {
      Rr01Eca0Ins (tuple);
    }
  if (IsDeleteEvent (tuple, RECORDRULE))
    {
      Rr01Eca0Del (tuple);
    }
  if (IsInsertEvent (tuple, RECORDRULE))
    {
      Rr02Eca0Ins (tuple);
    }
  if (IsDeleteEvent (tuple, RECORDRULE))
    {
      Rr02Eca0Del (tuple);
    }
  if (IsInsertEvent (tuple, RECORDRULE))
    {
      Rr03Eca0Ins (tuple);
    }
  if (IsDeleteEvent (tuple, RECORDRULE))
    {
      Rr03Eca0Del (tuple);
    }
  if (IsInsertEvent (tuple, INITPACKET))
    {
      Prov_r00Eca0Ins (tuple);
    }
  if (IsDeleteEvent (tuple, INITPACKET))
    {
      Prov_r00Eca0Del (tuple);
    }
  if (IsInsertEvent (tuple, LINK))
    {
      Prov_r01Eca0Ins (tuple);
    }
  if (IsDeleteEvent (tuple, LINK))
    {
      Prov_r01Eca0Del (tuple);
    }
  if (IsInsertEvent (tuple, FLOWENTRY))
    {
      Prov_r03Eca0Ins (tuple);
    }
  if (IsDeleteEvent (tuple, FLOWENTRY))
    {
      Prov_r03Eca0Del (tuple);
    }
  if (IsRecvEvent (tuple, EPACKET))
    {
      Prov_rs1_1_eca (tuple);
    }
  if (IsRecvEvent (tuple, EPACKETTEMP))
    {
      Prov_rs1_2_eca (tuple);
    }
  if (IsRecvEvent (tuple, EPACKETTEMP))
    {
      Prov_rs1_3_eca (tuple);
    }
  if (IsRecvEvent (tuple, EPACKET))
    {
      Prov_rs1_4_eca (tuple);
    }
  if (IsInsertEvent (tuple, INITPACKET))
    {
      Prov_rh1_1Eca0Ins (tuple);
    }
  if (IsInsertEvent (tuple, LINK))
    {
      Prov_rh1_1Eca1Ins (tuple);
    }
  if (IsRecvEvent (tuple, EPACKET))
    {
      Prov_rh2_1_eca (tuple);
    }
  if (IsRecvEvent (tuple, ERECVPACKETTEMP))
    {
      Prov_rh2_2_eca (tuple);
    }
  if (IsRecvEvent (tuple, ERECVPACKETTEMP))
    {
      Prov_rh2_3_eca (tuple);
    }
  if (IsRecvEvent (tuple, ERECVPACKET))
    {
      Prov_rh2_5_eca (tuple);
    }
  if (IsRecvEvent (tuple, ERECVPACKET))
    {
      Prov_rh2_4_eca (tuple);
    }
}

void
PktfwdNormDistHlistRmitmAuxtables::Rr01Eca0Ins (Ptr<Tuple> recordRule)
{
  RAPIDNET_LOG_INFO ("Rr01Eca0Ins triggered");

  Ptr<Tuple> result = recordRule;

  result->Assign (Assignor::New ("RName",
    ValueExpr::New (StrValue::New ("rh1"))));

  result->Assign (Assignor::New ("RHead",
    ValueExpr::New (StrValue::New ("packet(Next,SrcAdd,DstAdd,Data)"))));

  result->Assign (Assignor::New ("RBody",
    ValueExpr::New (StrValue::New ("initPacket(Node,SrcAdd,DstAdd,Data);link(Node,Next)."))));

  result = result->Project (
    RULEINFO,
    strlist ("recordRule_attr1",
      "RName",
      "RHead",
      "RBody"),
    strlist ("ruleInfo_attr1",
      "ruleInfo_attr2",
      "ruleInfo_attr3",
      "ruleInfo_attr4"));

  Insert (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Rr01Eca0Del (Ptr<Tuple> recordRule)
{
  RAPIDNET_LOG_INFO ("Rr01Eca0Del triggered");

  Ptr<Tuple> result = recordRule;

  result->Assign (Assignor::New ("RName",
    ValueExpr::New (StrValue::New ("rh1"))));

  result->Assign (Assignor::New ("RHead",
    ValueExpr::New (StrValue::New ("packet(Next,SrcAdd,DstAdd,Data)"))));

  result->Assign (Assignor::New ("RBody",
    ValueExpr::New (StrValue::New ("initPacket(Node,SrcAdd,DstAdd,Data);link(Node,Next)."))));

  result = result->Project (
    RULEINFO,
    strlist ("recordRule_attr1",
      "RName",
      "RHead",
      "RBody"),
    strlist ("ruleInfo_attr1",
      "ruleInfo_attr2",
      "ruleInfo_attr3",
      "ruleInfo_attr4"));

  Delete (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Rr02Eca0Ins (Ptr<Tuple> recordRule)
{
  RAPIDNET_LOG_INFO ("Rr02Eca0Ins triggered");

  Ptr<Tuple> result = recordRule;

  result->Assign (Assignor::New ("RName",
    ValueExpr::New (StrValue::New ("rs1"))));

  result->Assign (Assignor::New ("RHead",
    ValueExpr::New (StrValue::New ("packet(Next,SrcAdd,DstAdd,Data)"))));

  result->Assign (Assignor::New ("RBody",
    ValueExpr::New (StrValue::New ("packet(Node,SrcAdd,DstAdd,Data);flowEntry(Node,DstAdd,Next);link(Node,Next)."))));

  result = result->Project (
    RULEINFO,
    strlist ("recordRule_attr1",
      "RName",
      "RHead",
      "RBody"),
    strlist ("ruleInfo_attr1",
      "ruleInfo_attr2",
      "ruleInfo_attr3",
      "ruleInfo_attr4"));

  Insert (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Rr02Eca0Del (Ptr<Tuple> recordRule)
{
  RAPIDNET_LOG_INFO ("Rr02Eca0Del triggered");

  Ptr<Tuple> result = recordRule;

  result->Assign (Assignor::New ("RName",
    ValueExpr::New (StrValue::New ("rs1"))));

  result->Assign (Assignor::New ("RHead",
    ValueExpr::New (StrValue::New ("packet(Next,SrcAdd,DstAdd,Data)"))));

  result->Assign (Assignor::New ("RBody",
    ValueExpr::New (StrValue::New ("packet(Node,SrcAdd,DstAdd,Data);flowEntry(Node,DstAdd,Next);link(Node,Next)."))));

  result = result->Project (
    RULEINFO,
    strlist ("recordRule_attr1",
      "RName",
      "RHead",
      "RBody"),
    strlist ("ruleInfo_attr1",
      "ruleInfo_attr2",
      "ruleInfo_attr3",
      "ruleInfo_attr4"));

  Delete (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Rr03Eca0Ins (Ptr<Tuple> recordRule)
{
  RAPIDNET_LOG_INFO ("Rr03Eca0Ins triggered");

  Ptr<Tuple> result = recordRule;

  result->Assign (Assignor::New ("RName",
    ValueExpr::New (StrValue::New ("rh2"))));

  result->Assign (Assignor::New ("RHead",
    ValueExpr::New (StrValue::New ("recvPacket(Node,SrcAdd,DstAdd,Data)"))));

  result->Assign (Assignor::New ("RBody",
    ValueExpr::New (StrValue::New ("packet(Node,SrcAdd,DstAdd,Data);DstAdd==Node."))));

  result = result->Project (
    RULEINFO,
    strlist ("recordRule_attr1",
      "RName",
      "RHead",
      "RBody"),
    strlist ("ruleInfo_attr1",
      "ruleInfo_attr2",
      "ruleInfo_attr3",
      "ruleInfo_attr4"));

  Insert (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Rr03Eca0Del (Ptr<Tuple> recordRule)
{
  RAPIDNET_LOG_INFO ("Rr03Eca0Del triggered");

  Ptr<Tuple> result = recordRule;

  result->Assign (Assignor::New ("RName",
    ValueExpr::New (StrValue::New ("rh2"))));

  result->Assign (Assignor::New ("RHead",
    ValueExpr::New (StrValue::New ("recvPacket(Node,SrcAdd,DstAdd,Data)"))));

  result->Assign (Assignor::New ("RBody",
    ValueExpr::New (StrValue::New ("packet(Node,SrcAdd,DstAdd,Data);DstAdd==Node."))));

  result = result->Project (
    RULEINFO,
    strlist ("recordRule_attr1",
      "RName",
      "RHead",
      "RBody"),
    strlist ("ruleInfo_attr1",
      "ruleInfo_attr2",
      "ruleInfo_attr3",
      "ruleInfo_attr4"));

  Delete (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Prov_r00Eca0Ins (Ptr<Tuple> initPacket)
{
  RAPIDNET_LOG_INFO ("Prov_r00Eca0Ins triggered");

  Ptr<Tuple> result = initPacket;

  result->Assign (Assignor::New ("VID",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          Operation::New (RN_PLUS,
            Operation::New (RN_PLUS,
              ValueExpr::New (StrValue::New ("initPacket")),
              VarExpr::New ("initPacket_attr1")),
            VarExpr::New ("initPacket_attr2")),
          VarExpr::New ("initPacket_attr3")),
        VarExpr::New ("initPacket_attr4")))));

  result->Assign (Assignor::New ("TpStrList1",
    FAppend::New (
      ValueExpr::New (StrValue::New ("initPacket")))));

  result->Assign (Assignor::New ("TpStrList2",
    FAppend::New (
      VarExpr::New ("initPacket_attr1"))));

  result->Assign (Assignor::New ("TpStrList3",
    FConcat::New (
      VarExpr::New ("TpStrList1"),
      VarExpr::New ("TpStrList2"))));

  result->Assign (Assignor::New ("TpStrList4",
    FAppend::New (
      VarExpr::New ("initPacket_attr2"))));

  result->Assign (Assignor::New ("TpStrList5",
    FConcat::New (
      VarExpr::New ("TpStrList3"),
      VarExpr::New ("TpStrList4"))));

  result->Assign (Assignor::New ("TpStrList6",
    FAppend::New (
      VarExpr::New ("initPacket_attr3"))));

  result->Assign (Assignor::New ("TpStrList7",
    FConcat::New (
      VarExpr::New ("TpStrList5"),
      VarExpr::New ("TpStrList6"))));

  result->Assign (Assignor::New ("TpStrList8",
    FAppend::New (
      VarExpr::New ("initPacket_attr4"))));

  result->Assign (Assignor::New ("TpStrList",
    FConcat::New (
      VarExpr::New ("TpStrList7"),
      VarExpr::New ("TpStrList8"))));

  result = result->Project (
    PROVSTR,
    strlist ("initPacket_attr1",
      "VID",
      "TpStrList"),
    strlist ("provStr_attr1",
      "provStr_attr2",
      "provStr_attr3"));

  Insert (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Prov_r00Eca0Del (Ptr<Tuple> initPacket)
{
  RAPIDNET_LOG_INFO ("Prov_r00Eca0Del triggered");

  Ptr<Tuple> result = initPacket;

  result->Assign (Assignor::New ("VID",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          Operation::New (RN_PLUS,
            Operation::New (RN_PLUS,
              ValueExpr::New (StrValue::New ("initPacket")),
              VarExpr::New ("initPacket_attr1")),
            VarExpr::New ("initPacket_attr2")),
          VarExpr::New ("initPacket_attr3")),
        VarExpr::New ("initPacket_attr4")))));

  result->Assign (Assignor::New ("TpStrList1",
    FAppend::New (
      ValueExpr::New (StrValue::New ("initPacket")))));

  result->Assign (Assignor::New ("TpStrList2",
    FAppend::New (
      VarExpr::New ("initPacket_attr1"))));

  result->Assign (Assignor::New ("TpStrList3",
    FConcat::New (
      VarExpr::New ("TpStrList1"),
      VarExpr::New ("TpStrList2"))));

  result->Assign (Assignor::New ("TpStrList4",
    FAppend::New (
      VarExpr::New ("initPacket_attr2"))));

  result->Assign (Assignor::New ("TpStrList5",
    FConcat::New (
      VarExpr::New ("TpStrList3"),
      VarExpr::New ("TpStrList4"))));

  result->Assign (Assignor::New ("TpStrList6",
    FAppend::New (
      VarExpr::New ("initPacket_attr3"))));

  result->Assign (Assignor::New ("TpStrList7",
    FConcat::New (
      VarExpr::New ("TpStrList5"),
      VarExpr::New ("TpStrList6"))));

  result->Assign (Assignor::New ("TpStrList8",
    FAppend::New (
      VarExpr::New ("initPacket_attr4"))));

  result->Assign (Assignor::New ("TpStrList",
    FConcat::New (
      VarExpr::New ("TpStrList7"),
      VarExpr::New ("TpStrList8"))));

  result = result->Project (
    PROVSTR,
    strlist ("initPacket_attr1",
      "VID",
      "TpStrList"),
    strlist ("provStr_attr1",
      "provStr_attr2",
      "provStr_attr3"));

  Delete (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Prov_r01Eca0Ins (Ptr<Tuple> link)
{
  RAPIDNET_LOG_INFO ("Prov_r01Eca0Ins triggered");

  Ptr<Tuple> result = link;

  result->Assign (Assignor::New ("VID",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          ValueExpr::New (StrValue::New ("link")),
          VarExpr::New ("link_attr1")),
        VarExpr::New ("link_attr2")))));

  result->Assign (Assignor::New ("TpStrList1",
    FAppend::New (
      ValueExpr::New (StrValue::New ("link")))));

  result->Assign (Assignor::New ("TpStrList2",
    FAppend::New (
      VarExpr::New ("link_attr1"))));

  result->Assign (Assignor::New ("TpStrList3",
    FConcat::New (
      VarExpr::New ("TpStrList1"),
      VarExpr::New ("TpStrList2"))));

  result->Assign (Assignor::New ("TpStrList4",
    FAppend::New (
      VarExpr::New ("link_attr2"))));

  result->Assign (Assignor::New ("TpStrList",
    FConcat::New (
      VarExpr::New ("TpStrList3"),
      VarExpr::New ("TpStrList4"))));

  result = result->Project (
    PROVSTR,
    strlist ("link_attr1",
      "VID",
      "TpStrList"),
    strlist ("provStr_attr1",
      "provStr_attr2",
      "provStr_attr3"));

  Insert (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Prov_r01Eca0Del (Ptr<Tuple> link)
{
  RAPIDNET_LOG_INFO ("Prov_r01Eca0Del triggered");

  Ptr<Tuple> result = link;

  result->Assign (Assignor::New ("VID",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          ValueExpr::New (StrValue::New ("link")),
          VarExpr::New ("link_attr1")),
        VarExpr::New ("link_attr2")))));

  result->Assign (Assignor::New ("TpStrList1",
    FAppend::New (
      ValueExpr::New (StrValue::New ("link")))));

  result->Assign (Assignor::New ("TpStrList2",
    FAppend::New (
      VarExpr::New ("link_attr1"))));

  result->Assign (Assignor::New ("TpStrList3",
    FConcat::New (
      VarExpr::New ("TpStrList1"),
      VarExpr::New ("TpStrList2"))));

  result->Assign (Assignor::New ("TpStrList4",
    FAppend::New (
      VarExpr::New ("link_attr2"))));

  result->Assign (Assignor::New ("TpStrList",
    FConcat::New (
      VarExpr::New ("TpStrList3"),
      VarExpr::New ("TpStrList4"))));

  result = result->Project (
    PROVSTR,
    strlist ("link_attr1",
      "VID",
      "TpStrList"),
    strlist ("provStr_attr1",
      "provStr_attr2",
      "provStr_attr3"));

  Delete (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Prov_r03Eca0Ins (Ptr<Tuple> flowEntry)
{
  RAPIDNET_LOG_INFO ("Prov_r03Eca0Ins triggered");

  Ptr<Tuple> result = flowEntry;

  result->Assign (Assignor::New ("VID",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          Operation::New (RN_PLUS,
            ValueExpr::New (StrValue::New ("flowEntry")),
            VarExpr::New ("flowEntry_attr1")),
          VarExpr::New ("flowEntry_attr2")),
        VarExpr::New ("flowEntry_attr3")))));

  result->Assign (Assignor::New ("TpStrList1",
    FAppend::New (
      ValueExpr::New (StrValue::New ("flowEntry")))));

  result->Assign (Assignor::New ("TpStrList2",
    FAppend::New (
      VarExpr::New ("flowEntry_attr1"))));

  result->Assign (Assignor::New ("TpStrList3",
    FConcat::New (
      VarExpr::New ("TpStrList1"),
      VarExpr::New ("TpStrList2"))));

  result->Assign (Assignor::New ("TpStrList4",
    FAppend::New (
      VarExpr::New ("flowEntry_attr2"))));

  result->Assign (Assignor::New ("TpStrList5",
    FConcat::New (
      VarExpr::New ("TpStrList3"),
      VarExpr::New ("TpStrList4"))));

  result->Assign (Assignor::New ("TpStrList6",
    FAppend::New (
      VarExpr::New ("flowEntry_attr3"))));

  result->Assign (Assignor::New ("TpStrList",
    FConcat::New (
      VarExpr::New ("TpStrList5"),
      VarExpr::New ("TpStrList6"))));

  result = result->Project (
    PROVSTR,
    strlist ("flowEntry_attr1",
      "VID",
      "TpStrList"),
    strlist ("provStr_attr1",
      "provStr_attr2",
      "provStr_attr3"));

  Insert (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Prov_r03Eca0Del (Ptr<Tuple> flowEntry)
{
  RAPIDNET_LOG_INFO ("Prov_r03Eca0Del triggered");

  Ptr<Tuple> result = flowEntry;

  result->Assign (Assignor::New ("VID",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          Operation::New (RN_PLUS,
            ValueExpr::New (StrValue::New ("flowEntry")),
            VarExpr::New ("flowEntry_attr1")),
          VarExpr::New ("flowEntry_attr2")),
        VarExpr::New ("flowEntry_attr3")))));

  result->Assign (Assignor::New ("TpStrList1",
    FAppend::New (
      ValueExpr::New (StrValue::New ("flowEntry")))));

  result->Assign (Assignor::New ("TpStrList2",
    FAppend::New (
      VarExpr::New ("flowEntry_attr1"))));

  result->Assign (Assignor::New ("TpStrList3",
    FConcat::New (
      VarExpr::New ("TpStrList1"),
      VarExpr::New ("TpStrList2"))));

  result->Assign (Assignor::New ("TpStrList4",
    FAppend::New (
      VarExpr::New ("flowEntry_attr2"))));

  result->Assign (Assignor::New ("TpStrList5",
    FConcat::New (
      VarExpr::New ("TpStrList3"),
      VarExpr::New ("TpStrList4"))));

  result->Assign (Assignor::New ("TpStrList6",
    FAppend::New (
      VarExpr::New ("flowEntry_attr3"))));

  result->Assign (Assignor::New ("TpStrList",
    FConcat::New (
      VarExpr::New ("TpStrList5"),
      VarExpr::New ("TpStrList6"))));

  result = result->Project (
    PROVSTR,
    strlist ("flowEntry_attr1",
      "VID",
      "TpStrList"),
    strlist ("provStr_attr1",
      "provStr_attr2",
      "provStr_attr3"));

  Delete (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Prov_rs1_1_eca (Ptr<Tuple> epacket)
{
  RAPIDNET_LOG_INFO ("Prov_rs1_1_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (FLOWENTRY)->Join (
    epacket,
    strlist ("flowEntry_attr2", "flowEntry_attr1"),
    strlist ("epacket_attr3", "epacket_attr1"));

  result = GetRelation (LINK)->Join (
    result,
    strlist ("link_attr2", "link_attr1"),
    strlist ("flowEntry_attr3", "epacket_attr1"));

  result->Assign (Assignor::New ("PID3",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          Operation::New (RN_PLUS,
            ValueExpr::New (StrValue::New ("flowEntry")),
            VarExpr::New ("epacket_attr1")),
          VarExpr::New ("epacket_attr3")),
        VarExpr::New ("flowEntry_attr3")))));

  result->Assign (Assignor::New ("List3",
    FAppend::New (
      VarExpr::New ("PID3"))));

  result->Assign (Assignor::New ("PID4",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          ValueExpr::New (StrValue::New ("link")),
          VarExpr::New ("epacket_attr1")),
        VarExpr::New ("flowEntry_attr3")))));

  result->Assign (Assignor::New ("List4",
    FAppend::New (
      VarExpr::New ("PID4"))));

  result->Assign (Assignor::New ("List",
    FConcat::New (
      VarExpr::New ("List3"),
      VarExpr::New ("List4"))));

  result->Assign (Assignor::New ("RLOC",
    VarExpr::New ("epacket_attr1")));

  result->Assign (Assignor::New ("R",
    ValueExpr::New (StrValue::New ("rs1"))));

  result->Assign (Assignor::New ("RID",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          VarExpr::New ("R"),
          VarExpr::New ("RLOC")),
        VarExpr::New ("List")))));

  result->Assign (Assignor::New ("PreLoclist",
    FAppend::New (
      VarExpr::New ("epacket_attr5"))));

  result->Assign (Assignor::New ("PreRIDlist",
    FAppend::New (
      VarExpr::New ("epacket_attr6"))));

  result->Assign (Assignor::New ("PreInfolist",
    FConcat::New (
      VarExpr::New ("PreLoclist"),
      VarExpr::New ("PreRIDlist"))));

  result = result->Project (
    EPACKETTEMP,
    strlist ("RLOC",
      "flowEntry_attr3",
      "epacket_attr2",
      "epacket_attr3",
      "epacket_attr4",
      "RID",
      "R",
      "List",
      "PreInfolist",
      "RLOC"),
    strlist ("epacketTemp_attr1",
      "epacketTemp_attr2",
      "epacketTemp_attr3",
      "epacketTemp_attr4",
      "epacketTemp_attr5",
      "epacketTemp_attr6",
      "epacketTemp_attr7",
      "epacketTemp_attr8",
      "epacketTemp_attr9",
      RN_DEST));

  Send (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Prov_rs1_2_eca (Ptr<Tuple> epacketTemp)
{
  RAPIDNET_LOG_INFO ("Prov_rs1_2_eca triggered");

  Ptr<Tuple> result = epacketTemp;

  result->Assign (Assignor::New ("PreLoc",
    FFirst::New (
      VarExpr::New ("epacketTemp_attr9"))));

  result->Assign (Assignor::New ("Remainlist",
    FRemoveFirst::New (
      VarExpr::New ("epacketTemp_attr9"))));

  result->Assign (Assignor::New ("PreRID",
    FFirst::New (
      VarExpr::New ("Remainlist"))));

  result = result->Project (
    RULEEXEC,
    strlist ("epacketTemp_attr1",
      "epacketTemp_attr6",
      "epacketTemp_attr7",
      "epacketTemp_attr8",
      "PreLoc",
      "PreRID"),
    strlist ("ruleExec_attr1",
      "ruleExec_attr2",
      "ruleExec_attr3",
      "ruleExec_attr4",
      "ruleExec_attr5",
      "ruleExec_attr6"));

  Insert (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Prov_rs1_3_eca (Ptr<Tuple> epacketTemp)
{
  RAPIDNET_LOG_INFO ("Prov_rs1_3_eca triggered");

  Ptr<Tuple> result = epacketTemp;

  result = result->Project (
    EPACKET,
    strlist ("epacketTemp_attr2",
      "epacketTemp_attr3",
      "epacketTemp_attr4",
      "epacketTemp_attr5",
      "epacketTemp_attr1",
      "epacketTemp_attr6",
      "epacketTemp_attr2"),
    strlist ("epacket_attr1",
      "epacket_attr2",
      "epacket_attr3",
      "epacket_attr4",
      "epacket_attr5",
      "epacket_attr6",
      RN_DEST));

  Send (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Prov_rs1_4_eca (Ptr<Tuple> epacket)
{
  RAPIDNET_LOG_INFO ("Prov_rs1_4_eca triggered");

  Ptr<Tuple> result = epacket;

  result = result->Project (
    PACKET,
    strlist ("epacket_attr1",
      "epacket_attr2",
      "epacket_attr3",
      "epacket_attr4"),
    strlist ("packet_attr1",
      "packet_attr2",
      "packet_attr3",
      "packet_attr4"));

  SendLocal (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Prov_rh1_1Eca0Ins (Ptr<Tuple> initPacket)
{
  RAPIDNET_LOG_INFO ("Prov_rh1_1Eca0Ins triggered");

  Ptr<RelationBase> result;

  result = GetRelation (LINK)->Join (
    initPacket,
    strlist ("link_attr1"),
    strlist ("initPacket_attr1"));

  result->Assign (Assignor::New ("PID2",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          Operation::New (RN_PLUS,
            Operation::New (RN_PLUS,
              ValueExpr::New (StrValue::New ("initPacket")),
              VarExpr::New ("initPacket_attr1")),
            VarExpr::New ("initPacket_attr2")),
          VarExpr::New ("initPacket_attr3")),
        VarExpr::New ("initPacket_attr4")))));

  result->Assign (Assignor::New ("List",
    FAppend::New (
      VarExpr::New ("PID2"))));

  result->Assign (Assignor::New ("PID3",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          ValueExpr::New (StrValue::New ("link")),
          VarExpr::New ("initPacket_attr1")),
        VarExpr::New ("link_attr2")))));

  result->Assign (Assignor::New ("List3",
    FAppend::New (
      VarExpr::New ("PID3"))));

  result->Assign (Assignor::New ("List",
    FConcat::New (
      VarExpr::New ("List"),
      VarExpr::New ("List3"))));

  result->Assign (Assignor::New ("PreLoc",
    VarExpr::New ("initPacket_attr1")));

  result->Assign (Assignor::New ("PreLoclist",
    FAppend::New (
      VarExpr::New ("PreLoc"))));

  result->Assign (Assignor::New ("PreRID",
    FSha1::New (
      ValueExpr::New (StrValue::New ("NULL")))));

  result->Assign (Assignor::New ("PreRIDlist",
    FAppend::New (
      VarExpr::New ("PreRID"))));

  result->Assign (Assignor::New ("RLOC",
    VarExpr::New ("initPacket_attr1")));

  result->Assign (Assignor::New ("R",
    ValueExpr::New (StrValue::New ("rh1"))));

  result->Assign (Assignor::New ("RID",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          VarExpr::New ("R"),
          VarExpr::New ("RLOC")),
        VarExpr::New ("List")))));

  result->Assign (Assignor::New ("PreInfolist",
    FConcat::New (
      VarExpr::New ("PreLoclist"),
      VarExpr::New ("PreRIDlist"))));

  result = result->Project (
    EPACKETTEMP,
    strlist ("RLOC",
      "link_attr2",
      "initPacket_attr2",
      "initPacket_attr3",
      "initPacket_attr4",
      "RID",
      "R",
      "List",
      "PreInfolist",
      "RLOC"),
    strlist ("epacketTemp_attr1",
      "epacketTemp_attr2",
      "epacketTemp_attr3",
      "epacketTemp_attr4",
      "epacketTemp_attr5",
      "epacketTemp_attr6",
      "epacketTemp_attr7",
      "epacketTemp_attr8",
      "epacketTemp_attr9",
      RN_DEST));

  Send (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Prov_rh1_1Eca1Ins (Ptr<Tuple> link)
{
  RAPIDNET_LOG_INFO ("Prov_rh1_1Eca1Ins triggered");

  Ptr<RelationBase> result;

  result = GetRelation (INITPACKET)->Join (
    link,
    strlist ("initPacket_attr1"),
    strlist ("link_attr1"));

  result->Assign (Assignor::New ("PID2",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          Operation::New (RN_PLUS,
            Operation::New (RN_PLUS,
              ValueExpr::New (StrValue::New ("initPacket")),
              VarExpr::New ("link_attr1")),
            VarExpr::New ("initPacket_attr2")),
          VarExpr::New ("initPacket_attr3")),
        VarExpr::New ("initPacket_attr4")))));

  result->Assign (Assignor::New ("List",
    FAppend::New (
      VarExpr::New ("PID2"))));

  result->Assign (Assignor::New ("PID3",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          ValueExpr::New (StrValue::New ("link")),
          VarExpr::New ("link_attr1")),
        VarExpr::New ("link_attr2")))));

  result->Assign (Assignor::New ("List3",
    FAppend::New (
      VarExpr::New ("PID3"))));

  result->Assign (Assignor::New ("List",
    FConcat::New (
      VarExpr::New ("List"),
      VarExpr::New ("List3"))));

  result->Assign (Assignor::New ("PreLoc",
    VarExpr::New ("link_attr1")));

  result->Assign (Assignor::New ("PreLoclist",
    FAppend::New (
      VarExpr::New ("PreLoc"))));

  result->Assign (Assignor::New ("PreRID",
    FSha1::New (
      ValueExpr::New (StrValue::New ("NULL")))));

  result->Assign (Assignor::New ("PreRIDlist",
    FAppend::New (
      VarExpr::New ("PreRID"))));

  result->Assign (Assignor::New ("RLOC",
    VarExpr::New ("link_attr1")));

  result->Assign (Assignor::New ("R",
    ValueExpr::New (StrValue::New ("rh1"))));

  result->Assign (Assignor::New ("RID",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          VarExpr::New ("R"),
          VarExpr::New ("RLOC")),
        VarExpr::New ("List")))));

  result->Assign (Assignor::New ("PreInfolist",
    FConcat::New (
      VarExpr::New ("PreLoclist"),
      VarExpr::New ("PreRIDlist"))));

  result = result->Project (
    EPACKETTEMP,
    strlist ("RLOC",
      "link_attr2",
      "initPacket_attr2",
      "initPacket_attr3",
      "initPacket_attr4",
      "RID",
      "R",
      "List",
      "PreInfolist",
      "RLOC"),
    strlist ("epacketTemp_attr1",
      "epacketTemp_attr2",
      "epacketTemp_attr3",
      "epacketTemp_attr4",
      "epacketTemp_attr5",
      "epacketTemp_attr6",
      "epacketTemp_attr7",
      "epacketTemp_attr8",
      "epacketTemp_attr9",
      RN_DEST));

  Send (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Prov_rh2_1_eca (Ptr<Tuple> epacket)
{
  RAPIDNET_LOG_INFO ("Prov_rh2_1_eca triggered");

  Ptr<Tuple> result = epacket;

  result->Assign (Assignor::New ("List",
    FEmpty::New (
)));

  result->Assign (Assignor::New ("RLOC",
    VarExpr::New ("epacket_attr1")));

  result->Assign (Assignor::New ("R",
    ValueExpr::New (StrValue::New ("rh2"))));

  result->Assign (Assignor::New ("RID",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          VarExpr::New ("R"),
          VarExpr::New ("RLOC")),
        VarExpr::New ("List")))));

  result->Assign (Assignor::New ("PreLoclist",
    FAppend::New (
      VarExpr::New ("epacket_attr5"))));

  result->Assign (Assignor::New ("PreRIDlist",
    FAppend::New (
      VarExpr::New ("epacket_attr6"))));

  result->Assign (Assignor::New ("PreInfolist",
    FConcat::New (
      VarExpr::New ("PreLoclist"),
      VarExpr::New ("PreRIDlist"))));

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("epacket_attr3"),
      VarExpr::New ("epacket_attr1"))));

  result = result->Project (
    ERECVPACKETTEMP,
    strlist ("RLOC",
      "epacket_attr1",
      "epacket_attr2",
      "epacket_attr3",
      "epacket_attr4",
      "RID",
      "R",
      "List",
      "PreInfolist",
      "RLOC"),
    strlist ("erecvPacketTemp_attr1",
      "erecvPacketTemp_attr2",
      "erecvPacketTemp_attr3",
      "erecvPacketTemp_attr4",
      "erecvPacketTemp_attr5",
      "erecvPacketTemp_attr6",
      "erecvPacketTemp_attr7",
      "erecvPacketTemp_attr8",
      "erecvPacketTemp_attr9",
      RN_DEST));

  Send (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Prov_rh2_2_eca (Ptr<Tuple> erecvPacketTemp)
{
  RAPIDNET_LOG_INFO ("Prov_rh2_2_eca triggered");

  Ptr<Tuple> result = erecvPacketTemp;

  result->Assign (Assignor::New ("PreLoc",
    FFirst::New (
      VarExpr::New ("erecvPacketTemp_attr9"))));

  result->Assign (Assignor::New ("Remainlist",
    FRemoveFirst::New (
      VarExpr::New ("erecvPacketTemp_attr9"))));

  result->Assign (Assignor::New ("PreRID",
    FFirst::New (
      VarExpr::New ("Remainlist"))));

  result = result->Project (
    RULEEXEC,
    strlist ("erecvPacketTemp_attr1",
      "erecvPacketTemp_attr6",
      "erecvPacketTemp_attr7",
      "erecvPacketTemp_attr8",
      "PreLoc",
      "PreRID"),
    strlist ("ruleExec_attr1",
      "ruleExec_attr2",
      "ruleExec_attr3",
      "ruleExec_attr4",
      "ruleExec_attr5",
      "ruleExec_attr6"));

  Insert (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Prov_rh2_3_eca (Ptr<Tuple> erecvPacketTemp)
{
  RAPIDNET_LOG_INFO ("Prov_rh2_3_eca triggered");

  Ptr<Tuple> result = erecvPacketTemp;

  result = result->Project (
    ERECVPACKET,
    strlist ("erecvPacketTemp_attr2",
      "erecvPacketTemp_attr3",
      "erecvPacketTemp_attr4",
      "erecvPacketTemp_attr5",
      "erecvPacketTemp_attr6",
      "erecvPacketTemp_attr1",
      "erecvPacketTemp_attr2"),
    strlist ("erecvPacket_attr1",
      "erecvPacket_attr2",
      "erecvPacket_attr3",
      "erecvPacket_attr4",
      "erecvPacket_attr5",
      "erecvPacket_attr6",
      RN_DEST));

  Send (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Prov_rh2_5_eca (Ptr<Tuple> erecvPacket)
{
  RAPIDNET_LOG_INFO ("Prov_rh2_5_eca triggered");

  Ptr<Tuple> result = erecvPacket;

  result = result->Project (
    RECVPACKET,
    strlist ("erecvPacket_attr1",
      "erecvPacket_attr2",
      "erecvPacket_attr3",
      "erecvPacket_attr4"),
    strlist ("recvPacket_attr1",
      "recvPacket_attr2",
      "recvPacket_attr3",
      "recvPacket_attr4"));

  Insert (result);
}

void
PktfwdNormDistHlistRmitmAuxtables::Prov_rh2_4_eca (Ptr<Tuple> erecvPacket)
{
  RAPIDNET_LOG_INFO ("Prov_rh2_4_eca triggered");

  Ptr<Tuple> result = erecvPacket;

  result->Assign (Assignor::New ("VID",
    FSha1::New (
      Operation::New (RN_PLUS,
        Operation::New (RN_PLUS,
          Operation::New (RN_PLUS,
            Operation::New (RN_PLUS,
              ValueExpr::New (StrValue::New ("recvPacket")),
              VarExpr::New ("erecvPacket_attr1")),
            VarExpr::New ("erecvPacket_attr2")),
          VarExpr::New ("erecvPacket_attr3")),
        VarExpr::New ("erecvPacket_attr4")))));

  result = result->Project (
    PROV,
    strlist ("erecvPacket_attr1",
      "VID",
      "erecvPacket_attr6",
      "erecvPacket_attr5"),
    strlist ("prov_attr1",
      "prov_attr2",
      "prov_attr3",
      "prov_attr4"));

  Insert (result);
}

