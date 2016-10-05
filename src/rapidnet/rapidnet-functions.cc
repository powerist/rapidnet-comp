/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Pennsylvania
 *
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
 *
 */
#include "rapidnet-functions.h"
#include <cstdlib>
#include "ns3/simulator.h"
#include "ns3/rapidnet-types.h"
#include "rapidnet-application-base.h"
#include "rapidnet-utils.h"
#include "expression.h"

using namespace ns3;
using namespace rapidnet;

Ptr<Value>
FAppend::Eval (Ptr<Tuple> tuple)
{
  list<Ptr<Value> > result;
  result.push_back (m_source->Eval (tuple));
  return ListValue::New (result);
}

Ptr<FunctionExpr>
FAppend::New (Ptr<Expression> source)
{
  Ptr<FAppend> retval = Create<FAppend> ();
  retval->m_source = source;
  return retval;
}

Ptr<Value>
FConcat::Eval (Ptr<Tuple> tuple)
{
  list<Ptr<Value> > result;
  Ptr<ListValue> headVal = DynamicCast<ListValue, Value> (
                                                          m_head->Eval (tuple));
  Ptr<ListValue> tailVal = DynamicCast<ListValue, Value> (
                                                          m_tail->Eval (tuple));
  for (rn_list_iterator it = headVal->Begin(); it != headVal->End (); ++it)
    {
      result.push_back (*it);
    }
  for (rn_list_iterator it = tailVal->Begin(); it != tailVal->End (); ++it)
    {
      result.push_back (*it);
    }
  return ListValue::New (result);
}

Ptr<FunctionExpr>
FConcat::New (Ptr<Expression> head, Ptr<Expression> tail)
{
  Ptr<FConcat> retval = Create<FConcat> ();
  retval->m_head = head;
  retval->m_tail = tail;
  return retval;
}

Ptr<Value>
FMember::Eval (Ptr<Tuple> tuple)
{
  Ptr<ListValue> lstVal = DynamicCast<ListValue, Value> (m_lst->Eval (tuple));
  // Seems P2 does not support "true" and "false" as keywords. So use int32 for now
  return Int32Value::New (lstVal->Contains (m_item->Eval (tuple)));
}

Ptr<FunctionExpr>
FMember::New (Ptr<Expression> lst, Ptr<Expression> item)
{
  Ptr<FMember> retval = Create<FMember> ();
  retval->m_lst = lst;
  retval->m_item = item;
  return retval;
}

Ptr<Value>
FNow::Eval (Ptr<Tuple> tuple)
{
  return RealValue::New (Simulator::Now ().GetSeconds ());
}

Ptr<FunctionExpr>
FNow::New ()
{
  return Create<FNow> ();
}

Ptr<Value>
FDiffTime::Eval (Ptr<Tuple> tuple)
{
  return Operation::New (RN_MINUS, m_time2, m_time1)->Eval (tuple);
}

Ptr<FunctionExpr>
FDiffTime::New (Ptr<Expression> time2, Ptr<Expression> time1)
{
  Ptr<FDiffTime> retval = Create<FDiffTime> ();
  retval->m_time2 = time2;
  retval->m_time1 = time1;
  return retval;
}

Ptr<Value>
FHslsTtl::Eval (Ptr<Tuple> tuple)
{
  Ptr<Value> now = FNow::New ()->Eval (tuple);
  Ptr<Value> normTimeDiff = Operation::New (RN_PLUS,
                                            Operation::New (RN_DIVIDE, ValueExpr::New (now), m_periodAttrName),
                                            ValueExpr::New (RealValue::New (1.0)))->Eval (tuple);
  int32_t n = int32_t (rn_real (normTimeDiff));
  int32_t ttl = n % 2 == 1 ? 2 : (n % 4 == 2 ? 4 : (n % 8 == 4 ? 8 : 16));
  return Int32Value::New (ttl);
}

Ptr<FunctionExpr>
FHslsTtl::New (Ptr<Expression> timeAttrName, Ptr<Expression> periodAttrName)
{
  Ptr<FHslsTtl> retval = Create<FHslsTtl> ();
  retval->m_timeAttrName = timeAttrName;
  retval->m_periodAttrName = periodAttrName;
  return retval;
}

Ptr<Value>
FSize::Eval (Ptr<Tuple> tuple)
{
  Ptr<Value> lstVal = m_listAttrName->Eval (tuple);
  NS_ASSERT_MSG (V_InstanceOf (lstVal, ListValue),
                 "f_last can only be applied to a list type attribute.");

  return Int32Value::New ((DynamicCast<ListValue, Value> (lstVal))->Size ());
}

Ptr<FunctionExpr>
FSize::New (Ptr<Expression> listAttrName)
{
  Ptr<FSize> retval = Create<FSize> ();
  retval->m_listAttrName = listAttrName;
  return retval;
}

Ptr<Value>
FLast::Eval (Ptr<Tuple> tuple)
{
  Ptr<Value> lstVal = m_listAttrName->Eval (tuple);
  NS_ASSERT_MSG (V_InstanceOf (lstVal, ListValue),
                 "f_last can only be applied to a list type attribute.");

  list<Ptr<Value> > lst = rn_list (lstVal);

  if (lst.end () == lst.begin ())
    {
      return Ptr<Value> (NULL);
    }

  rn_list_iterator it = lst.end ();
  return (*--it)->Clone ();
}

Ptr<FunctionExpr>
FLast::New (Ptr<Expression> listAttrName)
{
  Ptr<FLast> retval = Create<FLast> ();
  retval->m_listAttrName = listAttrName;
  return retval;
}

Ptr<Value>
FRemoveLast::Eval (Ptr<Tuple> tuple)
{
  Ptr<Value> lstVal = m_listAttrName->Eval (tuple);
  NS_ASSERT_MSG (V_InstanceOf (lstVal, ListValue),
                 "f_removeLast can only be applied to a list type attribute.");

  list<Ptr<Value> > lst = rn_list (lstVal);

  if (lst.end () == lst.begin ())
    {
      return Ptr<Value> (NULL);
    }

  rn_list_iterator it = lst.end ();
  --it; // move one step back
  list<Ptr<Value> > newlst;
  for (rn_list_iterator jt = lst.begin (); jt != it; jt++)
    {
      newlst.push_back ((*jt)->Clone());
    }
  return ListValue::New (newlst);
}

Ptr<FunctionExpr>
FRemoveLast::New (Ptr<Expression> listAttrName)
{
  Ptr<FRemoveLast> retval = Create<FRemoveLast> ();
  retval->m_listAttrName = listAttrName;
  return retval;
}

