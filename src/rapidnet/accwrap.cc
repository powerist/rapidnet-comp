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

#include "accwrap.h"
#include <list>
#include <cstdlib>
#include <sstream>
#include "ns3/int32-value.h"
#include "ns3/bool-value.h"
#include "temp-relation.h"
#include "ns3/list-value.h"
#include "ns3/random-variable.h"

using namespace std;
using namespace ns3;
using namespace ns3::rapidnet;

const string AccWrap::ACC_ATTR = "accumulator";

AccWrap::AccWrap (list<string> params)
{
  m_params = params;
}


void
AccWrap::SetParams (list<string> params)
{
  m_params = params;
}

Ptr<Relation>
AccWrap::GetNewAccRelation (string name)
{
  list<string>::iterator it;
  Ptr<Relation> accResults = Relation::New (name);
  uint32_t i = 0;
  for (it = m_allAttrs.begin (); it != m_allAttrs.end (); ++it)
    { 
      i++;
      if (i == m_accPos)
	{
	  // Name of the attribute on which accumulator is applied
	  m_accAttrName = *it;
	}
      else 
	{
	  accResults->AddKeyAttribute (attrdeftype (*it, ANYTYPE));
	}
    }
  return accResults;
}


// AccList

AccWrapList::AccWrapList (list<string> params)
  : AccWrap (params)
{
}

Ptr<RelationBase>
AccWrapList::Compute (Ptr<RelationBase> result, Ptr<Tuple> event, list<string> allAttrs, uint32_t accPos)
//  const list<string>& lAttrNames, const list<string>& rAttrNames)
{
  NS_ASSERT_MSG (accPos <= allAttrs.size (), 
		 "AccWrap::Compute: Accumulate position greater than number of attributes");
  m_allAttrs = allAttrs;
  m_accPos = accPos;

  Ptr<Tuple> clone = CopyObject<Tuple> (event);
  //  Ptr<RelationBase> joinResult = result->Join (event, lAttrNames, rAttrNames);

  //Extract numK and repetitions
  uint32_t numK = 3, reps = 1;

  Ptr<Relation> accResults = GetNewAccRelation (result->GetName ());
  accResults->SetRelaxed (true);

  // Add a dummy key if accPos is 1
  string dummyKey;
  string dummyKeyValue = "same-for-all";
  if (m_accPos == 1)
    {
      stringstream ss;
      ss << "accwraprandk-dummy-key-" << rand ();
      dummyKey = ss.str ();
      accResults->AddKeyAttribute (attrdeftype (dummyKey, ANYTYPE));
    }

  list<Ptr<Tuple> > allTuples = result->GetAllTuples();
  Ptr<TupleAttribute> dummyKeyAttr = TupleAttribute::New (dummyKey,
							  StrValue::New (dummyKeyValue));

  if (allTuples.size() == 0)
    {
      //Insert cloned event with accumulator with null list
      list<Ptr<Value> > nodeIds;
      clone->AddAttribute(TupleAttribute::New(ACC_ATTR,ListValue::New(nodeIds)));
      if (m_accPos == 1)
	{
	  clone->AddAttribute (dummyKeyAttr);
	}
      // Below statement Commented for empty Tuples
      //TODO :  Need to handle empty tuple scenario.
      //accResults->Insert(clone);
      return accResults;
    }

  // Get any tuple
  Ptr<Tuple> anyTuple = *(allTuples.begin());
  if (m_accPos == 1)
    {
      anyTuple->AddAttribute (dummyKeyAttr);
    }

  list<Ptr<Value> > attrList = GetAttrList(allTuples);
  anyTuple->AddAttribute(TupleAttribute::New(ACC_ATTR, ListValue::New(attrList)));
  accResults->Insert(anyTuple);

  return accResults;
}


list<Ptr<Value> >
AccWrapList::GetAttrList (list<Ptr<Tuple> >& allTuples)
{
  list<Ptr<Value> > result;
  list<Ptr<Tuple> >::iterator iter;
  
  for(iter=allTuples.begin(); iter!=allTuples.end(); iter++)
    {
      Ptr<Tuple> tuple = *(iter);
      Ptr<Value> value = DynamicCast<Value> (tuple->GetAttribute(m_accAttrName)->GetValue());
      result.push_back(value);
    }
  return result;
}


// AccRandK

AccWrapRandK::AccWrapRandK (list<string> params)
  : AccWrap (params)
{
}

