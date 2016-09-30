#ifndef PKTFWDSDNPROV_HELPER_H
#define PKTFWDSDNPROV_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-sdn-prov.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdsdnprov {

class PktfwdSdnProv;

class PktfwdSdnProvHelper: public RapidNetApplicationHelper
{
public:
  PktfwdSdnProvHelper ()
  {
    m_factory.SetTypeId (PktfwdSdnProv::GetTypeId ());
  }
  virtual ~PktfwdSdnProvHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdSdnProv> ();
  }
};

} // namespace pktfwdsdnprov
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDSDNPROV_HELPER_H