Ptr<Value>
FTypeOf::Eval (Ptr<Tuple> tuple)
{
  return StrValue::New (m_arg->Eval (tuple)->GetTypeName ());
}

Ptr<FunctionExpr>
FTypeOf::New (Ptr<Expression> arg)
{
  Ptr<FTypeOf> retval = Create<FTypeOf> ();
  retval->m_arg = arg;
  return retval;
}

Ptr<Value>
FRand::Eval (Ptr<Tuple> tuple)
{
  stringstream ss;
  ss << rand ();
  return StrValue::New (ss.str ());
}

Ptr<FunctionExpr>
FRand::New ()
{
  return Create<FRand> ();
}

Ptr<Value>
FSha1::Eval (Ptr<Tuple> tuple)
{
  string str = m_arg->Eval (tuple)->ToString ();
  return IdValue::New (GetSHA1Digest (str), 16);
}

Ptr<FunctionExpr>
FSha1::New (Ptr<Expression> arg)
{
  Ptr<FSha1> retval = Create<FSha1> ();
  retval->m_arg = arg;
  return retval;
}

Ptr<Value>
FSvCreate::Eval (Ptr<Tuple> tuple)
{
  return SvValue::New();
}

Ptr<FunctionExpr>
FSvCreate::New ()
{
  Ptr<FSvCreate> retval = Create<FSvCreate> ();
  return retval;
}

Ptr<Value>
FSvIn::Eval(Ptr<Tuple> tuple)
{
  Ptr<Value> svVPtr = m_svExpr->Eval (tuple);
  Ptr<Value> strVPtr = m_strExpr->Eval (tuple);

  int32_t result = SvValue::HashIn(svVPtr, strVPtr) ? 1 : 0;

  return Int32Value::New(result);
}

Ptr<FunctionExpr>
FSvIn::New (Ptr<Expression> svExpr,
            Ptr<Expression> strExpr)
{
  Ptr<FSvIn> retval = Create<FSvIn>();
  retval->m_svExpr = svExpr;
  retval->m_strExpr = strExpr;
  return retval;
}

Ptr<Value>
FSvAndNot::Eval(Ptr<Tuple> tuple)
{
  Ptr<Value> svVPtr_1 = m_svExpr_1->Eval (tuple);
  Ptr<Value> svVPtr_2 = m_svExpr_2->Eval (tuple);

  return SvValue::AndNot(svVPtr_1, svVPtr_2);
}

Ptr<FunctionExpr>
FSvAndNot::New (Ptr<Expression> svExpr_1,
                Ptr<Expression> svExpr_2)
{
  Ptr<FSvAndNot> retval = Create<FSvAndNot>();
  retval->m_svExpr_1 = svExpr_1;
  retval->m_svExpr_2 = svExpr_2;
  return retval;
}

Ptr<Value>
FSvAppend::Eval(Ptr<Tuple> tuple)
{
  Ptr<Value> svVPtr = m_svExpr->Eval (tuple);
  Ptr<Value> strVPtr = m_strExpr->Eval (tuple);

  return  SvValue::HashInsert(svVPtr, strVPtr);
}

Ptr<FunctionExpr>
FSvAppend::New (Ptr<Expression> svExpr,
                Ptr<Expression> strExpr)
{
  Ptr<FSvAppend> retval = Create<FSvAppend>();
  retval->m_svExpr = svExpr;
  retval->m_strExpr = strExpr;
  return retval;
}

Ptr<Value>
FSvRemove::Eval(Ptr<Tuple> tuple)
{
  Ptr<Value> svVPtr = m_svExpr->Eval (tuple);
  Ptr<Value> strVPtr = m_strExpr->Eval (tuple);

  return SvValue::HashRemove(svVPtr, strVPtr);
}

Ptr<FunctionExpr>
FSvRemove::New (Ptr<Expression> svExpr,
                Ptr<Expression> strExpr)
{
  Ptr<FSvRemove> retval = Create<FSvRemove>();
  retval->m_svExpr = svExpr;
  retval->m_strExpr = strExpr;
  return retval;
}

Ptr<Value>
FJoin::Eval (Ptr<Tuple> tuple)
{
  list<Ptr<Value> > result;
  Ptr<Value> val1 = m_val1->Eval (tuple);
  Ptr<Value> val2 = m_val2->Eval (tuple);

  if (val1->GetType() != LIST)
    {
      val1 = ListValue::New (MakeList<Ptr<Value> >(val1));
    }
  if (val2->GetType() != LIST)
    {
      val2 = ListValue::New (MakeList<Ptr<Value> >(val2));
    }

  return FConcat::New (ValueExpr::New (val1), ValueExpr::New(val2))->Eval (
                                                                           tuple);
}

Ptr<FunctionExpr>
FJoin::New (Ptr<Expression> val1, Ptr<Expression> val2)
{
  Ptr<FJoin> retval = Create<FJoin> ();
  retval->m_val1 = val1;
  retval->m_val2 = val2;
  return retval;
}

Ptr<Value>
FGet::Eval (Ptr<Tuple> tuple)
{
  Ptr<Value> lstVal = m_listAttrName->Eval (tuple);
  NS_ASSERT_MSG (V_InstanceOf (lstVal, ListValue),
                 "f_last can only be applied to a list type attribute.");

  list<Ptr<Value> > lst = rn_list (lstVal);
  int32_t loc = rn_int32 (m_loc->Eval (tuple));

  rn_list_iterator it = lst.begin ();
  for(int32_t i = 1; i < loc && it != lst.end (); ++i)
    {
      ++it;
    }

  if (it == lst.end ())
    {
      return Ptr<Value> (NULL);
    }

  return (*it)->Clone ();
}

Ptr<FunctionExpr>
FGet::New (Ptr<Expression> listAttrName, Ptr<Expression> loc)
{
  Ptr<FGet> retval = Create<FGet> ();
  retval->m_listAttrName = listAttrName;
  retval->m_loc = loc;
  return retval;
}

Ptr<Value> 
FPopFront::Eval (Ptr<Tuple> tuple)
{
  Ptr<ListValue> listVal = DynamicCast<ListValue, Value> (m_list->Eval (tuple));

  if (listVal->Size() == 0)
    return Ptr<Value> (NULL);
  list<Ptr<Value> > lst(listVal->GetListValue());  
lst.pop_front();
  
return ListValue::New(lst); 


//rn_list_iterator it = lst.begin ();
//return (*it)->Clone ();
}

Ptr<FunctionExpr>
FPopFront::New (Ptr<Expression> list)
{
  Ptr<FPopFront> retVal = Create<FPopFront> ();
  retVal->m_list = list;
  return retVal;
}


