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

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <list>
#include "expression.h"
#include "tuple.h"
#include "rapidnet-application-base.h"
#include "evp-key.h"
#include "ns3/ptr.h"
#include "sendlog-authentication-manager.h"
#include "ns3/log.h"
#include "openssl/pem.h"
#include "ns3/byte-array-value.h"
#include "openssl/evp.h"
#include "openssl/hmac.h"

#define CLOG "clog"
#define COUT "cout"
#define __GetStream(stream_name) \
    (stream_name==COUT? cout: \
    (stream_name==CLOG? clog: \
     clog))

using namespace std;

namespace ns3 {
namespace rapidnet {

/**
 * \ingroup rapidnet_library
 * \defgroup rapidnet_functions RapidNet Functions
 */

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that returns a list with the given
 *        value(s) appended in it.
 */
class FAppend : public FunctionExpr
{
public:
  virtual ~FAppend () {}
  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);
  static Ptr<FunctionExpr> New (Ptr<Expression> source, Ptr<RapidNetApplicationBase> app = NULL);

protected:
  Ptr<Expression> m_source;
};


class FPrepend : public FunctionExpr
{
public:

  virtual ~FPrepend () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> value1, Ptr<Expression> listvalue1, Ptr<RapidNetApplicationBase> app = NULL);

protected:

  Ptr<Expression> m_value, m_listvalue;
};


class FHashIP : public FunctionExpr
{
public:
  virtual ~FHashIP () {}
  virtual Ptr<Value> Eval(Ptr<Tuple> tuple);
  static Ptr<FunctionExpr> New(Ptr<Expression> ipaddr);

protected:
  Ptr<Expression> m_ipaddr;
};


class FModulo : public FunctionExpr
{
public:
  virtual ~FModulo () {}
  virtual Ptr<Value> Eval(Ptr<Tuple> tuple);
  static Ptr<FunctionExpr> New(Ptr<Expression> divident, Ptr<Expression> divisor);

protected:
  Ptr<Expression> m_divident, m_divisor;
};



class FEmpty : public FunctionExpr
{
public:

  virtual ~FEmpty () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<RapidNetApplicationBase> app = NULL);

};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that concatenates two list values.
 */
class FConcat : public FunctionExpr
{
public:

  virtual ~FConcat () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> head, Ptr<Expression> tail, Ptr<RapidNetApplicationBase> app = NULL);

protected:

  Ptr<Expression> m_head, m_tail;
};

class FItem : public FunctionExpr
{
public:

  virtual ~FItem () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> lst, Ptr<Expression> index, Ptr<RapidNetApplicationBase> app = NULL);

protected:

  Ptr<Expression> m_lst, m_index;
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that tests if a given value is the member of
 *        a list.
 */
class FMember : public FunctionExpr
{
public:

  virtual ~FMember () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> lst, Ptr<Expression> item, Ptr<RapidNetApplicationBase> app = NULL);

protected:

  Ptr<Expression> m_lst, m_item;
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that returns the current time as a string.
 */
class FNow : public FunctionExpr
{
public:

  virtual ~FNow () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<RapidNetApplicationBase> app = NULL);
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RadidNet function that returns the difference of two time
 * strings (in seconds) as an integer.
 */
class FDiffTime : public FunctionExpr
{
public:

  virtual ~FDiffTime () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> time2, Ptr<Expression> time1, Ptr<RapidNetApplicationBase> app = NULL);

protected:

  Ptr<Expression> m_time2, m_time1;
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RadidNet function that returns the TTL or the number of hops that
 * a triggered update should propogate in triggered HSLS implementation.
 */
class FHslsTtl : public FunctionExpr
{
public:

  virtual ~FHslsTtl () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> time,
    Ptr<Expression> hslsPeriod, Ptr<RapidNetApplicationBase> app = NULL);

protected:

  Ptr<Expression> m_timeAttrName, m_periodAttrName;
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that returns the size of a list.
 */
class FSize : public FunctionExpr
{
public:

  virtual ~FSize () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> listAttrName, Ptr<RapidNetApplicationBase> app = NULL);

protected:

  Ptr<Expression> m_listAttrName;
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that returns the last element of a list.
 */
class FLast : public FunctionExpr
{
public:

  virtual ~FLast () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> listAttrName, Ptr<RapidNetApplicationBase> app = NULL);

protected:

  Ptr<Expression> m_listAttrName;
};


/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that removes and returns last element of a list.
 */
