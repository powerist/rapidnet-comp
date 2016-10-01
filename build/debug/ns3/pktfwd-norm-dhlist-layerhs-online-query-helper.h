#ifndef PKTFWDNORMDHLISTLAYERHSONLINEQUERY_HELPER_H
#define PKTFWDNORMDHLISTLAYERHSONLINEQUERY_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-dhlist-layerhs-online-query.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormdhlistlayerhsonlinequery {

class PktfwdNormDhlistLayerhsOnlineQuery;

class PktfwdNormDhlistLayerhsOnlineQueryHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormDhlistLayerhsOnlineQueryHelper ()
  {
    m_factory.SetTypeId (PktfwdNormDhlistLayerhsOnlineQuery::GetTypeId ());
  }
  virtual ~PktfwdNormDhlistLayerhsOnlineQueryHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormDhlistLayerhsOnlineQuery> ();
  }
};

} // namespace pktfwdnormdhlistlayerhsonlinequery
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMDHLISTLAYERHSONLINEQUERY_HELPER_H

