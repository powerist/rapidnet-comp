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

#ifndef TUPLEATTRIBUTE_H
#define TUPLEATTRIBUTE_H

#include <string>
#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/assert.h"
#include "ns3/str-value.h"

#include "ns3/bool-value.h"
#include "ns3/byte-array-value.h"
#include "ns3/id-value.h"
#include "ns3/int32-value.h"
#include "ns3/ipv4-value.h"
#include "ns3/list-value.h"
#include "ns3/nil-value.h"
#include "ns3/real-value.h"
#include "ns3/sv-value.h"


#include <typeinfo>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

using namespace std;

//Forward declaration of class boost::serialization::access
namespace boost{
  namespace serialization{
    class access;
  }
}

namespace ns3 {
namespace rapidnet {

/**
 * \ingroup rapidnet_library
 *
 * \brief Represents an attribute of a tuple. An attribute is a name-value
 *        pair.
 *
 * TupleAttribute is a name-value pair. The name is a string value object
 * and the value can be of any RapidNet value type. A @see Tuple is a collection
 * of TupleAttribute objects and maps them by their name.
 */
class TupleAttribute : public Object
{
public:

  static TypeId GetTypeId (void);

  TupleAttribute ();

  TupleAttribute (string name, Ptr<Value> value);

  TupleAttribute (TupleAttribute &attr);

  virtual ~TupleAttribute ();

  /**
   * \brief Returns the size in bytes when serialized.
   */
  virtual uint32_t GetSerializedSize (void) const;

  /**
   * \brief Serializes the object for transmitting over the
   *        network inside a RapidNet header.
   */
  virtual void Serialize (Buffer::Iterator& start) const;

  /**
   * \brief De-serializes the object that is embedded in a
   *         RapidNet header received from the network.
   */
  virtual uint32_t Deserialize (Buffer::Iterator& end);

  /**
   * \brief Returns the string representation of the TupleAttribute.
   */
  virtual string ToString ();

  /**
   * @return The attribute name.
   */
  inline string GetName () const;

  /**
   * @return The attribute value.
   */
  inline virtual Ptr<Value> GetValue () const;

  /**
   * \brief Returns the type code.
   */
  inline virtual ValueTypeId GetType () const;

  /**
   * \brief Sets the given value.
   */
  inline virtual void SetValue (Ptr<Value> value);

  /**
   * \brief Returns true if the value this object is equal to the
   *        value of the given TupleAttribute object.
   */
  virtual bool ValueEquals (const Ptr<TupleAttribute> attr) const;

  /**
   * \brief Compares with the given TupleAttribute for equality.
   */
  virtual bool Equals (const Ptr<TupleAttribute> attr) const;

  /**
   * \brief Compares the given two TupleAttribute objects and returns
   *        true if the first is "less" than the second. False otherwise.
   */
  static bool Less (const Ptr<TupleAttribute> a1,
    const Ptr<TupleAttribute> a2);

  /**
   * \brief Factory method to create a new TupleAttribute object.
   *
   * The default name is "unnamed" and default value set to a null pointer.
   */
  static Ptr<TupleAttribute> New (string name = "unnamed",
    Ptr<Value> value = Ptr<Value> (NULL));

  /**
   * \brief A prototype method that creates a new TupleAttribute object with
   *        the given name and value set to a clone of the value of the given
   *        TupleAttribute object.
   */
  static Ptr<TupleAttribute> New (string name,
    Ptr<TupleAttribute> attr);

protected:

  friend class boost::serialization::access;

  template<typename Archive>
  void serialize(Archive& ar, const unsigned version)
  {
    ar.template register_type<ListValue>();
    ar.template register_type<IdValue>();
    ar.template register_type<Ipv4Value>();
    ar.template register_type<BoolValue>();
    ar.template register_type<Int32Value>();
    ar.template register_type<StrValue>();
    ar.template register_type<RealValue>();

    std::cout << "Reach tuple attribute?";    
    ar & boost::serialization::base_object<Object>(*this);

    std::cout << "Serialize m_value" << endl;
    Value* vp = GetPointer(m_value);
    bool exist = false;
    BoolValue* bp = dynamic_cast<BoolValue*>(vp);
    if (bp != NULL){std::cout << "Bool type" << endl; exist = true;}
    ByteArrayValue* bap = dynamic_cast<ByteArrayValue*>(vp);
    if (bap != NULL){std::cout << "Byte Array type" << endl; exist = true;}    
    IdValue* ip = dynamic_cast<IdValue*>(vp);
    if (ip != NULL){std::cout << "IdValue type" << endl; exist = true;}        
    Int32Value* itp = dynamic_cast<Int32Value*>(vp);
    if (itp != NULL){std::cout << "Int32Value type" << endl; exist = true;}
    Ipv4Value* ivp = dynamic_cast<Ipv4Value*>(vp);
    if (ivp != NULL){std::cout << "Ipv4Value type" << endl; exist = true;}
    ListValue* lp = dynamic_cast<ListValue*>(vp);
    if (lp != NULL)
      {
        std::cout << "ListValue type" << endl; 
        exist = true;

        std::cout << "List size: " << lp->Size() << endl;

        list<Ptr<Value> > vlist = lp->GetListValue();
        list<Ptr<Value> >::iterator itr;
        for (itr = vlist.begin();itr != vlist.end();itr++)
          {
            Value* vvp = GetPointer(*itr);
            IdValue* vip = dynamic_cast<IdValue*>(vvp);
            if (vip != NULL){std::cout << "IdValue type" << endl;}        
          }
      }
    RealValue* rp = dynamic_cast<RealValue*>(vp);
    if (rp != NULL){std::cout << "RealValue type" << endl; exist = true;}
    SvValue* svp = dynamic_cast<SvValue*>(vp);
    if (svp != NULL){std::cout << "SvValue type" << endl; exist = true;}
    StrValue* sp = dynamic_cast<StrValue*>(vp);
    if (sp != NULL){std::cout << "StrValue type" << endl; exist = true;}
    NilValue* np = dynamic_cast<NilValue*>(vp);
    if (np != NULL){std::cout << "NilValue type" << endl; exist = true;}
    std:cout << "Type found: " << exist << endl;
    ar & m_value;

    ar & m_name;
    std::cout << "Finish tuple-attribute.h" << endl;
  }

  /**
   * \brief Sets name to the the given string.
   */
  inline void SetName (string name);

  Ptr<StrValue> m_name;

  Ptr<Value> m_value;
};


ostream& operator << (ostream& os, const Ptr<TupleAttribute>& attr);

void
TupleAttribute::SetName (string name)
{
  m_name = DynamicCast<StrValue, Value> (StrValue::New (name));
}

string
TupleAttribute::GetName () const
{
  return m_name->GetStrValue ();
}

Ptr<Value>
TupleAttribute::GetValue () const
{
  return m_value;
}

void
TupleAttribute::SetValue (Ptr<Value> value)
{
  m_value = value;
}

ValueTypeId
TupleAttribute::GetType () const
{
  return m_value->GetType ();
}
} // namespace rapidnet
} // namespace ns3

BOOST_CLASS_EXPORT_KEY(ns3::rapidnet::TupleAttribute)

#endif // TUPLEATTRIBUTE_H
