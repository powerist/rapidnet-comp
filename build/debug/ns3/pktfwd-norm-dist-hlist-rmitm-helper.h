#ifndef PKTFWDNORMDISTHLISTRMITM_HELPER_H
#define PKTFWDNORMDISTHLISTRMITM_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-dist-hlist-rmitm.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormdisthlistrmitm {

class PktfwdNormDistHlistRmitm;

class PktfwdNormDistHlistRmitmHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormDistHlistRmitmHelper ()
  {
    m_factory.SetTypeId (PktfwdNormDistHlistRmitm::GetTypeId ());
  }
  virtual ~PktfwdNormDistHlistRmitmHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormDistHlistRmitm> ();
  }
};

} // namespace pktfwdnormdisthlistrmitm
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMDISTHLISTRMITM_HELPER_H

