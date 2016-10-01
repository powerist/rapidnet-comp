#ifndef DNS_HELPER_H
#define DNS_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "dns.h"

namespace ns3 {
namespace rapidnet {
namespace dns {

class Dns;

class DnsHelper: public RapidNetApplicationHelper
{
public:
  DnsHelper ()
  {
    m_factory.SetTypeId (Dns::GetTypeId ());
  }
  virtual ~DnsHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<Dns> ();
  }
};

} // namespace dns
} // namespace rapidnet
} // namespace ns3

#endif // DNS_HELPER_H

