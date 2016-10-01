#ifndef PKTFWDSDNPROVCOMP_HELPER_H
#define PKTFWDSDNPROVCOMP_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-sdn-prov-comp.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdsdnprovcomp {

class PktfwdSdnProvComp;

class PktfwdSdnProvCompHelper: public RapidNetApplicationHelper
{
public:
  PktfwdSdnProvCompHelper ()
  {
    m_factory.SetTypeId (PktfwdSdnProvComp::GetTypeId ());
  }
  virtual ~PktfwdSdnProvCompHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdSdnProvComp> ();
  }
};

} // namespace pktfwdsdnprovcomp
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDSDNPROVCOMP_HELPER_H

