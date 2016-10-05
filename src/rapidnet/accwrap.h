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

#ifndef ACCWRAP_H
#define ACCWRAP_H

#include "ns3/ref-count-base.h"
#include "ns3/assert.h"
#include "relation-base.h"
#include "relation.h"
#include "ns3/abort.h"
#include "ns3/list-value.h"

using namespace std;

namespace ns3 {
  namespace rapidnet {

    /**
     *  \ingroup rapidnet_library
     *
     *  \brief Base class for accumulate-wrap implementation
     *
     *  Accumulators are variants of Aggregates
     */

    class AccWrap: public RefCountBase
    {
    public:
    
      AccWrap (list<string> params);
     
      const static string ACC_ATTR;

      // Note: Need to perform last join in Compute
      virtual Ptr<RelationBase> Compute (Ptr<RelationBase> result, Ptr<Tuple> event, list<string> allAttrs, uint32_t accPos) = 0;
      //const list<string>& lAttrNames, const list<string>& rAttrNames) = 0;

      void SetParams (list<string> params);

    protected:

      Ptr<Relation> GetNewAccRelation (string name);
    
      int m_type;

      list<string> m_allAttrs;

      uint32_t m_accPos;

      string m_accAttrName;

      list<string> m_params;

    };

    /**
     * \ingroup rapidnet_library
     * \brief AccRandK impelmentation
     */

    class AccWrapRandK : public AccWrap
    {
    public:
      AccWrapRandK (list<string> params);
      virtual Ptr<RelationBase> Compute (Ptr<RelationBase> result, Ptr<Tuple> event, list<string> allAttrs, uint32_t accPos);
      //    const list<string>& lAttrNames, const list<string>& rAttrNames);
      list<Ptr<Value> > GetNodeIds (list<Ptr<Tuple> >& allTuples, uint8_t numK);
    };

    /**
     * \ingroup rapidnet_library
     * \brief AccRandK impelmentation
     */

    class AccWrapList : public AccWrap
    {
    public:
      AccWrapList (list<string> params);
      virtual Ptr<RelationBase> Compute (Ptr<RelationBase> result, Ptr<Tuple> event, list<string> allAttrs, uint32_t accPos);
      //    const list<string>& lAttrNames, const list<string>& rAttrNames);
      list<Ptr<Value> > GetAttrList (list<Ptr<Tuple> >& allTuples);
    };


    /**
     * \ingroup rapidnet_library
     * \brief AccRandK impelmentation
     */

    class AccWrapRandWeightedK : public AccWrap
    {
    public:
      AccWrapRandWeightedK (list<string> params);
      virtual Ptr<RelationBase> Compute (Ptr<RelationBase> result, Ptr<Tuple> event, list<string> allAttrs, uint32_t accPos);
      //    const list<string>& lAttrNames, const list<string>& rAttrNames);
      list<Ptr<Value> > GetNodeIds (list<Ptr<Tuple> >& allTuples, uint32_t numK, uint32_t field);
    };


    /**
     * \ingroup rapidnet_library
     * \brief AccRandK impelmentation
     */

    class AccWrapDisjoint : public AccWrap
    {
    public:
      AccWrapDisjoint (list<string> params);
      virtual Ptr<RelationBase> Compute (Ptr<RelationBase> result, Ptr<Tuple> event, list<string> allAttrs, uint32_t accPos);
      bool IsDisjoint(Ptr<ListValue> lst1, Ptr<ListValue> lst2);
    
      struct AscComp {
	uint32_t field;
	bool operator() (Ptr<Value> i, Ptr<Value> j);
      };

    };

  } // namespace rapidnet
} // namespace ns3
#endif
