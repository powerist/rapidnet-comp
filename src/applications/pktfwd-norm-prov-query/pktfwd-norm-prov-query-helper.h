#ifndef PKTFWDNORMPROVQUERY_HELPER_H
#define PKTFWDNORMPROVQUERY_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-prov-query.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormprovquery {

class PktfwdNormProvQuery;

class PktfwdNormProvQueryHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormProvQueryHelper ()
  {
    m_factory.SetTypeId (PktfwdNormProvQuery::GetTypeId ());
  }
  virtual ~PktfwdNormProvQueryHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormProvQuery> ();
  }
};

} // namespace pktfwdnormprovquery
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMPROVQUERY_HELPER_H

