#ifndef PKTFWDSDNPROVCOMPSTATICCHECK_HELPER_H
#define PKTFWDSDNPROVCOMPSTATICCHECK_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-sdn-prov-comp-static-check.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdsdnprovcompstaticcheck {

class PktfwdSdnProvCompStaticCheck;

class PktfwdSdnProvCompStaticCheckHelper: public RapidNetApplicationHelper
{
public:
  PktfwdSdnProvCompStaticCheckHelper ()
  {
    m_factory.SetTypeId (PktfwdSdnProvCompStaticCheck::GetTypeId ());
  }
  virtual ~PktfwdSdnProvCompStaticCheckHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdSdnProvCompStaticCheck> ();
  }
};

} // namespace pktfwdsdnprovcompstaticcheck
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDSDNPROVCOMPSTATICCHECK_HELPER_H