Ptr<RelationBase>
AccWrapRandK::Compute (Ptr<RelationBase> result, Ptr<Tuple> event, list<string> allAttrs, uint32_t accPos)
//  const list<string>& lAttrNames, const list<string>& rAttrNames)
{
  NS_ASSERT_MSG (accPos <= allAttrs.size (), 
		 "AccWrap::Compute: Accumulate position greater than number of attributes");
  m_allAttrs = allAttrs;
  m_accPos = accPos;

  Ptr<Tuple> clone = CopyObject<Tuple> (event);
  //  Ptr<RelationBase> joinResult = result->Join (event, lAttrNames, rAttrNames);

  //Extract numK and repetitions
  uint32_t numK = 3, reps = 1;
  if (m_params.size() > 0)
    {
      list<string>::iterator iter = m_params.begin();
      numK = atoi (iter->c_str());
      if (m_params.size() > 1)
	{
	  iter++;
	  reps = atoi(iter->c_str());
	}
    }

  Ptr<Relation> accResults = GetNewAccRelation (result->GetName ());
  accResults->SetRelaxed (true);

  // Add a dummy key if accPos is 1
  string dummyKey;
  string dummyKeyValue = "same-for-all";
  if (m_accPos == 1)
    {
      stringstream ss;
      ss << "accwraprandk-dummy-key-" << rand ();
      dummyKey = ss.str ();
      accResults->AddKeyAttribute (attrdeftype (dummyKey, ANYTYPE));
    }

  list<Ptr<Tuple> > allTuples = result->GetAllTuples();
  Ptr<TupleAttribute> dummyKeyAttr = TupleAttribute::New (dummyKey,
							  StrValue::New (dummyKeyValue));

  if (allTuples.size() == 0)
    {
      //Insert cloned event with accumulator with null list
      list<Ptr<Value> > nodeIds;
      clone->AddAttribute(TupleAttribute::New(ACC_ATTR,ListValue::New(nodeIds)));
      if (m_accPos == 1)
	{
	  clone->AddAttribute (dummyKeyAttr);
	}
      // Below statement Commented for empty Tuples
      //TODO :  Need to handle empty tuple scenario.
      //accResults->Insert(clone);
      return accResults;
    }

  // Get any tuple
  Ptr<Tuple> anyTuple = *(allTuples.begin());
  if (m_accPos == 1)
    {
      anyTuple->AddAttribute (dummyKeyAttr);
    }

  if (reps > 1)
    {
      // Init accumulator
      list<Ptr<Value> > nodeSets;

      for (uint32_t i = 0 ; i < reps ; i++)
	{
	  list<Ptr<Value> > nodeIds = GetNodeIds(allTuples, numK);  
	  if (nodeIds.size () == numK)
	    {
	      nodeSets.push_back(ListValue::New(nodeIds)); 
	    }
	  else
	    {
	      break;
	    }
	}
      //Insert result
      anyTuple->AddAttribute(TupleAttribute::New(ACC_ATTR, ListValue::New(nodeSets)));
      accResults->Insert(anyTuple);
    }
  else
    {
      list<Ptr<Value> > nodeIds = GetNodeIds(allTuples, numK);
      anyTuple->AddAttribute(TupleAttribute::New(ACC_ATTR, ListValue::New(nodeIds)));
      accResults->Insert(anyTuple);
    }
  return accResults;
}

list<Ptr<Value> >
AccWrapRandK::GetNodeIds (list<Ptr<Tuple> >& allTuples, uint8_t numK)
{
  list<Ptr<Value> > nodeIds;
  list<Ptr<Tuple> > tempList;
  for (uint8_t j = 0; j < numK; j++)
    {
      //Compute RandK here and insert ACC_ATTR
      if (allTuples.size() == 0)
	break;
      SeedManager::SetSeed(time(0));
      uint32_t random = (uint32_t) UniformVariable(0,allTuples.size()-1).GetValue();
      list<Ptr<Tuple> >::iterator iter = allTuples.begin();
      advance (iter, random);
      Ptr<Tuple> randTuple = *(iter);
      Ptr<Value> nodeId = DynamicCast<Value> (randTuple->GetAttribute(m_accAttrName)->GetValue());
      nodeIds.push_back (nodeId);
      tempList.push_back(*iter);
      // Remove entry from Table
      allTuples.erase(iter);
    }
  //Make sure that the nodes are added back to the list of tuples. 
  list<Ptr<Tuple> >::iterator it;
  for (it = tempList.begin (); it != tempList.end() ; it ++)
    {
      allTuples.push_back(*it); 
    }
  tempList.clear (); 
  return nodeIds;
}


// AccRandWeightedK

