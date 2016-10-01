#ifndef SDNLOADBALANCING_HELPER_H
#define SDNLOADBALANCING_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "sdn-load-balancing.h"

namespace ns3 {
namespace rapidnet {
namespace sdnloadbalancing {

class SdnLoadBalancing;

class SdnLoadBalancingHelper: public RapidNetApplicationHelper
{
public:
  SdnLoadBalancingHelper ()
  {
    m_factory.SetTypeId (SdnLoadBalancing::GetTypeId ());
  }
  virtual ~SdnLoadBalancingHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<SdnLoadBalancing> ();
  }
};

} // namespace sdnloadbalancing
} // namespace rapidnet
} // namespace ns3

#endif // SDNLOADBALANCING_HELPER_H