Ptr<Value>
FFirst::Eval (Ptr<Tuple> tuple)
{
  Ptr<Value> lstVal = m_listAttrName->Eval (tuple);
  NS_ASSERT_MSG (V_InstanceOf (lstVal, ListValue),
                 "f_last can only be applied to a list type attribute.");

  list<Ptr<Value> > lst = rn_list (lstVal);

  if (lst.end () == lst.begin ())
    {
      return Ptr<Value> (NULL);
    }

  rn_list_iterator it = lst.begin ();
  return (*it++)->Clone ();
}

Ptr<FunctionExpr>
FFirst::New (Ptr<Expression> listAttrName)
{
  Ptr<FFirst> retval = Create<FFirst> ();
  retval->m_listAttrName = listAttrName;
  return retval;
}

Ptr<Value>
FDupList::Eval (Ptr<Tuple> tuple)
{
  Ptr<ListValue> listVal = DynamicCast<ListValue, Value> (m_list->Eval (tuple));
  if (listVal->Size() == 0)
    return Ptr<Value> (NULL);
  std::list<Ptr<Value> > newlist = listVal->GetListValue();
  return ListValue::New(newlist);
}

Ptr<FunctionExpr>
FDupList::New (Ptr<Expression> list)
{
  Ptr<FDupList> retVal = Create<FDupList> ();
  retVal->m_list = list;
  return retVal;
}

Ptr<Value>
FInitList::Eval (Ptr<Tuple> tuple)
{
  list<Ptr<Value> > empty;
  return ListValue::New(empty);
}

Ptr<FunctionExpr>
FInitList::New ()
{
  Ptr<FInitList> retVal = Create<FInitList> ();
  return retVal;
}


Ptr<Value>
FDisjoint::Eval (Ptr<Tuple> tuple)
{
  int disjoint = 0; 
  Ptr<ListValue> lstVal1 = DynamicCast<ListValue, Value> (m_lst1->Eval (tuple));
  Ptr<ListValue> lstVal2 = DynamicCast<ListValue, Value> (m_lst2->Eval (tuple));
  list<Ptr<Value> > set1 = lstVal1->GetListValue(); 
  list<Ptr<Value> > set2 = lstVal2->GetListValue(); 
 
  if(set1.empty() || set2.empty()) return Int32Value::New (1);

  list<Ptr<Value> >::const_iterator it1 = set1.begin(); 
  list<Ptr<Value> >::const_iterator it1End = set1.end();
  list<Ptr<Value> >::const_iterator it2 = set2.begin(); 
  list<Ptr<Value> >::const_iterator it2End = set2.end();

  if(*it1 > *set2.rbegin() || *it2 > *set1.rbegin()) return Int32Value::New (1);

  while(it1 != it1End && it2 != it2End)
    {
      if(*it1 == *it2) return Int32Value::New (0);
      if(*it1 < *it2) { it1++; }
      else { it2++; }
    }

  //return true;
  return Int32Value::New (1);
  
  // Seems P2 does not support "true" and "false" as keywords. So use int32 for now

}

Ptr<FunctionExpr>
FDisjoint::New (Ptr<Expression> lst1, Ptr<Expression> lst2)
{
  Ptr<FDisjoint> retval = Create<FDisjoint> ();
  retval->m_lst1 = lst1;
  retval->m_lst2 = lst2;
  return retval;
}


/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Pennsylvania
 *
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
 *
 */

// #include "rapidnet-functions.h"
// #include <cstdlib>
// #include "ns3/ref-count-base.h"
// #include "ns3/simulator.h"
// #include "ns3/rapidnet-types.h"
// #include "rapidnet-application-base.h"
// #include "rapidnet-utils.h"
// #include "expression.h"
// #include <iostream>
// #include <fstream>
// #include "ns3/log.h"
// #include "ns3/byte-array-value.h"
// #include "ns3/nil-value.h"
// #include <stdlib.h>

// using namespace ns3;
// using namespace rapidnet;

// std::string
// PrintAsHex (char* arr, ssize_t bytes);

// /* ************************************************************** */

// Ptr<Value>
// FAppend::Eval (Ptr<Tuple> tuple)
// {
//   list<Ptr<Value> > result;
//   result.push_back (m_source->Eval (tuple));
//   return ListValue::New (result);
// }

// Ptr<FunctionExpr>
// FAppend::New (Ptr<Expression> source, Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FAppend> retval = Create<FAppend> ();
//   retval->m_source = source;
//   return retval;
// }

// /* ************************************************************** */



Ptr<Value>
FHashIP::Eval (Ptr<Tuple> tuple)
{
  string s = m_ipaddr->Eval(tuple)->ToString ();
  int32_t sum = 0;
  for (int i=0; i<s.size(); i++) {
    sum += s[i];
  }
  //uint32_t ipaddr = (rn_ipv4 (m_ipaddr->Eval (tuple))).Get ();
  return Int32Value::New(sum);
}


Ptr<FunctionExpr>
FHashIP::New (Ptr<Expression> ipaddr)
{
  Ptr<FHashIP> retval = Create<FHashIP> ();
  retval->m_ipaddr = ipaddr;
  return retval;
}

/* ************************************************************** */

Ptr<Value>
FModulo::Eval (Ptr<Tuple> tuple)
{
  return Operation::New (RN_MODULUS, m_divident, m_divisor)->Eval (tuple);
}

Ptr<FunctionExpr>
FModulo::New (Ptr<Expression> divident, Ptr<Expression> divisor)
{
  Ptr<FModulo> retval = Create<FModulo> ();
  retval->m_divident = divident;
  retval->m_divisor = divisor;
  return retval;
}

// /* ************************************************************** */

Ptr<Value>
FEmpty::Eval (Ptr<Tuple> tuple)
{
  return ListValue::New ();
}

Ptr<FunctionExpr>
FEmpty::New (Ptr<RapidNetApplicationBase> app)
{
  return Create<FEmpty> ();
}

// /* ************************************************************** */

// Ptr<Value>
// FConcat::Eval (Ptr<Tuple> tuple)
// {
//   list<Ptr<Value> > result;
//   Ptr<ListValue> headVal = DynamicCast<ListValue, Value> (
//     m_head->Eval (tuple));
//   Ptr<ListValue> tailVal = DynamicCast<ListValue, Value> (
//     m_tail->Eval (tuple));
//   for (rn_list_iterator it = headVal->Begin(); it != headVal->End (); ++it)
//   {
//     result.push_back (*it);
//   }
//   for (rn_list_iterator it = tailVal->Begin(); it != tailVal->End (); ++it)
//   {
//     result.push_back (*it);
//   }
//   return ListValue::New (result);
// }

