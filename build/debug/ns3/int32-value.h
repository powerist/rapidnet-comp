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

#ifndef INT32VALUE_H
#define INT32VALUE_H

#include "value.h"
#define rn_int32(ptr) \
  DynamicCast<Int32Value, Value> (ptr)->GetInt32Value ()

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
 * \ingroup rapidnet_values
 *
 * \brief RapidNet value type for 32-bit integers.
 */
class Int32Value: public Value
{
public:

  Int32Value ();

  Int32Value (int32_t value);

  virtual ~Int32Value ();

  virtual uint32_t GetSerializedSize (void) const;

  virtual void Serialize (Buffer::Iterator& start) const;

  virtual uint32_t Deserialize (Buffer::Iterator& end);

  virtual string ToString () const;

  /**
   * \brief Returns the 32-bit integer value.
   */
  inline int32_t GetInt32Value () const;

  virtual Ptr<Value> Clone () const;

  virtual bool Equals (const Ptr<Value> v) const;

  virtual bool Less (const Ptr<Value> v) const;

  virtual Ptr<Value> Eval (Operator op, Ptr<Tuple> tuple,
    Ptr<Expression> expr = Ptr<Expression> (NULL));

  /**
   * \brief Factory method that creates and returns a new Int32Value
   *        object. The default value is 0.
   *
   */
  static Ptr<Value> New (int32_t value = 0);

protected:

  friend class boost::serialization::access;

  template<typename Archive>
  void serialize(Archive& ar, const unsigned version)
  {
    ar & boost::serialization::base_object<Value>(*this);    
    ar & m_value;
  }  

  int32_t m_value;
};


int32_t
Int32Value::GetInt32Value () const
{
  return m_value;
}

} // namespace rapidnet
} // namespace ns3

BOOST_CLASS_EXPORT_KEY(ns3::rapidnet::Int32Value)

#endif // INT32VALUE_H
