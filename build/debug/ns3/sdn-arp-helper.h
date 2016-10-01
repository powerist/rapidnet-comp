#ifndef SDNARP_HELPER_H
#define SDNARP_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "sdn-arp.h"

namespace ns3 {
namespace rapidnet {
namespace sdnarp {

class SdnArp;

class SdnArpHelper: public RapidNetApplicationHelper
{
public:
  SdnArpHelper ()
  {
    m_factory.SetTypeId (SdnArp::GetTypeId ());
  }
  virtual ~SdnArpHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<SdnArp> ();
  }
};

} // namespace sdnarp
} // namespace rapidnet
} // namespace ns3

#endif // SDNARP_HELPER_H

