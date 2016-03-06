/* A RapidNet application. Generated by RapidNet compiler. */

#include "sim-hsls-periodic.h"
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
using namespace ns3::rapidnet::simhslsperiodic;

const string SimHslsPeriodic::ELSU = "eLSU";
const string SimHslsPeriodic::ELSUCHANGE = "eLSUChange";
const string SimHslsPeriodic::ELSUCOUNT = "eLSUCount";
const string SimHslsPeriodic::ELSULOC = "eLSULoc";
const string SimHslsPeriodic::ELINKADD = "eLinkAdd";
const string SimHslsPeriodic::ELINKCOUNT = "eLinkCount";
const string SimHslsPeriodic::LINK = "link";
const string SimHslsPeriodic::PERIODIC = "periodic";
const string SimHslsPeriodic::R11A_ECAPERIODIC = "r11A_ecaperiodic";
const string SimHslsPeriodic::R11B_ECAPERIODIC = "r11B_ecaperiodic";
const string SimHslsPeriodic::R12A_ECAPERIODIC = "r12A_ecaperiodic";
const string SimHslsPeriodic::R12B_ECAPERIODIC = "r12B_ecaperiodic";
const string SimHslsPeriodic::R13A_ECAPERIODIC = "r13A_ecaperiodic";
const string SimHslsPeriodic::R13B_ECAPERIODIC = "r13B_ecaperiodic";
const string SimHslsPeriodic::R14A_ECAPERIODIC = "r14A_ecaperiodic";
const string SimHslsPeriodic::R14B_ECAPERIODIC = "r14B_ecaperiodic";
const string SimHslsPeriodic::TLSU = "tLSU";
const string SimHslsPeriodic::TLINK = "tLink";

NS_LOG_COMPONENT_DEFINE ("SimHslsPeriodic");
NS_OBJECT_ENSURE_REGISTERED (SimHslsPeriodic);

TypeId
SimHslsPeriodic::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::rapidnet::simhslsperiodic::SimHslsPeriodic")
    .SetParent<Discovery> ()
    .AddConstructor<SimHslsPeriodic> ()
    ;
  return tid;
}

SimHslsPeriodic::SimHslsPeriodic()
{
  NS_LOG_FUNCTION_NOARGS ();
}

SimHslsPeriodic::~SimHslsPeriodic()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
SimHslsPeriodic::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  Discovery::DoDispose ();
}

void
SimHslsPeriodic::StartApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  Discovery::StartApplication ();
  m_event_r11a_ecaperiodic=
    Simulator::Schedule (Seconds (60 + (drand48 () * 1)), &SimHslsPeriodic::R11A_ecaperiodic, this);
  m_event_r11b_ecaperiodic=
    Simulator::Schedule (Seconds (59), &SimHslsPeriodic::R11B_ecaperiodic, this);
  m_event_r12a_ecaperiodic=
    Simulator::Schedule (Seconds (120 + (drand48 () * 1)), &SimHslsPeriodic::R12A_ecaperiodic, this);
  m_event_r12b_ecaperiodic=
    Simulator::Schedule (Seconds (119), &SimHslsPeriodic::R12B_ecaperiodic, this);
  m_event_r13a_ecaperiodic=
    Simulator::Schedule (Seconds (240 + (drand48 () * 1)), &SimHslsPeriodic::R13A_ecaperiodic, this);
  m_event_r13b_ecaperiodic=
    Simulator::Schedule (Seconds (239), &SimHslsPeriodic::R13B_ecaperiodic, this);
  m_event_r14a_ecaperiodic=
    Simulator::Schedule (Seconds (480 + (drand48 () * 1)), &SimHslsPeriodic::R14A_ecaperiodic, this);
  m_event_r14b_ecaperiodic=
    Simulator::Schedule (Seconds (479), &SimHslsPeriodic::R14B_ecaperiodic, this);
  RAPIDNET_LOG_INFO("SimHslsPeriodic Application Started");
}

