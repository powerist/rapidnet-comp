#ifndef PKTFWDNORMPROVCOMPONLINE_HELPER_H
#define PKTFWDNORMPROVCOMPONLINE_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-prov-comp-online.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormprovcomponline {

class PktfwdNormProvCompOnline;

class PktfwdNormProvCompOnlineHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormProvCompOnlineHelper ()
  {
    m_factory.SetTypeId (PktfwdNormProvCompOnline::GetTypeId ());
  }
  virtual ~PktfwdNormProvCompOnlineHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormProvCompOnline> ();
  }
};

} // namespace pktfwdnormprovcomponline
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMPROVCOMPONLINE_HELPER_H