AccWrapRandWeightedK::AccWrapRandWeightedK (list<string> params)
  : AccWrap (params)
{
}

Ptr<RelationBase>
AccWrapRandWeightedK::Compute (Ptr<RelationBase> result, Ptr<Tuple> event, list<string> allAttrs, uint32_t accPos)
//  const list<string>& lAttrNames, const list<string>& rAttrNames)
{
  NS_ASSERT_MSG (accPos <= allAttrs.size (), 
		 "AccWrap::Compute: Accumulate position greater than number of attributes");
  m_allAttrs = allAttrs;
  m_accPos = accPos;

  Ptr<Tuple> clone = CopyObject<Tuple> (event);
  //  Ptr<RelationBase> joinResult = result->Join (event, lAttrNames, rAttrNames);

  //Extract numK and field for random weighted
  uint32_t numK = 3, field = 1;
  if (m_params.size() > 0)
    {
      list<string>::iterator iter = m_params.begin();
      field = atoi (iter->c_str());
      if (m_params.size() > 1)
	{
	  iter++;
	  numK = atoi(iter->c_str());
	}
    }

  Ptr<Relation> accResults = GetNewAccRelation (result->GetName ());
  accResults->SetRelaxed (true);

  // Add a dummy key if accPos is 1
  string dummyKey;
  string dummyKeyValue = "same-for-all";
  if (m_accPos == 1)
    {
      stringstream ss;
      ss << "accwraprandk-dummy-key-" << rand ();
      dummyKey = ss.str ();
      accResults->AddKeyAttribute (attrdeftype (dummyKey, ANYTYPE));
    }

  list<Ptr<Tuple> > allTuples = result->GetAllTuples();
  Ptr<TupleAttribute> dummyKeyAttr = TupleAttribute::New (dummyKey,
							  StrValue::New (dummyKeyValue));

  if (allTuples.size() == 0)
    {
      //Insert cloned event with accumulator with null list
      list<Ptr<Value> > nodeIds;
      clone->AddAttribute(TupleAttribute::New(ACC_ATTR,ListValue::New(nodeIds)));
      if (m_accPos == 1)
	{
	  clone->AddAttribute (dummyKeyAttr);
	}
      // Below statement Commented for empty Tuples
      //TODO :  Need to handle empty tuple scenario.
      //accResults->Insert(clone);
      return accResults;
    }

  // Get any tuple
  Ptr<Tuple> anyTuple = *(allTuples.begin());
  if (m_accPos == 1)
    {
      anyTuple->AddAttribute (dummyKeyAttr);
    }

  list<Ptr<Value> > selNodes = GetNodeIds(allTuples, numK, field);
  anyTuple->AddAttribute(TupleAttribute::New(ACC_ATTR, ListValue::New(selNodes)));
  accResults->Insert(anyTuple);
  
  return accResults;
}

list<Ptr<Value> >
AccWrapRandWeightedK::GetNodeIds (list<Ptr<Tuple> >& allTuples, uint32_t numK, uint32_t field)
{
  list<Ptr<Value> > nodes;
  Ptr<Value> selNode;
  list<Ptr<Tuple> >::iterator selIter;
  list<Ptr<Tuple> > tempList;
  Ptr<Tuple> randTuple;
  SeedManager::SetSeed(time(0));
  for (uint8_t j = 0; j < numK; j++)
    {
      //Compute RandK here and insert ACC_ATTR
      if (allTuples.size() == 0)
	break;
      double random;
      list<Ptr<Tuple> >::iterator iter = allTuples.begin();
      double sum = 0;
      for(;iter != allTuples.end (); iter++)
	{
          randTuple = *(iter);
          Ptr<ListValue> lst = DynamicCast<ListValue, Value> (randTuple->GetAttribute(m_accAttrName)->GetValue() );
          rn_list_iterator iterI = DynamicCast<ListValue> (lst)->Begin();
          advance(iterI,field); 
          Ptr<Int32Value> curwt =  DynamicCast<Int32Value, Value> (*iterI);
          sum += curwt->GetInt32Value ();
          random = UniformVariable(0.0,1.0).GetValue();
          if (random < (curwt->GetInt32Value () / sum ) )
            {
	      selNode = lst;
	      selIter = iter;
            }
	}
      nodes.push_back (selNode);
      tempList.push_back(*selIter);
      // Remove entry from Table
      allTuples.erase(selIter);
    }
  //Make sure that the nodes are added back to the list of tuples. 
  list<Ptr<Tuple> >::iterator it;
  for (it = tempList.begin (); it != tempList.end() ; it ++)
    {
      allTuples.push_back(*it); 
    }
  tempList.clear (); 
  return nodes;
}