void
SimHslsPeriodic::StopApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();

  Discovery::StopApplication ();
  Simulator::Cancel(m_event_r11a_ecaperiodic);
  Simulator::Cancel(m_event_r11b_ecaperiodic);
  Simulator::Cancel(m_event_r12a_ecaperiodic);
  Simulator::Cancel(m_event_r12b_ecaperiodic);
  Simulator::Cancel(m_event_r13a_ecaperiodic);
  Simulator::Cancel(m_event_r13b_ecaperiodic);
  Simulator::Cancel(m_event_r14a_ecaperiodic);
  Simulator::Cancel(m_event_r14b_ecaperiodic);
  RAPIDNET_LOG_INFO("SimHslsPeriodic Application Stopped");
}

void
SimHslsPeriodic::InitDatabase ()
{
  //Discovery::InitDatabase ();

  AddRelationWithKeys (LINK, attrdeflist (
    attrdef ("link_attr1", IPV4),
    attrdef ("link_attr2", IPV4)),
    Seconds (11));

  AddRelationWithKeys (TLSU, attrdeflist (
    attrdef ("tLSU_attr1", IPV4),
    attrdef ("tLSU_attr2", IPV4),
    attrdef ("tLSU_attr3", IPV4)),
    Seconds (65));

  AddRelationWithKeys (TLINK, attrdeflist (
    attrdef ("tLink_attr1", IPV4),
    attrdef ("tLink_attr2", IPV4)),
    Seconds (11));

}

void
SimHslsPeriodic::DemuxRecv (Ptr<Tuple> tuple)
{
  Discovery::DemuxRecv (tuple);

  if (IsInsertEvent (tuple, LINK))
    {
      R01Eca0Ins (tuple);
    }
  if (IsRefreshEvent (tuple, LINK))
    {
      R01Eca0Ref (tuple);
    }
  if (IsRecvEvent (tuple, ELINKADD))
    {
      R02_eca (tuple);
    }
  if (IsRecvEvent (tuple, ELINKCOUNT))
    {
      R03_eca (tuple);
    }
  if (IsRecvEvent (tuple, ELINKADD))
    {
      R04_eca (tuple);
    }
  if (IsRecvEvent (tuple, R11A_ECAPERIODIC))
    {
      R11A_eca (tuple);
    }
  if (IsRecvEvent (tuple, R11B_ECAPERIODIC))
    {
      R11B_eca (tuple);
    }
  if (IsRecvEvent (tuple, R12A_ECAPERIODIC))
    {
      R12A_eca (tuple);
    }
  if (IsRecvEvent (tuple, R12B_ECAPERIODIC))
    {
      R12B_eca (tuple);
    }
  if (IsRecvEvent (tuple, R13A_ECAPERIODIC))
    {
      R13A_eca (tuple);
    }
  if (IsRecvEvent (tuple, R13B_ECAPERIODIC))
    {
      R13B_eca (tuple);
    }
  if (IsRecvEvent (tuple, R14A_ECAPERIODIC))
    {
      R14A_eca (tuple);
    }
  if (IsRecvEvent (tuple, R14B_ECAPERIODIC))
    {
      R14B_eca (tuple);
    }
  if (IsRecvEvent (tuple, ELSU))
    {
      R21_eca (tuple);
    }
  if (IsRecvEvent (tuple, ELSULOC))
    {
      R22_eca (tuple);
    }
  if (IsRecvEvent (tuple, ELSUCOUNT))
    {
      R23_eca (tuple);
    }
  if (IsRecvEvent (tuple, ELSULOC))
    {
      R24_eca (tuple);
    }
  if (IsInsertEvent (tuple, TLSU))
    {
      R31Eca0Ins (tuple);
    }
  if (IsRefreshEvent (tuple, TLSU))
    {
      R31Eca0Ref (tuple);
    }
  if (IsRecvEvent (tuple, ELSUCHANGE))
    {
      R32_eca (tuple);
    }
}

void
SimHslsPeriodic::R01Eca0Ins (Ptr<Tuple> link)
{
  RAPIDNET_LOG_INFO ("R01Eca0Ins triggered");

  Ptr<Tuple> result = link;

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("link_attr1"),
      VarExpr::New ("link_attr2"))));

  result = result->Project (
    ELINKADD,
    strlist ("link_attr1",
      "link_attr2",
      "link_attr3"),
    strlist ("eLinkAdd_attr1",
      "eLinkAdd_attr2",
      "eLinkAdd_attr3"));

  SendLocal (result);
}