// Ptr<FunctionExpr>
// FConcat::New (Ptr<Expression> head, Ptr<Expression> tail, Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FConcat> retval = Create<FConcat> ();
//   retval->m_head = head;
//   retval->m_tail = tail;
//   return retval;
// }

// /* ************************************************************** */

Ptr<Value>
FItem::Eval (Ptr<Tuple> tuple)
{
  list<Ptr<Value> > lst = rn_list (m_lst->Eval (tuple));
  uint32_t index = rn_int32 (m_index->Eval (tuple));

  rn_list_iterator it = lst.begin ();

  if (index > lst.size () || index < 1)
  {
    return NilValue::New ();
  }

  for (uint32_t i = 0; i < index - 1; i++)
  {
    it++;
  }

  return (*it)->Clone ();
}

Ptr<FunctionExpr>
FItem::New (Ptr<Expression> lst, Ptr<Expression> index, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FItem> retval = Create<FItem> ();
  retval->m_lst = lst;
  retval->m_index = index;
  return retval;
}

// /* ************************************************************** */

// Ptr<Value>
// FMember::Eval (Ptr<Tuple> tuple)
// {
//   Ptr<ListValue> lstVal = DynamicCast<ListValue, Value> (m_lst->Eval (tuple));
//   // Seems P2 does not support "true" and "false" as keywords. So use int32 for now
//   return Int32Value::New (lstVal->Contains (m_item->Eval (tuple)));
// }

// Ptr<FunctionExpr>
// FMember::New (Ptr<Expression> lst, Ptr<Expression> item, Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FMember> retval = Create<FMember> ();
//   retval->m_lst = lst;
//   retval->m_item = item;
//   return retval;
// }

// /* ************************************************************** */

// Ptr<Value>
// FNow::Eval (Ptr<Tuple> tuple)
// {
// 	return RealValue::New (Simulator::Now ().GetSeconds ());
// }

// Ptr<FunctionExpr>
// FNow::New (Ptr<RapidNetApplicationBase> app)
// {
//   return Create<FNow> ();
// }

// /* ************************************************************** */

// Ptr<Value>
// FDiffTime::Eval (Ptr<Tuple> tuple)
// {
//   return Operation::New (RN_MINUS, m_time2, m_time1)->Eval (tuple);
// }

// Ptr<FunctionExpr>
// FDiffTime::New (Ptr<Expression> time2, Ptr<Expression> time1, Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FDiffTime> retval = Create<FDiffTime> ();
//   retval->m_time2 = time2;
//   retval->m_time1 = time1;
//   return retval;
// }

// /* ************************************************************** */

// Ptr<Value>
// FHslsTtl::Eval (Ptr<Tuple> tuple)
// {
//   Ptr<Value> now = FNow::New ()->Eval (tuple);
//   Ptr<Value> normTimeDiff = Operation::New (RN_PLUS,
//     Operation::New (RN_DIVIDE, ValueExpr::New (now), m_periodAttrName),
//     ValueExpr::New (RealValue::New (1.0)))->Eval (tuple);
//   int32_t n = int32_t (rn_real (normTimeDiff));
//   int32_t ttl = n % 2 == 1 ? 2 : (n % 4 == 2 ? 4 : (n % 8 == 4 ? 8 : 16));
//   return Int32Value::New (ttl);
// }

// Ptr<FunctionExpr>
// FHslsTtl::New (Ptr<Expression> timeAttrName, Ptr<Expression> periodAttrName, Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FHslsTtl> retval = Create<FHslsTtl> ();
//   retval->m_timeAttrName = timeAttrName;
//   retval->m_periodAttrName = periodAttrName;
//   return retval;
// }

// /* ************************************************************** */

// Ptr<Value>
// FSize::Eval (Ptr<Tuple> tuple)
// {
//   Ptr<Value> lstVal = m_listAttrName->Eval (tuple);
//   NS_ASSERT_MSG (V_InstanceOf (lstVal, ListValue),
//     "f_size can only be applied to a list type attribute.");

//   return Int32Value::New ((DynamicCast<ListValue, Value> (lstVal))->Size ());
// }

// Ptr<FunctionExpr>
// FSize::New (Ptr<Expression> listAttrName, Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FSize> retval = Create<FSize> ();
//   retval->m_listAttrName = listAttrName;
//   return retval;
// }

// /* ************************************************************** */

// Ptr<Value>
// FLast::Eval (Ptr<Tuple> tuple)
// {
//   Ptr<Value> lstVal = m_listAttrName->Eval (tuple);
//   NS_ASSERT_MSG (V_InstanceOf (lstVal, ListValue),
//     "f_last can only be applied to a list type attribute.");

//   list<Ptr<Value> > lst = rn_list (lstVal);

//   if (lst.end () == lst.begin ())
//   {
//     return Ptr<Value> (NULL);
//   }

//   rn_list_iterator it = lst.end ();
//   return (*--it)->Clone ();
// }

// Ptr<FunctionExpr>
// FLast::New (Ptr<Expression> listAttrName, Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FLast> retval = Create<FLast> ();
//   retval->m_listAttrName = listAttrName;
//   return retval;
// }

// /* ************************************************************** */

// Ptr<Value>
// FRemoveLast::Eval (Ptr<Tuple> tuple)
// {
//   Ptr<Value> lstVal = m_listAttrName->Eval (tuple);
//   NS_ASSERT_MSG (V_InstanceOf (lstVal, ListValue),
//     "f_removeLast can only be applied to a list type attribute.");

//   list<Ptr<Value> > lst = rn_list (lstVal);

//   if (lst.end () == lst.begin ())
//   {
//     return Ptr<Value> (NULL);
//   }

//   rn_list_iterator it = lst.end ();
//   --it; // move one step back
//   list<Ptr<Value> > newlst;
//   for (rn_list_iterator jt = lst.begin (); jt != it; jt++)
//   {
//     newlst.push_back (*jt);
//   }
//   return ListValue::New (newlst);
// }

// Ptr<FunctionExpr>
// FRemoveLast::New (Ptr<Expression> listAttrName, Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FRemoveLast> retval = Create<FRemoveLast> ();
//   retval->m_listAttrName = listAttrName;
//   return retval;
// }



// Ptr<Value>
// FFirst::Eval (Ptr<Tuple> tuple)
// {
//   Ptr<Value> lstVal = m_listAttrName->Eval (tuple);
//   NS_ASSERT_MSG (V_InstanceOf (lstVal, ListValue),
//     "f_first can only be applied to a list type attribute.");

//   list<Ptr<Value> > lst = rn_list (lstVal);

//   if (lst.end () == lst.begin ())
//   {
//     return Ptr<Value> (NULL);
//   }

