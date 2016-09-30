#ifndef PKTFWDNORMPROV_HELPER_H
#define PKTFWDNORMPROV_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-prov.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormprov {

class PktfwdNormProv;

class PktfwdNormProvHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormProvHelper ()
  {
    m_factory.SetTypeId (PktfwdNormProv::GetTypeId ());
  }
  virtual ~PktfwdNormProvHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormProv> ();
  }
};

} // namespace pktfwdnormprov
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMPROV_HELPER_H

