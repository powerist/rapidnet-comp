#ifndef PKTFWDNORMBDFAIR_HELPER_H
#define PKTFWDNORMBDFAIR_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-bdfair.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormbdfair {

class PktfwdNormBdfair;

class PktfwdNormBdfairHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormBdfairHelper ()
  {
    m_factory.SetTypeId (PktfwdNormBdfair::GetTypeId ());
  }
  virtual ~PktfwdNormBdfairHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormBdfair> ();
  }
};

} // namespace pktfwdnormbdfair
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMBDFAIR_HELPER_H