void
SimHslsPeriodic::R01Eca0Ref (Ptr<Tuple> link)
{
  RAPIDNET_LOG_INFO ("R01Eca0Ref triggered");

  Ptr<Tuple> result = link;

  result = result->Select (Selector::New (
    Operation::New (RN_NEQ,
      VarExpr::New ("link_attr1"),
      VarExpr::New ("link_attr2"))));

  result = result->Project (
    ELINKADD,
    strlist ("link_attr1",
      "link_attr2",
      "link_attr3"),
    strlist ("eLinkAdd_attr1",
      "eLinkAdd_attr2",
      "eLinkAdd_attr3"));

  SendLocal (result);
}

void
SimHslsPeriodic::R02_eca (Ptr<Tuple> eLinkAdd)
{
  RAPIDNET_LOG_INFO ("R02_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (TLINK)->Join (
    eLinkAdd,
    strlist ("tLink_attr2", "tLink_attr1"),
    strlist ("eLinkAdd_attr2", "eLinkAdd_attr1"));

  result = AggWrapCount::New ()->Compute (result, eLinkAdd);

  result = result->Project (
    ELINKCOUNT,
    strlist ("eLinkAdd_attr1",
      "eLinkAdd_attr2",
      "eLinkAdd_attr3",
      "count"),
    strlist ("eLinkCount_attr1",
      "eLinkCount_attr2",
      "eLinkCount_attr3",
      "eLinkCount_attr4"));

  SendLocal (result);
}

void
SimHslsPeriodic::R03_eca (Ptr<Tuple> eLinkCount)
{
  RAPIDNET_LOG_INFO ("R03_eca triggered");

  Ptr<Tuple> result = eLinkCount;

  result->Assign (Assignor::New ("SeqNum",
    ValueExpr::New (Int32Value::New (0))));

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("eLinkCount_attr4"),
      ValueExpr::New (Int32Value::New (0)))));

  result = result->Project (
    TLINK,
    strlist ("eLinkCount_attr1",
      "eLinkCount_attr2",
      "eLinkCount_attr3",
      "SeqNum"),
    strlist ("tLink_attr1",
      "tLink_attr2",
      "tLink_attr3",
      "tLink_attr4"));

  Insert (result);
}

void
SimHslsPeriodic::R04_eca (Ptr<Tuple> eLinkAdd)
{
  RAPIDNET_LOG_INFO ("R04_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (TLINK)->Join (
    eLinkAdd,
    strlist ("tLink_attr2", "tLink_attr1"),
    strlist ("eLinkAdd_attr2", "eLinkAdd_attr1"));

  result = result->Project (
    TLINK,
    strlist ("eLinkAdd_attr1",
      "eLinkAdd_attr2",
      "eLinkAdd_attr3",
      "tLink_attr4"),
    strlist ("tLink_attr1",
      "tLink_attr2",
      "tLink_attr3",
      "tLink_attr4"));

  Insert (result);
}

void
SimHslsPeriodic::R11A_ecaperiodic ()
{
  RAPIDNET_LOG_INFO ("R11A_ecaperiodic triggered");

  SendLocal (tuple (R11A_ECAPERIODIC, attrlist (
    attr ("r11A_ecaperiodic_attr1", Ipv4Value, GetAddress ()),
    attr ("r11A_ecaperiodic_attr2", Int32Value, rand ()))));

  m_event_r11a_ecaperiodic = Simulator::Schedule (Seconds(120),
    &SimHslsPeriodic::R11A_ecaperiodic, this);
}

void
SimHslsPeriodic::R11A_eca (Ptr<Tuple> r11A_ecaperiodic)
{
  RAPIDNET_LOG_INFO ("R11A_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (TLINK)->Join (
    r11A_ecaperiodic,
    strlist ("tLink_attr1"),
    strlist ("r11A_ecaperiodic_attr1"));

  result->Assign (Assignor::New ("$1",
    VarExpr::New ("r11A_ecaperiodic_attr1")));

  result->Assign (Assignor::New ("TTL",
    ValueExpr::New (Int32Value::New (2))));

  result = result->Project (
    ELSU,
    strlist ("r11A_ecaperiodic_attr1",
      "$1",
      "tLink_attr2",
      "tLink_attr3",
      "tLink_attr4",
      "TTL"),
    strlist ("eLSU_attr1",
      "eLSU_attr2",
      "eLSU_attr3",
      "eLSU_attr4",
      "eLSU_attr5",
      "eLSU_attr6"));

  SendLocal (result);
}

