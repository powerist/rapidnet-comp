#ifndef PKTFWDSDN_HELPER_H
#define PKTFWDSDN_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-sdn.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdsdn {

class PktfwdSdn;

class PktfwdSdnHelper: public RapidNetApplicationHelper
{
public:
  PktfwdSdnHelper ()
  {
    m_factory.SetTypeId (PktfwdSdn::GetTypeId ());
  }
  virtual ~PktfwdSdnHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdSdn> ();
  }
};

} // namespace pktfwdsdn
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDSDN_HELPER_H

