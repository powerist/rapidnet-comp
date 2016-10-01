#ifndef ARP_HELPER_H
#define ARP_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "arp.h"

namespace ns3 {
namespace rapidnet {
namespace arp {

class Arp;

class ArpHelper: public RapidNetApplicationHelper
{
public:
  ArpHelper ()
  {
    m_factory.SetTypeId (Arp::GetTypeId ());
  }
  virtual ~ArpHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<Arp> ();
  }
};

} // namespace arp
} // namespace rapidnet
} // namespace ns3

#endif // ARP_HELPER_H

