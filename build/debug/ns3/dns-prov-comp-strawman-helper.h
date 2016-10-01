#ifndef DNSPROVCOMPSTRAWMAN_HELPER_H
#define DNSPROVCOMPSTRAWMAN_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "dns-prov-comp-strawman.h"

namespace ns3 {
namespace rapidnet {
namespace dnsprovcompstrawman {

class DnsProvCompStrawman;

class DnsProvCompStrawmanHelper: public RapidNetApplicationHelper
{
public:
  DnsProvCompStrawmanHelper ()
  {
    m_factory.SetTypeId (DnsProvCompStrawman::GetTypeId ());
  }
  virtual ~DnsProvCompStrawmanHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<DnsProvCompStrawman> ();
  }
};

} // namespace dnsprovcompstrawman
} // namespace rapidnet
} // namespace ns3

#endif // DNSPROVCOMPSTRAWMAN_HELPER_H

