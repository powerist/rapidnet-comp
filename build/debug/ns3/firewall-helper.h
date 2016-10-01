#ifndef FIREWALL_HELPER_H
#define FIREWALL_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "firewall.h"

namespace ns3 {
namespace rapidnet {
namespace firewall {

class Firewall;

class FirewallHelper: public RapidNetApplicationHelper
{
public:
  FirewallHelper ()
  {
    m_factory.SetTypeId (Firewall::GetTypeId ());
  }
  virtual ~FirewallHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<Firewall> ();
  }
};

} // namespace firewall
} // namespace rapidnet
} // namespace ns3

#endif // FIREWALL_HELPER_H

