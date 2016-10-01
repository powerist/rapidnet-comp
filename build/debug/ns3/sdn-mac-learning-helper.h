#ifndef SDNMACLEARNING_HELPER_H
#define SDNMACLEARNING_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "sdn-mac-learning.h"

namespace ns3 {
namespace rapidnet {
namespace sdnmaclearning {

class SdnMacLearning;

class SdnMacLearningHelper: public RapidNetApplicationHelper
{
public:
  SdnMacLearningHelper ()
  {
    m_factory.SetTypeId (SdnMacLearning::GetTypeId ());
  }
  virtual ~SdnMacLearningHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<SdnMacLearning> ();
  }
};

} // namespace sdnmaclearning
} // namespace rapidnet
} // namespace ns3

#endif // SDNMACLEARNING_HELPER_H

