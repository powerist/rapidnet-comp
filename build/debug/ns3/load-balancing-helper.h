#ifndef LOADBALANCING_HELPER_H
#define LOADBALANCING_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "load-balancing.h"

namespace ns3 {
namespace rapidnet {
namespace loadbalancing {

class LoadBalancing;

class LoadBalancingHelper: public RapidNetApplicationHelper
{
public:
  LoadBalancingHelper ()
  {
    m_factory.SetTypeId (LoadBalancing::GetTypeId ());
  }
  virtual ~LoadBalancingHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<LoadBalancing> ();
  }
};

} // namespace loadbalancing
} // namespace rapidnet
} // namespace ns3

#endif // LOADBALANCING_HELPER_H

