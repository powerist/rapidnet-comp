#ifndef PKTFWDNORMDISTHLISTONLINEQUERY_HELPER_H
#define PKTFWDNORMDISTHLISTONLINEQUERY_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-dist-hlist-online-query.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormdisthlistonlinequery {

class PktfwdNormDistHlistOnlineQuery;

class PktfwdNormDistHlistOnlineQueryHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormDistHlistOnlineQueryHelper ()
  {
    m_factory.SetTypeId (PktfwdNormDistHlistOnlineQuery::GetTypeId ());
  }
  virtual ~PktfwdNormDistHlistOnlineQueryHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormDistHlistOnlineQuery> ();
  }
};

} // namespace pktfwdnormdisthlistonlinequery
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMDISTHLISTONLINEQUERY_HELPER_H