class FRemoveLast : public FunctionExpr
{
public:

  virtual ~FRemoveLast () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> listAttrName, Ptr<RapidNetApplicationBase> app = NULL);

protected:

  Ptr<Expression> m_listAttrName;
};






/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that returns the first element of a list.
 */

class FFirst : public FunctionExpr
{
public:

  virtual ~FFirst () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> listAttrName, Ptr<RapidNetApplicationBase> app = NULL);

protected:

  Ptr<Expression> m_listAttrName;
};


/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that removes and returns first element of a list.
 */
class FRemoveFirst : public FunctionExpr
{
public:

  virtual ~FRemoveFirst () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> listAttrName, Ptr<RapidNetApplicationBase> app = NULL);

protected:

  Ptr<Expression> m_listAttrName;
};






/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that returns the string representation of the
 *        type of the given expression.
 */
class FTypeOf : public FunctionExpr
{
public:

  virtual ~FTypeOf () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> arg, Ptr<RapidNetApplicationBase> app = NULL);

protected:

  Ptr<Expression> m_arg;
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that returns a random number as a string.
 */
class FRand : public FunctionExpr
{
public:

  virtual ~FRand () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<RapidNetApplicationBase> app = NULL);
};


/**
 * \ingroup rapidnet_functions
 *
 * \brief A RadpidNet function that returns the SHA1 hash of the string
 * representation of the given value.
 */
class FSha1 : public FunctionExpr
{
public:

  virtual ~FSha1 () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> arg, Ptr<RapidNetApplicationBase> app = NULL);

protected:

  Ptr<Expression> m_arg;
};

/*
 * \ingroup rapidnet_functions
 *
 * \brief A RadpidNet function that creates a new and empty summary vector.
 *
 * Example: SV := f_svcreate()
 */
class FSvCreate : public FunctionExpr
{
public:

  virtual ~FSvCreate () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<RapidNetApplicationBase> app = NULL);
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that tests whether a given ID (string) is
 * hashed in a summary vector.
 *
 * @return 0:no or 1:yes, type:INT32
 *
 * Example: Result := f_svin(SV, ID)
 */
class FSvIn : public FunctionExpr
{
public:

  virtual ~FSvIn () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> svExpr,
                                Ptr<Expression> strExpr,
                                Ptr<RapidNetApplicationBase> app = NULL);
protected:

  Ptr<Expression> m_svExpr, m_strExpr;
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that computes the andNot result of
 * two summary vectors: (SV_1 & (~ SV_2)).
 *
 * @return The result summary vector, type:SvValue
 *
 * Example: ResultSV := f_svandnot(SV_1, SV_2), SV_1 and SV_2 are not affected
 */
class FSvAndNot : public FunctionExpr
{
public:

  virtual ~FSvAndNot () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> svExpr_1,
                                Ptr<Expression> svExpr_2,
                                Ptr<RapidNetApplicationBase> app = NULL);
protected:

  Ptr<Expression> m_svExpr_1, m_svExpr_2;
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief A RapidNet function that hashes a given ID (string) into a new
 * summary vector.
 *
 * @return Resultant summary vector, type: SV
 *
 * examples: ResultSV := f_svappend(SV, ID), SV is not affected
 */
class FSvAppend : public FunctionExpr
{
public:
  virtual ~FSvAppend () {}
  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);
  static Ptr<FunctionExpr> New (Ptr<Expression> svExpr,
                                Ptr<Expression> strExpr,
                                Ptr<RapidNetApplicationBase> app = NULL);
private:
  Ptr<Expression> m_svExpr, m_strExpr;
};

/**
 * \ingroup rapidnet_functions
 *
 * \brief FA RapidNet function that hashes a given ID (string) out from
 * a new summary vector.
 *
 * @return The resultant summary vector, type:SV
 *
 * Example: ResultSV := f_svremove(SV, ID), SV is not affected
 */
class FSvRemove : public FunctionExpr
{
public:
  virtual ~FSvRemove () {}
  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);
  static Ptr<FunctionExpr> New (Ptr<Expression> svExpr,
                                Ptr<Expression> strExpr,
                                Ptr<RapidNetApplicationBase> app = NULL);
private:
  Ptr<Expression> m_svExpr, m_strExpr;
};

class FPEdb : public FunctionExpr
{
public:
  virtual ~FPEdb () {}
  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);
  static Ptr<FunctionExpr> New (Ptr<Expression> prov, Ptr<Expression> id, Ptr<RapidNetApplicationBase> app = NULL);

