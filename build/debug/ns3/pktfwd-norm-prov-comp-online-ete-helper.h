#ifndef PKTFWDNORMPROVCOMPONLINEETE_HELPER_H
#define PKTFWDNORMPROVCOMPONLINEETE_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-prov-comp-online-ete.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormprovcomponlineete {

class PktfwdNormProvCompOnlineEte;

class PktfwdNormProvCompOnlineEteHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormProvCompOnlineEteHelper ()
  {
    m_factory.SetTypeId (PktfwdNormProvCompOnlineEte::GetTypeId ());
  }
  virtual ~PktfwdNormProvCompOnlineEteHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormProvCompOnlineEte> ();
  }
};

} // namespace pktfwdnormprovcomponlineete
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMPROVCOMPONLINEETE_HELPER_H