//   rn_list_iterator it = lst.begin ();
//   //clog<<"f_first " << (*it)->Clone() <<"\n";
//   return (*it)->Clone ();
// }

// Ptr<FunctionExpr>
// FFirst::New (Ptr<Expression> listAttrName, Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FFirst> retval = Create<FFirst> ();
//   retval->m_listAttrName = listAttrName;
//   return retval;
// }

Ptr<Value>
FRemoveFirst::Eval (Ptr<Tuple> tuple)
{
  Ptr<Value> lstVal = m_listAttrName->Eval (tuple);
  NS_ASSERT_MSG (V_InstanceOf (lstVal, ListValue),
    "f_removeFirst can only be applied to a list type attribute.");
  
  list<Ptr<Value> > lst = rn_list (lstVal);

  if (lst.end () == lst.begin ())
  {
   clog<<lstVal;
   return Ptr<Value> (NULL);

 }

 rn_list_iterator it = lst.begin ();
 rn_list_iterator it2 = lst.end ();
 ++it;

  list<Ptr<Value> > newlst;
  for (rn_list_iterator jt = it; jt != it2; jt++)
  {
    newlst.push_back (*jt);
  }

 Ptr<Value> newlist_ptr= ListValue::New(newlst);
 //clog<<"new list " << newlist_ptr <<"\n";
 return newlist_ptr;
}

Ptr<FunctionExpr>
FRemoveFirst::New (Ptr<Expression> listAttrName, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FRemoveFirst> retval = Create<FRemoveFirst> ();
  retval->m_listAttrName = listAttrName;
  return retval;
}


// /* ************************************************************** */

// Ptr<Value>
// FTypeOf::Eval (Ptr<Tuple> tuple)
// {
//   return StrValue::New (m_arg->Eval (tuple)->GetTypeName ());
// }

// Ptr<FunctionExpr>
// FTypeOf::New (Ptr<Expression> arg, Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FTypeOf> retval = Create<FTypeOf> ();
//   retval->m_arg = arg;
//   return retval;
// }

// /* ************************************************************** */

// Ptr<Value>
// FRand::Eval (Ptr<Tuple> tuple)
// {
//   stringstream ss;
//   ss << rand ();
//   return StrValue::New (ss.str ());
// }

// Ptr<FunctionExpr>
// FRand::New (Ptr<RapidNetApplicationBase> app)
// {
//   return Create<FRand> ();
// }

// /* ************************************************************** */

// Ptr<Value>
// FSha1::Eval (Ptr<Tuple> tuple)
// {
//   string str = m_arg->Eval (tuple)->ToString ();
//   return IdValue::New (GetSHA1Digest (str), 16);
// }

// Ptr<FunctionExpr>
// FSha1::New (Ptr<Expression> arg, Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FSha1> retval = Create<FSha1> ();
//   retval->m_arg = arg;
//   return retval;
// }

// /* ************************************************************** */

// Ptr<Value>
// FSvCreate::Eval (Ptr<Tuple> tuple)
// {
//   return SvValue::New();
// }

// Ptr<FunctionExpr>
// FSvCreate::New (Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FSvCreate> retval = Create<FSvCreate> ();
//   return retval;
// }

// /* ************************************************************** */

// Ptr<Value>
// FSvIn::Eval(Ptr<Tuple> tuple)
// {
//   Ptr<Value> svVPtr = m_svExpr->Eval (tuple);
//   Ptr<Value> strVPtr = m_strExpr->Eval (tuple);

//   int32_t result = SvValue::HashIn(svVPtr, strVPtr) ? 1 : 0;

//   return Int32Value::New(result);
// }

// Ptr<FunctionExpr>
// FSvIn::New (Ptr<Expression> svExpr,
//   Ptr<Expression> strExpr,
//   Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FSvIn> retval = Create<FSvIn>();
//   retval->m_svExpr = svExpr;
//   retval->m_strExpr = strExpr;
//   return retval;
// }

// /* ************************************************************** */

// Ptr<Value>
// FSvAndNot::Eval(Ptr<Tuple> tuple)
// {
//   Ptr<Value> svVPtr_1 = m_svExpr_1->Eval (tuple);
//   Ptr<Value> svVPtr_2 = m_svExpr_2->Eval (tuple);

//   return SvValue::AndNot(svVPtr_1, svVPtr_2);
// }

// Ptr<FunctionExpr>
// FSvAndNot::New (Ptr<Expression> svExpr_1,
//   Ptr<Expression> svExpr_2,
//   Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FSvAndNot> retval = Create<FSvAndNot>();
//   retval->m_svExpr_1 = svExpr_1;
//   retval->m_svExpr_2 = svExpr_2;
//   return retval;
// }

// /* ************************************************************** */

// Ptr<Value>
// FSvAppend::Eval(Ptr<Tuple> tuple)
// {
//   Ptr<Value> svVPtr = m_svExpr->Eval (tuple);
//   Ptr<Value> strVPtr = m_strExpr->Eval (tuple);

//   return  SvValue::HashInsert(svVPtr, strVPtr);
// }

// Ptr<FunctionExpr>
// FSvAppend::New (Ptr<Expression> svExpr,
//   Ptr<Expression> strExpr,
//   Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FSvAppend> retval = Create<FSvAppend>();
//   retval->m_svExpr = svExpr;
//   retval->m_strExpr = strExpr;
//   return retval;
// }

// /* ************************************************************** */

// Ptr<Value>
// FSvRemove::Eval(Ptr<Tuple> tuple)
// {
//   Ptr<Value> svVPtr = m_svExpr->Eval (tuple);
//   Ptr<Value> strVPtr = m_strExpr->Eval (tuple);

//   return SvValue::HashRemove(svVPtr, strVPtr);
// }

// Ptr<FunctionExpr>
// FSvRemove::New (Ptr<Expression> svExpr,
//   Ptr<Expression> strExpr,
//   Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FSvRemove> retval = Create<FSvRemove>();
//   retval->m_svExpr = svExpr;
//   retval->m_strExpr = strExpr;
//   return retval;
// }

// /* ************************************************************** */

Ptr<Value>
FPEdb::Eval(Ptr<Tuple> tuple)
{
  string prov = m_prov->Eval (tuple)-> ToString();

  return StrValue::New (prov);
}

Ptr<FunctionExpr>
FPEdb::New (Ptr<Expression> prov, Ptr<Expression> id, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FPEdb> retval = Create<FPEdb>();
  retval->m_prov = prov;
  retval->m_id = id;
  return retval;
}

 /* ************************************************************** */