AccWrapDisjoint::AccWrapDisjoint (list<string> params)
  : AccWrap (params)
{
}


Ptr<RelationBase>
AccWrapDisjoint::Compute (Ptr<RelationBase> result, Ptr<Tuple> event, list<string> allAttrs, uint32_t accPos)
{
  
  int timeStart = clock(); 

  NS_ASSERT_MSG (accPos <= allAttrs.size (), 
		 "AccWrap::Compute: Accumulate position greater than number of attributes");
  m_allAttrs = allAttrs;
  m_accPos = accPos;

  Ptr<Tuple> clone = CopyObject<Tuple> (event);
  //  Ptr<RelationBase> joinResult = result->Join (event, lAttrNames, rAttrNames);

  uint32_t reps = 3;
  /*if (m_params.size() > 0)
  {
    list<string>::iterator iter = m_params.begin();
    reps = atoi (iter->c_str());
    }*/

  list<Ptr<Tuple> > joinTuples = result->GetAllTuples(); 

  list<Ptr<Value> > paths; 

  for (list<Ptr<Tuple> >::iterator it = joinTuples.begin (); it != joinTuples.end (); ++it)
    {
      Ptr<Tuple> feasiblePathTuple = *it;
      list<Ptr<Value> > lst; 
      if(feasiblePathTuple->HasAttribute("bestPath_attr4"))
	{
	  Ptr<TupleAttribute> patht = feasiblePathTuple->GetAttribute ("bestPath_attr4");
	  Ptr<TupleAttribute> costt = feasiblePathTuple->GetAttribute ("bestPath_attr5");
	  Ptr<TupleAttribute> bandwidtht = feasiblePathTuple->GetAttribute ("bestPath_attr6");
	  Ptr<TupleAttribute> latencyt = feasiblePathTuple->GetAttribute ("bestPath_attr7");
	  Ptr<TupleAttribute> countt = feasiblePathTuple->GetAttribute ("bestPath_attr9");
	  Ptr<Int32Value> cnt = DynamicCast<Int32Value,Value>(countt->GetValue());
	  reps = cnt->GetInt32Value(); 
	  lst.push_back(patht->GetValue());
	  lst.push_back(costt->GetValue());
	  lst.push_back(bandwidtht->GetValue());
	  lst.push_back(latencyt->GetValue());
	}
      else if(feasiblePathTuple->HasAttribute("exPath_attr4"))
	{
	  Ptr<TupleAttribute> patht = feasiblePathTuple->GetAttribute ("exPath_attr4");
	  Ptr<TupleAttribute> costt = feasiblePathTuple->GetAttribute ("exPath_attr5");
	  Ptr<TupleAttribute> bandwidtht = feasiblePathTuple->GetAttribute ("exPath_attr6");
	  Ptr<TupleAttribute> latencyt = feasiblePathTuple->GetAttribute ("exPath_attr7");
	  Ptr<TupleAttribute> countt = feasiblePathTuple->GetAttribute ("exPath_attr9");
	  Ptr<Int32Value> cnt = DynamicCast<Int32Value,Value>(countt->GetValue());
	  reps = cnt->GetInt32Value(); 
	  lst.push_back(patht->GetValue());
	  lst.push_back(costt->GetValue());
	  lst.push_back(bandwidtht->GetValue());
	  lst.push_back(latencyt->GetValue());
	}
      Ptr<Value> value = ListValue::New(lst);
      paths.push_back(value);
    }
 
  /*
  Ptr<ListValue> check = DynamicCast<ListValue, Value>(ListValue::New(paths));
  Ptr<ListValue> check1 = DynamicCast<ListValue, Value>(ListValue::New(m_params));
  Ptr<ListValue> check2 = DynamicCast<ListValue, Value>(ListValue::New(allAttrs));
  */
  AscComp ascComp;
  ascComp.field = 1;
  paths.sort(ascComp);
  /*
    cout<<"********************************************************************"<<endl;
    cout<<check->ToString()<<endl;
    cout<<check1->ToString()<<endl;
    cout<<check2->ToString()<<endl;
    cout<<"********************************************************************"<<endl;
  */
  list <Ptr<Value> > nodesSeen; 
  list <Ptr<Value> > finalPaths; 

  for (list<Ptr<Value> >::iterator it = paths.begin (); it != paths.end (); ++it)
    {
      if(finalPaths.size() == reps)
	break; 

      Ptr<Value> p = *it; 
      if(nodesSeen.size() == 0)
	{
	  //Ptr<ListValue> tp1 = DynamicCast<ListValue,Value> (p); 
	  //list<Ptr<Value> > tpnodes = tp1->GetListValue(); 
	  rn_list_iterator iterI = DynamicCast<ListValue, Value> (p)->Begin();
	  list<Ptr<Value> > tpnodes = (DynamicCast<ListValue, Value> (*iterI))->GetListValue(); 
	  tpnodes.pop_front(); 
	  tpnodes.pop_back();
	  for (list<Ptr<Value> >::iterator it2 = tpnodes.begin (); it2 != tpnodes.end (); ++it2)
	    {
	      nodesSeen.push_back(*it2); 
	    }
	  
	  finalPaths.push_back(*iterI); 
	}
      else
	{
	  //Ptr<ListValue> tp1 = DynamicCast<ListValue,Value> (p); 
	  rn_list_iterator iterI = DynamicCast<ListValue, Value> (p)->Begin();
	  list<Ptr<Value> > tpnodes = (DynamicCast<ListValue, Value> (*iterI))->GetListValue(); 
	  tpnodes.pop_front(); 
	  tpnodes.pop_back();
	  
	  bool seen = false; 

	  for (list<Ptr<Value> >::iterator it2 = tpnodes.begin (); it2 != tpnodes.end (); ++it2)
	    {
	      Ptr<Value> pNode = *it2; 
	      for (list<Ptr<Value> >::iterator it3 = nodesSeen.begin (); it3 != nodesSeen.end (); ++it3)
		{
		  Ptr<Value> node = *it3; 
		  if(node->Equals(pNode))
		    {
		      seen = true; 
		      break; 
		    }
		}

	      if(seen)
		break; 
	    }
	  
	  if(seen) 
	    continue; 
	  else
	    {
	      for (list<Ptr<Value> >::iterator it2 = tpnodes.begin (); it2 != tpnodes.end (); ++it2)
		nodesSeen.push_back(*it2); 
	      finalPaths.push_back(*iterI);
	    }
	}
      
    }
  
  //cout<<(ListValue::New(finalPaths))->ToString()<<endl;

  Ptr<Relation> accResults = GetNewAccRelation (result->GetName ());
  accResults->SetRelaxed (true);

  Ptr<Tuple> anyTuple = *(joinTuples.begin());
  anyTuple->AddAttribute(TupleAttribute::New(ACC_ATTR, ListValue::New(finalPaths)));
  accResults->Insert(anyTuple);

  int timeComplete = clock(); 
  cout<<"Time taken to compute do path Computation - "<<((timeComplete - timeStart)/(CLOCKS_PER_SEC/1000)) <<" milliseconds. "<<endl;
  return accResults; 
}