void
SimHslsPeriodic::R11B_ecaperiodic ()
{
  RAPIDNET_LOG_INFO ("R11B_ecaperiodic triggered");

  SendLocal (tuple (R11B_ECAPERIODIC, attrlist (
    attr ("r11B_ecaperiodic_attr1", Ipv4Value, GetAddress ()),
    attr ("r11B_ecaperiodic_attr2", Int32Value, rand ()))));

  m_event_r11b_ecaperiodic = Simulator::Schedule (Seconds(120),
    &SimHslsPeriodic::R11B_ecaperiodic, this);
}

void
SimHslsPeriodic::R11B_eca (Ptr<Tuple> r11B_ecaperiodic)
{
  RAPIDNET_LOG_INFO ("R11B_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (TLINK)->Join (
    r11B_ecaperiodic,
    strlist ("tLink_attr1"),
    strlist ("r11B_ecaperiodic_attr1"));

  result->Assign (Assignor::New ("SeqNumU",
    Operation::New (RN_PLUS,
      VarExpr::New ("tLink_attr4"),
      ValueExpr::New (Int32Value::New (1)))));

  result = result->Project (
    TLINK,
    strlist ("r11B_ecaperiodic_attr1",
      "tLink_attr2",
      "tLink_attr3",
      "SeqNumU"),
    strlist ("tLink_attr1",
      "tLink_attr2",
      "tLink_attr3",
      "tLink_attr4"));

  Insert (result);
}

void
SimHslsPeriodic::R12A_ecaperiodic ()
{
  RAPIDNET_LOG_INFO ("R12A_ecaperiodic triggered");

  SendLocal (tuple (R12A_ECAPERIODIC, attrlist (
    attr ("r12A_ecaperiodic_attr1", Ipv4Value, GetAddress ()),
    attr ("r12A_ecaperiodic_attr2", Int32Value, rand ()))));

  m_event_r12a_ecaperiodic = Simulator::Schedule (Seconds(240),
    &SimHslsPeriodic::R12A_ecaperiodic, this);
}

void
SimHslsPeriodic::R12A_eca (Ptr<Tuple> r12A_ecaperiodic)
{
  RAPIDNET_LOG_INFO ("R12A_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (TLINK)->Join (
    r12A_ecaperiodic,
    strlist ("tLink_attr1"),
    strlist ("r12A_ecaperiodic_attr1"));

  result->Assign (Assignor::New ("$1",
    VarExpr::New ("r12A_ecaperiodic_attr1")));

  result->Assign (Assignor::New ("TTL",
    ValueExpr::New (Int32Value::New (4))));

  result = result->Project (
    ELSU,
    strlist ("r12A_ecaperiodic_attr1",
      "$1",
      "tLink_attr2",
      "tLink_attr3",
      "tLink_attr4",
      "TTL"),
    strlist ("eLSU_attr1",
      "eLSU_attr2",
      "eLSU_attr3",
      "eLSU_attr4",
      "eLSU_attr5",
      "eLSU_attr6"));

  SendLocal (result);
}

void
SimHslsPeriodic::R12B_ecaperiodic ()
{
  RAPIDNET_LOG_INFO ("R12B_ecaperiodic triggered");

  SendLocal (tuple (R12B_ECAPERIODIC, attrlist (
    attr ("r12B_ecaperiodic_attr1", Ipv4Value, GetAddress ()),
    attr ("r12B_ecaperiodic_attr2", Int32Value, rand ()))));

  m_event_r12b_ecaperiodic = Simulator::Schedule (Seconds(240),
    &SimHslsPeriodic::R12B_ecaperiodic, this);
}

void
SimHslsPeriodic::R12B_eca (Ptr<Tuple> r12B_ecaperiodic)
{
  RAPIDNET_LOG_INFO ("R12B_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (TLINK)->Join (
    r12B_ecaperiodic,
    strlist ("tLink_attr1"),
    strlist ("r12B_ecaperiodic_attr1"));

  result->Assign (Assignor::New ("SeqNumU",
    Operation::New (RN_PLUS,
      VarExpr::New ("tLink_attr4"),
      ValueExpr::New (Int32Value::New (1)))));

  result = result->Project (
    TLINK,
    strlist ("r12B_ecaperiodic_attr1",
      "tLink_attr2",
      "tLink_attr3",
      "SeqNumU"),
    strlist ("tLink_attr1",
      "tLink_attr2",
      "tLink_attr3",
      "tLink_attr4"));

  Insert (result);
}

