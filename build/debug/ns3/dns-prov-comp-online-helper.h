#ifndef DNSPROVCOMPONLINE_HELPER_H
#define DNSPROVCOMPONLINE_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "dns-prov-comp-online.h"

namespace ns3 {
namespace rapidnet {
namespace dnsprovcomponline {

class DnsProvCompOnline;

class DnsProvCompOnlineHelper: public RapidNetApplicationHelper
{
public:
  DnsProvCompOnlineHelper ()
  {
    m_factory.SetTypeId (DnsProvCompOnline::GetTypeId ());
  }
  virtual ~DnsProvCompOnlineHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<DnsProvCompOnline> ();
  }
};

} // namespace dnsprovcomponline
} // namespace rapidnet
} // namespace ns3

#endif // DNSPROVCOMPONLINE_HELPER_H

