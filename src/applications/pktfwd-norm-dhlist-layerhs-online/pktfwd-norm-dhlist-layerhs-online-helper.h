#ifndef PKTFWDNORMDHLISTLAYERHSONLINE_HELPER_H
#define PKTFWDNORMDHLISTLAYERHSONLINE_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-dhlist-layerhs-online.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormdhlistlayerhsonline {

class PktfwdNormDhlistLayerhsOnline;

class PktfwdNormDhlistLayerhsOnlineHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormDhlistLayerhsOnlineHelper ()
  {
    m_factory.SetTypeId (PktfwdNormDhlistLayerhsOnline::GetTypeId ());
  }
  virtual ~PktfwdNormDhlistLayerhsOnlineHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormDhlistLayerhsOnline> ();
  }
};

} // namespace pktfwdnormdhlistlayerhsonline
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMDHLISTLAYERHSONLINE_HELPER_H