Ptr<Value>
FPIdb::Eval(Ptr<Tuple> tuple)
{
  list<Ptr<Value> > provList = rn_list (m_provList->Eval (tuple));

  stringstream ss;

  if (provList.size() != 1)
  {
   ss << "(";
 }

 int index = 0;

 for (rn_list_iterator it = provList.begin (); it != provList.end (); it++)
 {
  if (index++!=0) ss << "+";
  ss << (*it)->ToString ();
}

//  string loc = m_loc->Eval (tuple)->ToString ();
//  ss << ")@[" << loc << "]";

if (provList.size() != 1) 
{
 ss << ")";
}

return StrValue::New (ss.str ());
}

Ptr<FunctionExpr>
FPIdb::New (Ptr<Expression> provList, Ptr<Expression> loc, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FPIdb> retval = Create<FPIdb>();
  retval->m_provList = provList;
  retval->m_loc = loc;
  return retval;
}

/* ************************************************************** */

Ptr<Value>
FPRule::Eval(Ptr<Tuple> tuple)
{
  list<Ptr<Value> > provList = rn_list (m_provList->Eval (tuple));

  stringstream ss;

  string rule = m_rule->Eval (tuple)->ToString ();

  uint32_t ipaddr = (rn_ipv4 (m_rloc->Eval (tuple))).Get ();
  ipaddr = (ipaddr / 256) % 65536;
  ss << rule << "@n" << ipaddr << "(";

    int index = 0;

    for (rn_list_iterator it = provList.begin (); it != provList.end (); it++)
    {
      if (index++!=0) ss << "*";
      ss << (*it)->ToString ();
    }

    ss << ")";

return StrValue::New (ss.str ());
}

Ptr<FunctionExpr>
FPRule::New (Ptr<Expression> provList, Ptr<Expression> rloc, Ptr<Expression> rule, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FPRule> retval = Create<FPRule>();
  retval->m_provList = provList;
  retval->m_rloc = rloc;
  retval->m_rule = rule;

  return retval;
}




// Ptr<Value>
// FSign::Eval(Ptr<Tuple> tuple)
// {
//   Ptr<Value> list1 = m_list->Eval(tuple);
//   Ptr<Value> keya = m_privateKey->Eval(tuple);

//   Ptr<StrValue> key = DynamicCast<StrValue, Value> (keya);
//   string keyString = key->ToString();

//   //clog<<"fsign eval start"<<endl;

//   FILE *stream;
//   stream = fopen(keyString.c_str(), "r");

//   //clog<<"secret key file: "<<keyString<<endl;

//   if (stream==NULL)
//   {
//     clog<<"error opening file \n";
//   }

//   EVP_PKEY* privateKey= PEM_read_PrivateKey (stream, NULL, NULL, NULL);
//   fclose(stream);
//   if (privateKey == NULL)
//   {
//     clog << "Error loading key from key file \n";
//     return Create<NilValue>();
//   }


//   //clog<<"list1 : " <<list1 <<"\n";
//   Ptr<ListValue> buf1 = DynamicCast<ListValue, Value> (list1);
//   uint32_t numBytes = buf1->GetSerializedSize ();
//   Buffer buffer;
//   buffer.AddAtStart (numBytes);
//   Buffer::Iterator it = buffer.Begin ();
//   buf1->Serialize (it);
//   uint8_t *arr = const_cast<uint8_t*> (buffer.PeekData());


//   EVP_MD_CTX md_ctx;
//   EVP_MD_CTX_init (&md_ctx);
//   EVP_SignInit (&md_ctx, EVP_sha1 ());
//   EVP_SignUpdate (&md_ctx, arr, numBytes);

//   //clog << "key: " << privateKey <<'\n';
//   uint8_t* signature = new uint8_t[EVP_PKEY_size (privateKey)];
//   uint32_t sigLen;

//   int err = EVP_SignFinal (&md_ctx, signature, &sigLen, privateKey);

//   Ptr<Value> retval = ByteArrayValue::New (signature, sigLen);

//   EVP_MD_CTX_cleanup (&md_ctx);

//   if (err != 1)
//   {
//     clog<<"error in sign\n";
//     return Create<NilValue> ();
//   }
//   else
//   {
//     //clog<<"no error in sign\n";
//     return retval;
//   }  
// }

// Ptr<FunctionExpr>
// FSign::New (Ptr<Expression> list1, Ptr<Expression> key1,Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FSign> retval = Create<FSign> ();
//   retval->m_list = list1;
//   retval->m_privateKey = key1;
//   return retval;
// }

// Ptr<Value>
// FVerify::Eval(Ptr<Tuple> tuple)
// {
//   Ptr<Value> signature1 = m_signature->Eval(tuple);
//   Ptr<Value> list1 = m_list->Eval(tuple);
//   Ptr<Value> keya = m_publicKey->Eval(tuple);

//   Ptr<StrValue> key = DynamicCast<StrValue, Value> (keya);
//   string keyString = key->ToString();

//   //clog<<" keystring " << keyString <<"\n";
  
//   FILE *stream;
//   stream = fopen (keyString.c_str(), "r");
//   if (stream==NULL)
//   {
//     clog<<"error opening file \n";
//   }

//   EVP_PKEY* publicKey= PEM_read_PUBKEY (stream, NULL, NULL, NULL);
//   fclose(stream);
//   if (publicKey == NULL)
//   {
//     clog << "Error loading key from key file \n";
//   }

//   //clog<<"list 1 " <<list1 <<"\n";
//   Ptr<ListValue> buf1 = DynamicCast<ListValue, Value> (list1);
//   uint32_t numBytes = buf1->GetSerializedSize ();
//   Buffer buffer;
//   buffer.AddAtStart (numBytes);
//   Buffer::Iterator it = buffer.Begin ();
//   buf1->Serialize (it);
//   uint8_t *arr = const_cast<uint8_t*> (buffer.PeekData());

//   Ptr<ByteArrayValue> buf2 = DynamicCast<ByteArrayValue, Value> (signature1);

//   EVP_MD_CTX md_ctx;
//   EVP_MD_CTX_init (&md_ctx);  
//   EVP_VerifyInit (&md_ctx, EVP_sha1 ());
//   EVP_VerifyUpdate (&md_ctx, arr, numBytes);
  
//   //clog<<"buffer 2 : " << buf2->ToString() <<"\n";

//   int err = EVP_VerifyFinal (&md_ctx, buf2->GetByteArrayPtr(),
//   buf2->GetByteArrayLen(), publicKey);
//   EVP_MD_CTX_cleanup (&md_ctx);
//   if(err==1)
//   {
//     //clog<<"verified\n";
//     return Int32Value::New (1);
//   }
//   else
//   {
//     //clog<<"not verified\n";
//     return Int32Value::New (0);
//   }  
// }


