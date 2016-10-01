#ifndef PKTFWDNORMBDFAIRPROVCOMPONLINE_HELPER_H
#define PKTFWDNORMBDFAIRPROVCOMPONLINE_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-bdfair-prov-comp-online.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormbdfairprovcomponline {

class PktfwdNormBdfairProvCompOnline;

class PktfwdNormBdfairProvCompOnlineHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormBdfairProvCompOnlineHelper ()
  {
    m_factory.SetTypeId (PktfwdNormBdfairProvCompOnline::GetTypeId ());
  }
  virtual ~PktfwdNormBdfairProvCompOnlineHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormBdfairProvCompOnline> ();
  }
};

} // namespace pktfwdnormbdfairprovcomponline
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMBDFAIRPROVCOMPONLINE_HELPER_H