private:
  Ptr<Expression> m_prov, m_id;
};

class FPIdb : public FunctionExpr
{
public:
  virtual ~FPIdb () {}
  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);
  static Ptr<FunctionExpr> New (Ptr<Expression> provList, Ptr<Expression> loc, Ptr<RapidNetApplicationBase> app = NULL);

private:
  Ptr<Expression> m_provList, m_loc;
};

class FPRule : public FunctionExpr
{
public:
  virtual ~FPRule () {}
  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);
  static Ptr<FunctionExpr> New (Ptr<Expression> provList, Ptr<Expression> rloc,Ptr<Expression> rule, Ptr<RapidNetApplicationBase> app = NULL);

private:
  Ptr<Expression> m_provList, m_rloc, m_rule;
};


class FAppend2 : public FunctionExpr
{
public:

  virtual ~FAppend2 () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> source, Ptr<Expression> list, Ptr<RapidNetApplicationBase> app = NULL);

protected:

  Ptr<Expression> m_source,  m_listvalue;
};


/* 
 * Jul 11, 2012
 * Cheng Luo 
*/
class FSign: public FunctionExpr
{
public:

  virtual ~FSign () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New(Ptr<Expression> list1, Ptr<Expression> key1, Ptr<RapidNetApplicationBase> app= NULL);

protected:

  Ptr<Expression> m_list;
  Ptr<Expression> m_privateKey;
};

class FVerify: public FunctionExpr
{
public:

  virtual ~FVerify () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New(Ptr<Expression> list1, Ptr<Expression> signature, Ptr<Expression> publicKey, Ptr<RapidNetApplicationBase> app= NULL);

protected:

  Ptr<Expression> m_signature, m_list;
  Ptr<Expression> m_publicKey;
  //Ptr<Expression> m_node, m_node1;
};

class FMAC : public FunctionExpr
{
public:
  virtual ~FMAC () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> message, Ptr<Expression> secretKey, Ptr<RapidNetApplicationBase> app = NULL);

protected: 
  Ptr<Expression> m_message, m_secretKey;
};


class FReverse : public FunctionExpr
{
public:
  virtual ~FReverse () {}

  Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> list, Ptr<RapidNetApplicationBase> app = NULL);

protected: 
  Ptr<Expression> m_list;
};

class FGet : public FunctionExpr
{
public:
  virtual ~FGet () {}

  Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New (Ptr<Expression> list, Ptr<Expression> i, Ptr<RapidNetApplicationBase> app = NULL);

protected: 
  Ptr<Expression> m_list;
  Ptr<Expression> m_i;
};


class FVerifyMac : public FunctionExpr
{
public:
  virtual ~FVerifyMac () {}

  virtual Ptr<Value> Eval (Ptr<Tuple> tuple);

  static Ptr<FunctionExpr> New(Ptr<Expression> msg, Ptr<Expression> mac, Ptr<Expression> skey, Ptr<RapidNetApplicationBase> app= NULL);

protected:
  Ptr<Expression> m_message, m_mac, m_secretKey;
};


class FSubString : public FunctionExpr
{
public:
  virtual ~FSubString () {}

  Ptr<Value> Eval (Ptr<Tuple> tuple);

 static Ptr<FunctionExpr> New (Ptr<Expression> str, Ptr<Expression> startIndex,Ptr<Expression> endIndex, Ptr<RapidNetApplicationBase> app = NULL);

protected: 
  Ptr<Expression> str;
  Ptr<Expression> startIndex;
  Ptr<Expression> endIndex;
};


class FIndexOf : public FunctionExpr
{
public:
  virtual ~FIndexOf () {}

  Ptr<Value> Eval (Ptr<Tuple> tuple);

 static Ptr<FunctionExpr> New (Ptr<Expression> str, Ptr<Expression> strToFind,Ptr<RapidNetApplicationBase> app = NULL);

protected: 
  Ptr<Expression> str;
  Ptr<Expression> strToFind;
};

class FStrLength : public FunctionExpr
{
public:
  virtual ~FStrLength () {}

  Ptr<Value> Eval (Ptr<Tuple> tuple);

 static Ptr<FunctionExpr> New (Ptr<Expression> str,Ptr<RapidNetApplicationBase> app = NULL);

protected: 
  Ptr<Expression> str;
};

} // namespace rapidnet
} // namespace ns3

#endif // FUNCTIONS_H