void
SimHslsPeriodic::R13A_ecaperiodic ()
{
  RAPIDNET_LOG_INFO ("R13A_ecaperiodic triggered");

  SendLocal (tuple (R13A_ECAPERIODIC, attrlist (
    attr ("r13A_ecaperiodic_attr1", Ipv4Value, GetAddress ()),
    attr ("r13A_ecaperiodic_attr2", Int32Value, rand ()))));

  m_event_r13a_ecaperiodic = Simulator::Schedule (Seconds(480),
    &SimHslsPeriodic::R13A_ecaperiodic, this);
}

void
SimHslsPeriodic::R13A_eca (Ptr<Tuple> r13A_ecaperiodic)
{
  RAPIDNET_LOG_INFO ("R13A_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (TLINK)->Join (
    r13A_ecaperiodic,
    strlist ("tLink_attr1"),
    strlist ("r13A_ecaperiodic_attr1"));

  result->Assign (Assignor::New ("$1",
    VarExpr::New ("r13A_ecaperiodic_attr1")));

  result->Assign (Assignor::New ("TTL",
    ValueExpr::New (Int32Value::New (8))));

  result = result->Project (
    ELSU,
    strlist ("r13A_ecaperiodic_attr1",
      "$1",
      "tLink_attr2",
      "tLink_attr3",
      "tLink_attr4",
      "TTL"),
    strlist ("eLSU_attr1",
      "eLSU_attr2",
      "eLSU_attr3",
      "eLSU_attr4",
      "eLSU_attr5",
      "eLSU_attr6"));

  SendLocal (result);
}

void
SimHslsPeriodic::R13B_ecaperiodic ()
{
  RAPIDNET_LOG_INFO ("R13B_ecaperiodic triggered");

  SendLocal (tuple (R13B_ECAPERIODIC, attrlist (
    attr ("r13B_ecaperiodic_attr1", Ipv4Value, GetAddress ()),
    attr ("r13B_ecaperiodic_attr2", Int32Value, rand ()))));

  m_event_r13b_ecaperiodic = Simulator::Schedule (Seconds(480),
    &SimHslsPeriodic::R13B_ecaperiodic, this);
}

void
SimHslsPeriodic::R13B_eca (Ptr<Tuple> r13B_ecaperiodic)
{
  RAPIDNET_LOG_INFO ("R13B_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (TLINK)->Join (
    r13B_ecaperiodic,
    strlist ("tLink_attr1"),
    strlist ("r13B_ecaperiodic_attr1"));

  result->Assign (Assignor::New ("SeqNumU",
    Operation::New (RN_PLUS,
      VarExpr::New ("tLink_attr4"),
      ValueExpr::New (Int32Value::New (1)))));

  result = result->Project (
    TLINK,
    strlist ("r13B_ecaperiodic_attr1",
      "tLink_attr2",
      "tLink_attr3",
      "SeqNumU"),
    strlist ("tLink_attr1",
      "tLink_attr2",
      "tLink_attr3",
      "tLink_attr4"));

  Insert (result);
}

void
SimHslsPeriodic::R14A_ecaperiodic ()
{
  RAPIDNET_LOG_INFO ("R14A_ecaperiodic triggered");

  SendLocal (tuple (R14A_ECAPERIODIC, attrlist (
    attr ("r14A_ecaperiodic_attr1", Ipv4Value, GetAddress ()),
    attr ("r14A_ecaperiodic_attr2", Int32Value, rand ()))));

  m_event_r14a_ecaperiodic = Simulator::Schedule (Seconds(960),
    &SimHslsPeriodic::R14A_ecaperiodic, this);
}