// Ptr<FunctionExpr>
// FVerify::New (Ptr<Expression> list1, Ptr <Expression> signature, Ptr<Expression> key1, Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FVerify> retval = Create<FVerify> ();
//   retval->m_signature = signature;
//   retval->m_list = list1;
//   retval->m_publicKey = key1;
//   //retval->m_node = node;
//   //retval->m_node1 = node1;
//   return retval;
// }

// Ptr<Value> 
// FVerifyMac::Eval (Ptr<Tuple> tuple)
// {
//   //Regenerating the mac
//   cout<<"fmac veri start"<<endl;
//   Ptr<Value> msg = m_message->Eval(tuple);
//   Ptr<Value> keya = m_secretKey->Eval(tuple);

//   Ptr<StrValue> key = DynamicCast<StrValue, Value> (keya);
//   string keyFile = key->ToString();
  
//   FILE *fp = fopen (keyFile.c_str(), "r");
  
//   clog<<"secret key file: "<<keyFile<<endl;

//   if (fp==NULL)
//     {
//       clog<<"error opening file \n";
//       return Create<NilValue>();
//     }

//   EVP_PKEY* secretKey= PEM_read_PrivateKey (fp, NULL, NULL, NULL);
//   fclose(fp);
//   if (secretKey == NULL)
//   {
//     clog << "Error loading key from key file: "<<keyFile<<endl;
//     return Create<NilValue>();
//   }
  

//   cout << "key: "<<secretKey << endl;
//   Ptr<ListValue> buf1 = DynamicCast<ListValue, Value> (msg);
  
//   cout << "buf1: "<< buf1->GetTypeName()<<":"<<buf1->ToString()<<endl; 

//   uint32_t numBytes = buf1->GetSerializedSize ();
//   cout <<"buffer of size: "<<numBytes<<endl;

//   Buffer buffer;
//   buffer.AddAtStart (numBytes);
//   Buffer::Iterator it = buffer.Begin ();
//   buf1->Serialize (it);
//   uint8_t *arr = const_cast<uint8_t*> (buffer.PeekData());
  
//   cout<<"fmac eval Serialize: "<<PrintAsHex((char*)arr,numBytes)<<endl;

//   HMAC_CTX ctx;
//   HMAC_CTX_init (&ctx);
//   //HMAC_Init_ex (&ctx, secretKey, EVP_PKEY_size (secretKey), EVP_sha1 (), NULL);
//   char hmac[] = "Password";
//   HMAC_Init_ex (&ctx, hmac, strlen((const char*)hmac), EVP_sha1 (), NULL);
//   HMAC_Update (&ctx, arr, numBytes);

//   //cout <<"fmac eval done hmac"<<endl;

//   uint8_t* signature = new uint8_t[EVP_MAX_MD_SIZE];
//   uint32_t sigLen;

//   HMAC_Final (&ctx, signature, &sigLen);
//   Ptr<Value> retval = ByteArrayValue::New (signature, sigLen);

//   HMAC_cleanup (&ctx);
//   cout<<"fmac eval result: "<< PrintAsHex((char*)signature,sigLen)<<endl;

//   //Mac comparison
//   Ptr<ByteArrayValue> mac1 = DynamicCast<ByteArrayValue,Value>(retval);
//   Ptr<ByteArrayValue> mac = DynamicCast<ByteArrayValue,Value>(m_mac->Eval(tuple));
//   //cout<<"fverifymac"<<endl;
//   //cout <<"mac : "<< PrintAsHex((char*)mac->GetByteArrayPtr(),mac->GetByteArrayLen()) <<endl;
//   //cout <<"mac1: "<< PrintAsHex((char*)mac1->GetByteArrayPtr(),mac1->GetByteArrayLen()) <<endl;

//   if (mac->Equals(mac1)){
//     //cout<<"mac verified"<<endl;
//     return Create<Int32Value>(1);
//   }
//   else {
//     //cout<<"mac not verified"<<endl;
//     return Create<Int32Value>(0);
//   }
// }

// Ptr<FunctionExpr>
// FVerifyMac::New(Ptr<Expression> msg, Ptr<Expression> mac, Ptr<Expression> skey, Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FVerifyMac> retval = Create<FVerifyMac>();
//   retval->m_message = msg;
//   retval->m_mac = mac;
//   retval->m_secretKey = skey;
//   return retval;
// }


Ptr<Value>
FPrepend::Eval (Ptr<Tuple> tuple)
{
  list<Ptr<Value> > result;
  Ptr<ListValue> listvalue = DynamicCast<ListValue, Value> (
    m_listvalue->Eval (tuple));
  result.push_back (m_value->Eval (tuple));

  for (rn_list_iterator it = listvalue->Begin(); it != listvalue->End (); ++it)
  {
    result.push_back (*it);
  }

  return ListValue::New (result);
}

Ptr<FunctionExpr>
FPrepend::New (Ptr<Expression> value1, Ptr<Expression> listvalue1, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FPrepend> retval = Create<FPrepend> ();
  retval->m_value = value1;
  retval->m_listvalue = listvalue1;
  return retval;
}

// Ptr<Value>
// FAppend2::Eval (Ptr<Tuple> tuple)
// {
//   list<Ptr<Value> > result;
//  // result.push_back (m_source->Eval (tuple));
//   Ptr<ListValue> listvalue1 = DynamicCast<ListValue, Value> (
//     m_listvalue->Eval (tuple));
//   for (rn_list_iterator it = listvalue1->Begin(); it != listvalue1->End (); ++it)
//   {
//     result.push_back (*it);
//   }
//   result.push_back(m_source->Eval(tuple));
//   return ListValue::New (result);
// }

// Ptr<FunctionExpr>
// FAppend2::New (Ptr<Expression> source, Ptr<Expression> list, Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FAppend2> retval = Create<FAppend2> ();
//   retval->m_source = source;
//   retval->m_listvalue = list;
//   return retval;
// }


// std::string
// PrintAsHex (char* arr, ssize_t bytes)
// {
//   // Bytes should be multiples of 4
//   std::ostringstream sb;
//   char str[33];
//   for (unsigned i = 0; i < bytes/sizeof(uint32_t); i++)
//   {
//     sprintf(str, "%08x", ((uint32_t*) arr)[i]);
//     sb << str;
//   }
//   return sb.str();
// }

// /* Jul 11, 2012
//  * Cheng Luo 
// */

// Ptr<Value>
// FMAC::Eval(Ptr<Tuple> tuple)
// {
//   cout<<"fmac eval start"<<endl;
//   Ptr<Value> msg = m_message->Eval(tuple);
//   Ptr<Value> keya = m_secretKey->Eval(tuple);

