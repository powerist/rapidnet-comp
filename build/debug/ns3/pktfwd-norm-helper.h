#ifndef PKTFWDNORM_HELPER_H
#define PKTFWDNORM_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnorm {

class PktfwdNorm;

class PktfwdNormHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormHelper ()
  {
    m_factory.SetTypeId (PktfwdNorm::GetTypeId ());
  }
  virtual ~PktfwdNormHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNorm> ();
  }
};

} // namespace pktfwdnorm
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORM_HELPER_H

