#ifndef DNSPROVDISTHLISTRMITM_HELPER_H
#define DNSPROVDISTHLISTRMITM_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "dns-prov-dist-hlist-rmitm.h"

namespace ns3 {
namespace rapidnet {
namespace dnsprovdisthlistrmitm {

class DnsProvDistHlistRmitm;

class DnsProvDistHlistRmitmHelper: public RapidNetApplicationHelper
{
public:
  DnsProvDistHlistRmitmHelper ()
  {
    m_factory.SetTypeId (DnsProvDistHlistRmitm::GetTypeId ());
  }
  virtual ~DnsProvDistHlistRmitmHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<DnsProvDistHlistRmitm> ();
  }
};

} // namespace dnsprovdisthlistrmitm
} // namespace rapidnet
} // namespace ns3

#endif // DNSPROVDISTHLISTRMITM_HELPER_H