//   Ptr<StrValue> key = DynamicCast<StrValue, Value> (keya);
//   string keyFile = key->ToString();
  
//   FILE *fp = fopen (keyFile.c_str(), "r");
  
//   clog<<"secret key file: "<<keyFile<<endl;

//   if (fp==NULL)
//     {
//       clog<<"error opening file \n";
//       return Create<NilValue>();
//     }

//   EVP_PKEY* secretKey= PEM_read_PrivateKey (fp, NULL, NULL, NULL);
//   fclose(fp);
//   if (secretKey == NULL)
//   {
//     clog << "Error loading key from key file: "<<keyFile<<endl;
//     return Create<NilValue>();
//   }
  

//   cout << "key: "<<secretKey << endl;
//   Ptr<ListValue> buf1 = DynamicCast<ListValue, Value> (msg);
  
//   cout << "buf1: "<< buf1->GetTypeName()<<":"<<buf1->ToString()<<endl; 

//   uint32_t numBytes = buf1->GetSerializedSize ();
//   cout <<"buffer of size: "<<numBytes<<endl;

//   Buffer buffer;
//   buffer.AddAtStart (numBytes);
//   Buffer::Iterator it = buffer.Begin ();
//   buf1->Serialize (it);
//   uint8_t *arr = const_cast<uint8_t*> (buffer.PeekData());
  
//   cout<<"fmac eval Serialize: "<<PrintAsHex((char*)arr,numBytes)<<endl;

//   HMAC_CTX ctx;
//   HMAC_CTX_init (&ctx);
//   //HMAC_Init_ex (&ctx, secretKey, EVP_PKEY_size (secretKey), EVP_sha1 (), NULL);
//   char hmac[] = "Password";
//   HMAC_Init_ex (&ctx, hmac, strlen((const char*)hmac), EVP_sha1 (), NULL);
//   HMAC_Update (&ctx, arr, numBytes);

//   //cout <<"fmac eval done hmac"<<endl;

//   uint8_t* signature = new uint8_t[EVP_MAX_MD_SIZE];
//   uint32_t sigLen;

//   HMAC_Final (&ctx, signature, &sigLen);
//   Ptr<Value> retval = ByteArrayValue::New (signature, sigLen);

//   HMAC_cleanup (&ctx);
//   cout<<"fmac eval result: "<< PrintAsHex((char*)signature,sigLen)<<endl;

//   return retval;
// }

// Ptr<FunctionExpr>
// FMAC::New (Ptr<Expression> message, Ptr<Expression> secretKey, Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FMAC> retval = Create<FMAC> ();
//   retval->m_message = message;
//   retval->m_secretKey = secretKey;
//   return retval;
// }

// Ptr<Value>
// FReverse::Eval (Ptr<Tuple> tuple)
// {
//   list<Ptr<Value> > result = rn_list (m_list->Eval(tuple));
//   result.reverse();

//   return ListValue::New (result);
// }

// Ptr<FunctionExpr>
// FReverse::New (Ptr<Expression> list, Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FReverse> retval = Create<FReverse> ();
//   retval->m_list = list;
//   return retval;
// }

// Ptr<Value>
// FGet::Eval (Ptr<Tuple> tuple)
// {
//   Ptr<Value> lstVal = m_list->Eval (tuple);
//   Ptr<Value> iVal = m_i->Eval(tuple);
//   int i = (DynamicCast<Int32Value, Value> (iVal))->GetInt32Value();

//   if (!V_InstanceOf (lstVal, ListValue))
//   {
//     return Create<NilValue> ();
//   }
//   //NS_ASSERT_MSG (V_InstanceOf (lstVal, ListValue), "f_get can only be applied to a list type attribute.");

//   list<Ptr<Value> > lst = rn_list (lstVal);

//   //cout<<"fget i: "<<i<<"\tlst.size:"<<lst.size()<<endl;
//   if (lst.end () == lst.begin ())
//   {
//     //cout <<"not found 1"<<endl;
//     return Create<NilValue> ();
//   }

//   if (i >= (int)(lst.size()) )
//   {
//     //cout<<"not found 2"<<endl;
//     return Create<NilValue> ();
//   }
  
//   rn_list_iterator it = lst.begin();
//   for (int j=0; j<i; j++)
//     it++;
//   //cout<<"fget result: "<<*it<<endl;
//   return (*it)->Clone ();
// }

// Ptr<FunctionExpr>
// FGet::New (Ptr<Expression> list, Ptr<Expression> i, Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FGet> retval = Create<FGet> ();
//   retval->m_list = list;
//   retval->m_i = i;
//   return retval;
// }


// Ptr<Value>
// FSubString::Eval (Ptr<Tuple> tuple)
// {
//   string result = str->Eval(tuple)->ToString();
//   int32_t startIndexInt = rn_int32(startIndex->Eval(tuple));
//   int32_t endIndexInt = rn_int32(endIndex->Eval(tuple));
//   string substring = "";
//   if(startIndexInt <= endIndexInt && endIndexInt<= result.length())
//     {
//       substring = result.substr(startIndexInt,endIndexInt-startIndexInt);
//     }
//   return StrValue::New (substring);
// }

// Ptr<FunctionExpr> 
// FSubString::New (Ptr<Expression> str, Ptr<Expression> startIndex,Ptr<Expression> endIndex, Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FSubString> retval = Create<FSubString> ();
//   retval->str = str;
//   retval->startIndex = startIndex;
//   retval->endIndex = endIndex;
//   return retval;
// }

Ptr<Value>
FIndexOf::Eval (Ptr<Tuple> tuple)
{
  string result = str->Eval(tuple)->ToString();
  string patternToFind = strToFind->Eval(tuple)->ToString();
  int found = result.find(patternToFind);
  if(found==string::npos)
    found = -1;
  return Int32Value::New (found);
}

Ptr<FunctionExpr> 
FIndexOf::New (Ptr<Expression> str, Ptr<Expression> strToFind, Ptr<RapidNetApplicationBase> app)
{
  Ptr<FIndexOf> retval = Create<FIndexOf> ();
  retval->str = str;
  retval->strToFind = strToFind;
  return retval;
}

// Ptr<Value>
// FStrLength::Eval (Ptr<Tuple> tuple)
// {
//   string result = str->Eval(tuple)->ToString();
//   return Int32Value::New (result.length());
// }

// Ptr<FunctionExpr> 
// FStrLength::New (Ptr<Expression> str, Ptr<RapidNetApplicationBase> app)
// {
//   Ptr<FStrLength> retval = Create<FStrLength> ();
//   retval->str = str;
//   return retval;
// }



// /* ************************************************************** */



