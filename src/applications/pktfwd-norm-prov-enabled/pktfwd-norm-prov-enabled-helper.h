#ifndef PKTFWDNORMPROVENABLED_HELPER_H
#define PKTFWDNORMPROVENABLED_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-prov-enabled.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormprovenabled {

class PktfwdNormProvEnabled;

class PktfwdNormProvEnabledHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormProvEnabledHelper ()
  {
    m_factory.SetTypeId (PktfwdNormProvEnabled::GetTypeId ());
  }
  virtual ~PktfwdNormProvEnabledHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormProvEnabled> ();
  }
};

} // namespace pktfwdnormprovenabled
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMPROVENABLED_HELPER_H

