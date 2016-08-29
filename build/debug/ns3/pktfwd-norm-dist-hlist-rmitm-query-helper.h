#ifndef PKTFWDNORMDISTHLISTRMITMQUERY_HELPER_H
#define PKTFWDNORMDISTHLISTRMITMQUERY_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-dist-hlist-rmitm-query.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormdisthlistrmitmquery {

class PktfwdNormDistHlistRmitmQuery;

class PktfwdNormDistHlistRmitmQueryHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormDistHlistRmitmQueryHelper ()
  {
    m_factory.SetTypeId (PktfwdNormDistHlistRmitmQuery::GetTypeId ());
  }
  virtual ~PktfwdNormDistHlistRmitmQueryHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormDistHlistRmitmQuery> ();
  }
};

} // namespace pktfwdnormdisthlistrmitmquery
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMDISTHLISTRMITMQUERY_HELPER_H

