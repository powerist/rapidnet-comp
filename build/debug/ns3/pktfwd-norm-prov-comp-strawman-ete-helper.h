#ifndef PKTFWDNORMPROVCOMPSTRAWMANETE_HELPER_H
#define PKTFWDNORMPROVCOMPSTRAWMANETE_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-prov-comp-strawman-ete.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormprovcompstrawmanete {

class PktfwdNormProvCompStrawmanEte;

class PktfwdNormProvCompStrawmanEteHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormProvCompStrawmanEteHelper ()
  {
    m_factory.SetTypeId (PktfwdNormProvCompStrawmanEte::GetTypeId ());
  }
  virtual ~PktfwdNormProvCompStrawmanEteHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormProvCompStrawmanEte> ();
  }
};

} // namespace pktfwdnormprovcompstrawmanete
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMPROVCOMPSTRAWMANETE_HELPER_H

