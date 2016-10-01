#ifndef PKTFWDNORMPROVETE_HELPER_H
#define PKTFWDNORMPROVETE_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-prov-ete.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormprovete {

class PktfwdNormProvEte;

class PktfwdNormProvEteHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormProvEteHelper ()
  {
    m_factory.SetTypeId (PktfwdNormProvEte::GetTypeId ());
  }
  virtual ~PktfwdNormProvEteHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormProvEte> ();
  }
};

} // namespace pktfwdnormprovete
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMPROVETE_HELPER_H

