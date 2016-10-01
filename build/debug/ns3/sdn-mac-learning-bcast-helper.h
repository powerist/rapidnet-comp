#ifndef SDNMACLEARNINGBCAST_HELPER_H
#define SDNMACLEARNINGBCAST_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "sdn-mac-learning-bcast.h"

namespace ns3 {
namespace rapidnet {
namespace sdnmaclearningbcast {

class SdnMacLearningBcast;

class SdnMacLearningBcastHelper: public RapidNetApplicationHelper
{
public:
  SdnMacLearningBcastHelper ()
  {
    m_factory.SetTypeId (SdnMacLearningBcast::GetTypeId ());
  }
  virtual ~SdnMacLearningBcastHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<SdnMacLearningBcast> ();
  }
};

} // namespace sdnmaclearningbcast
} // namespace rapidnet
} // namespace ns3

#endif // SDNMACLEARNINGBCAST_HELPER_H

