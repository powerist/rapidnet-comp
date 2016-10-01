#ifndef PKTFWDNORMNODEVPROVQUERY_HELPER_H
#define PKTFWDNORMNODEVPROVQUERY_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-nodev-provquery.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormnodevprovquery {

class PktfwdNormNodevProvquery;

class PktfwdNormNodevProvqueryHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormNodevProvqueryHelper ()
  {
    m_factory.SetTypeId (PktfwdNormNodevProvquery::GetTypeId ());
  }
  virtual ~PktfwdNormNodevProvqueryHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormNodevProvquery> ();
  }
};

} // namespace pktfwdnormnodevprovquery
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMNODEVPROVQUERY_HELPER_H

