#ifndef PKTFWDNORMPROVCOMPSTRAWMAN_HELPER_H
#define PKTFWDNORMPROVCOMPSTRAWMAN_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-prov-comp-strawman.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormprovcompstrawman {

class PktfwdNormProvCompStrawman;

class PktfwdNormProvCompStrawmanHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormProvCompStrawmanHelper ()
  {
    m_factory.SetTypeId (PktfwdNormProvCompStrawman::GetTypeId ());
  }
  virtual ~PktfwdNormProvCompStrawmanHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormProvCompStrawman> ();
  }
};

} // namespace pktfwdnormprovcompstrawman
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMPROVCOMPSTRAWMAN_HELPER_H

