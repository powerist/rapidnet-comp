#ifndef PKTFWDNORMNODEVICE_HELPER_H
#define PKTFWDNORMNODEVICE_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "pktfwd-norm-no-device.h"

namespace ns3 {
namespace rapidnet {
namespace pktfwdnormnodevice {

class PktfwdNormNoDevice;

class PktfwdNormNoDeviceHelper: public RapidNetApplicationHelper
{
public:
  PktfwdNormNoDeviceHelper ()
  {
    m_factory.SetTypeId (PktfwdNormNoDevice::GetTypeId ());
  }
  virtual ~PktfwdNormNoDeviceHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<PktfwdNormNoDevice> ();
  }
};

} // namespace pktfwdnormnodevice
} // namespace rapidnet
} // namespace ns3

#endif // PKTFWDNORMNODEVICE_HELPER_H

