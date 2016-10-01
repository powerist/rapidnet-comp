#ifndef DNSPROVDHLISTLAYERHSONLINE_HELPER_H
#define DNSPROVDHLISTLAYERHSONLINE_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "dns-prov-dhlist-layerhs-online.h"

namespace ns3 {
namespace rapidnet {
namespace dnsprovdhlistlayerhsonline {

class DnsProvDhlistLayerhsOnline;

class DnsProvDhlistLayerhsOnlineHelper: public RapidNetApplicationHelper
{
public:
  DnsProvDhlistLayerhsOnlineHelper ()
  {
    m_factory.SetTypeId (DnsProvDhlistLayerhsOnline::GetTypeId ());
  }
  virtual ~DnsProvDhlistLayerhsOnlineHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<DnsProvDhlistLayerhsOnline> ();
  }
};

} // namespace dnsprovdhlistlayerhsonline
} // namespace rapidnet
} // namespace ns3

#endif // DNSPROVDHLISTLAYERHSONLINE_HELPER_H

