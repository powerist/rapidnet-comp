#ifndef LOADBALANCERLEARNING_HELPER_H
#define LOADBALANCERLEARNING_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "load-balancer-learning.h"

namespace ns3 {
namespace rapidnet {
namespace loadbalancerlearning {

class LoadBalancerLearning;

class LoadBalancerLearningHelper: public RapidNetApplicationHelper
{
public:
  LoadBalancerLearningHelper ()
  {
    m_factory.SetTypeId (LoadBalancerLearning::GetTypeId ());
  }
  virtual ~LoadBalancerLearningHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<LoadBalancerLearning> ();
  }
};

} // namespace loadbalancerlearning
} // namespace rapidnet
} // namespace ns3

#endif // LOADBALANCERLEARNING_HELPER_H