void
SimHslsPeriodic::R14A_eca (Ptr<Tuple> r14A_ecaperiodic)
{
  RAPIDNET_LOG_INFO ("R14A_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (TLINK)->Join (
    r14A_ecaperiodic,
    strlist ("tLink_attr1"),
    strlist ("r14A_ecaperiodic_attr1"));

  result->Assign (Assignor::New ("$1",
    VarExpr::New ("r14A_ecaperiodic_attr1")));

  result->Assign (Assignor::New ("TTL",
    ValueExpr::New (Int32Value::New (16))));

  result = result->Project (
    ELSU,
    strlist ("r14A_ecaperiodic_attr1",
      "$1",
      "tLink_attr2",
      "tLink_attr3",
      "tLink_attr4",
      "TTL"),
    strlist ("eLSU_attr1",
      "eLSU_attr2",
      "eLSU_attr3",
      "eLSU_attr4",
      "eLSU_attr5",
      "eLSU_attr6"));

  SendLocal (result);
}

void
SimHslsPeriodic::R14B_ecaperiodic ()
{
  RAPIDNET_LOG_INFO ("R14B_ecaperiodic triggered");

  SendLocal (tuple (R14B_ECAPERIODIC, attrlist (
    attr ("r14B_ecaperiodic_attr1", Ipv4Value, GetAddress ()),
    attr ("r14B_ecaperiodic_attr2", Int32Value, rand ()))));

  m_event_r14b_ecaperiodic = Simulator::Schedule (Seconds(960),
    &SimHslsPeriodic::R14B_ecaperiodic, this);
}

void
SimHslsPeriodic::R14B_eca (Ptr<Tuple> r14B_ecaperiodic)
{
  RAPIDNET_LOG_INFO ("R14B_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (TLINK)->Join (
    r14B_ecaperiodic,
    strlist ("tLink_attr1"),
    strlist ("r14B_ecaperiodic_attr1"));

  result->Assign (Assignor::New ("SeqNumU",
    Operation::New (RN_PLUS,
      VarExpr::New ("tLink_attr4"),
      ValueExpr::New (Int32Value::New (1)))));

  result = result->Project (
    TLINK,
    strlist ("r14B_ecaperiodic_attr1",
      "tLink_attr2",
      "tLink_attr3",
      "SeqNumU"),
    strlist ("tLink_attr1",
      "tLink_attr2",
      "tLink_attr3",
      "tLink_attr4"));

  Insert (result);
}

void
SimHslsPeriodic::R21_eca (Ptr<Tuple> eLSU)
{
  RAPIDNET_LOG_INFO ("R21_eca triggered");

  Ptr<Tuple> result = eLSU;

  result->Assign (Assignor::New ("Local",
    LOCAL_ADDRESS));

  result = result->Project (
    ELSULOC,
    strlist ("Local",
      "eLSU_attr2",
      "eLSU_attr3",
      "eLSU_attr4",
      "eLSU_attr5",
      "eLSU_attr6",
      "Local"),
    strlist ("eLSULoc_attr1",
      "eLSULoc_attr2",
      "eLSULoc_attr3",
      "eLSULoc_attr4",
      "eLSULoc_attr5",
      "eLSULoc_attr6",
      RN_DEST));

  Send (result);
}

void
SimHslsPeriodic::R22_eca (Ptr<Tuple> eLSULoc)
{
  RAPIDNET_LOG_INFO ("R22_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (TLSU)->Join (
    eLSULoc,
    strlist ("tLSU_attr3", "tLSU_attr2", "tLSU_attr1"),
    strlist ("eLSULoc_attr3", "eLSULoc_attr2", "eLSULoc_attr1"));

  result = AggWrapCount::New ()->Compute (result, eLSULoc);

  result = result->Project (
    ELSUCOUNT,
    strlist ("eLSULoc_attr1",
      "eLSULoc_attr2",
      "eLSULoc_attr3",
      "eLSULoc_attr4",
      "eLSULoc_attr5",
      "eLSULoc_attr6",
      "count"),
    strlist ("eLSUCount_attr1",
      "eLSUCount_attr2",
      "eLSUCount_attr3",
      "eLSUCount_attr4",
      "eLSUCount_attr5",
      "eLSUCount_attr6",
      "eLSUCount_attr7"));

  SendLocal (result);
}