bool
AccWrapDisjoint::AscComp::operator() (Ptr<Value> i, Ptr<Value> j)
{
  rn_list_iterator iterI = DynamicCast<ListValue, Value> (i)->Begin();
  rn_list_iterator iterJ = DynamicCast<ListValue, Value> (j)->Begin();
  advance (iterI, field);
  advance (iterJ, field);
  Ptr<Value> valI = *iterI;
  Ptr<Value> valJ = *iterJ;
  //cout<<"Value i = "<<i->ToString()<<endl;
  //cout<<"Value j = "<<j->ToString()<<endl;
   
  //cout<<"Comparing -- "<<valI->ToString()<< " vs. " << valJ->ToString()<<endl;
  
  return (valI->Less(valJ));
}


bool
AccWrapDisjoint::IsDisjoint(Ptr<ListValue> lstVal1, Ptr<ListValue> lstVal2)
{
  list<Ptr<Value> > set1 = lstVal1->GetListValue(); 
  list<Ptr<Value> > set2 = lstVal2->GetListValue(); 
 
  if(set1.empty() || set2.empty()) return true;

  list<Ptr<Value> >::const_iterator it1 = set1.begin(); 
  list<Ptr<Value> >::const_iterator it1End = set1.end();
  list<Ptr<Value> >::const_iterator it2 = set2.begin(); 
  list<Ptr<Value> >::const_iterator it2End = set2.end();
   
  it1++;
  it2++;
  it1End--;
  it2End--;
        
  if(*it1 > *set2.rbegin() || *it2 > *set1.rbegin()) return true;

  while(it1 != it1End && it2 != it2End)
    {
      if(*it1 == *it2) return false;
      if(*it1 < *it2) { it1++; }
      else { it2++; }
    }

  return true;
}
 
