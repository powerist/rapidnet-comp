#ifndef PKTFWDNORMBDFAIRPROVCOMPSTRAWMAN_HELPER_H
#define PKTFWDNORMBDFAIRPROVCOMPSTRAWMAN_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-bdfair-prov-comp-strawman.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormbdfairprovcompstrawman {

class PktfwdNormBdfairProvCompStrawman;

class PktfwdNormBdfairProvCompStrawmanHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormBdfairProvCompStrawmanHelper ()
  {
    m_factory.SetTypeId (PktfwdNormBdfairProvCompStrawman::GetTypeId ());
  }
  virtual ~PktfwdNormBdfairProvCompStrawmanHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormBdfairProvCompStrawman> ();
  }
};

} // namespace pktfwdnormbdfairprovcompstrawman
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMBDFAIRPROVCOMPSTRAWMAN_HELPER_H

