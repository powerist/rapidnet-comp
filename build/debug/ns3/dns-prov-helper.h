#ifndef DNSPROV_HELPER_H
#define DNSPROV_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "dns-prov.h"

namespace ns3 {
namespace rapidnet {
namespace dnsprov {

class DnsProv;

class DnsProvHelper: public RapidNetApplicationHelper
{
public:
  DnsProvHelper ()
  {
    m_factory.SetTypeId (DnsProv::GetTypeId ());
  }
  virtual ~DnsProvHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<DnsProv> ();
  }
};

} // namespace dnsprov
} // namespace rapidnet
} // namespace ns3

#endif // DNSPROV_HELPER_H

