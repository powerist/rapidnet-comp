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

#ifndef STRVALUE_H_
#define STRVALUE_H_

#include <iostream>
#include <string>
#include "value.h"

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#define rn_str(ptr) \
  DynamicCast<StrValue, Value> (ptr)->GetStrValue ()

//Forward declaration of class boost::serialization::access
namespace boost{
  namespace serialization{
    class access;
  }
}

using namespace std;

namespace ns3 {
namespace rapidnet {

/**
 * \ingroup rapidnet_values
 *
 * \brief A value type for strings.
 */
class StrValue: public Value
{
public:
  StrValue ();
  
  StrValue (string value);

  virtual ~StrValue ();

  virtual uint32_t GetSerializedSize (void) const;

  virtual void Serialize (Buffer::Iterator& start) const;

  virtual uint32_t Deserialize (Buffer::Iterator& end);

  virtual string ToString () const;

  /**
   * \brief Returns the string value.
   */
  inline string GetStrValue () const;

  virtual Ptr<Value> Clone () const;

  virtual bool Equals (const Ptr<Value> v) const;

  virtual bool Less (const Ptr<Value> v) const;

  virtual Ptr<Value> Eval (Operator op, Ptr<Tuple> tuple,
    Ptr<Expression> expr = Ptr<Expression> (NULL));

  /**
   * \brief Factory method to create a new StringValue object. The default
   *        value is an empty string.
   */
  static Ptr<Value> New (string value = "");

protected:

  friend class boost::serialization::access;

  template<typename Archive>
  void serialize(Archive& ar, const unsigned version)
  {
    std::cout << "Serialize StrValue" << std::endl;
    ar & boost::serialization::base_object<Value>(*this);    
    ar & m_value;
    std::cout << "Serialize StrValue finished" << std::endl;
  }

  string m_value;
};



string
StrValue::GetStrValue () const
{
  return m_value;
}

} // namespace rapidnet
} // namespace ns3

BOOST_CLASS_EXPORT_KEY(ns3::rapidnet::StrValue)

#endif // STRVALUE_H