void
SimHslsPeriodic::R23_eca (Ptr<Tuple> eLSUCount)
{
  RAPIDNET_LOG_INFO ("R23_eca triggered");

  Ptr<Tuple> result = eLSUCount;

  result = result->Select (Selector::New (
    Operation::New (RN_EQ,
      VarExpr::New ("eLSUCount_attr7"),
      ValueExpr::New (Int32Value::New (0)))));

  result = result->Project (
    TLSU,
    strlist ("eLSUCount_attr1",
      "eLSUCount_attr2",
      "eLSUCount_attr3",
      "eLSUCount_attr4",
      "eLSUCount_attr5",
      "eLSUCount_attr6"),
    strlist ("tLSU_attr1",
      "tLSU_attr2",
      "tLSU_attr3",
      "tLSU_attr4",
      "tLSU_attr5",
      "tLSU_attr6"));

  Insert (result);
}

void
SimHslsPeriodic::R24_eca (Ptr<Tuple> eLSULoc)
{
  RAPIDNET_LOG_INFO ("R24_eca triggered");

  Ptr<RelationBase> result;

  result = GetRelation (TLSU)->Join (
    eLSULoc,
    strlist ("tLSU_attr3", "tLSU_attr2", "tLSU_attr1"),
    strlist ("eLSULoc_attr3", "eLSULoc_attr2", "eLSULoc_attr1"));

  result = result->Select (Selector::New (
    Operation::New (RN_GT,
      VarExpr::New ("eLSULoc_attr5"),
      VarExpr::New ("tLSU_attr5"))));

  result = result->Project (
    TLSU,
    strlist ("eLSULoc_attr1",
      "eLSULoc_attr2",
      "eLSULoc_attr3",
      "eLSULoc_attr4",
      "eLSULoc_attr5",
      "eLSULoc_attr6"),
    strlist ("tLSU_attr1",
      "tLSU_attr2",
      "tLSU_attr3",
      "tLSU_attr4",
      "tLSU_attr5",
      "tLSU_attr6"));

  Insert (result);
}

void
SimHslsPeriodic::R31Eca0Ins (Ptr<Tuple> tLSU)
{
  RAPIDNET_LOG_INFO ("R31Eca0Ins triggered");

  Ptr<Tuple> result = tLSU;

  result = result->Project (
    ELSUCHANGE,
    strlist ("tLSU_attr1",
      "tLSU_attr2",
      "tLSU_attr3",
      "tLSU_attr4",
      "tLSU_attr5",
      "tLSU_attr6"),
    strlist ("eLSUChange_attr1",
      "eLSUChange_attr2",
      "eLSUChange_attr3",
      "eLSUChange_attr4",
      "eLSUChange_attr5",
      "eLSUChange_attr6"));

  SendLocal (result);
}

void
SimHslsPeriodic::R31Eca0Ref (Ptr<Tuple> tLSU)
{
  RAPIDNET_LOG_INFO ("R31Eca0Ref triggered");

  Ptr<Tuple> result = tLSU;

  result = result->Project (
    ELSUCHANGE,
    strlist ("tLSU_attr1",
      "tLSU_attr2",
      "tLSU_attr3",
      "tLSU_attr4",
      "tLSU_attr5",
      "tLSU_attr6"),
    strlist ("eLSUChange_attr1",
      "eLSUChange_attr2",
      "eLSUChange_attr3",
      "eLSUChange_attr4",
      "eLSUChange_attr5",
      "eLSUChange_attr6"));

  SendLocal (result);
}

void
SimHslsPeriodic::R32_eca (Ptr<Tuple> eLSUChange)
{
  RAPIDNET_LOG_INFO ("R32_eca triggered");

  Ptr<Tuple> result = eLSUChange;

  result->Assign (Assignor::New ("TTLU",
    Operation::New (RN_MINUS,
      VarExpr::New ("eLSUChange_attr6"),
      ValueExpr::New (Int32Value::New (1)))));

  result->Assign (Assignor::New ("Broadcast",
    BROADCAST_ADDRESS));

  result = result->Select (Selector::New (
    Operation::New (RN_GT,
      VarExpr::New ("eLSUChange_attr6"),
      ValueExpr::New (Int32Value::New (0)))));

  result = result->Project (
    ELSU,
    strlist ("Broadcast",
      "eLSUChange_attr2",
      "eLSUChange_attr3",
      "eLSUChange_attr4",
      "eLSUChange_attr5",
      "TTLU",
      "Broadcast"),
    strlist ("eLSU_attr1",
      "eLSU_attr2",
      "eLSU_attr3",
      "eLSU_attr4",
      "eLSU_attr5",
      "eLSU_attr6",
      RN_DEST));

  Send (result);
}

