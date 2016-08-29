#ifndef PKTFWDNORMBDFAIRPROV_HELPER_H
#define PKTFWDNORMBDFAIRPROV_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-bdfair-prov.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormbdfairprov {

class PktfwdNormBdfairProv;

class PktfwdNormBdfairProvHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormBdfairProvHelper ()
  {
    m_factory.SetTypeId (PktfwdNormBdfairProv::GetTypeId ());
  }
  virtual ~PktfwdNormBdfairProvHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormBdfairProv> ();
  }
};

} // namespace pktfwdnormbdfairprov
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMBDFAIRPROV_HELPER_H

