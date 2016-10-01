#ifndef PKTFWDNORMDISTHLISTONLINE_HELPER_H
#define PKTFWDNORMDISTHLISTONLINE_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-dist-hlist-online.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormdisthlistonline {

class PktfwdNormDistHlistOnline;

class PktfwdNormDistHlistOnlineHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormDistHlistOnlineHelper ()
  {
    m_factory.SetTypeId (PktfwdNormDistHlistOnline::GetTypeId ());
  }
  virtual ~PktfwdNormDistHlistOnlineHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormDistHlistOnline> ();
  }
};

} // namespace pktfwdnormdisthlistonline
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMDISTHLISTONLINE_HELPER_H

