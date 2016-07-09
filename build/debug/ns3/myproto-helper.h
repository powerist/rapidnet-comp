#ifndef MYPROTO_HELPER_H
#define MYPROTO_HELPER_H

#include "ns3/rapidnet-application-helper.h"
#include "myproto.h"

namespace ns3 {
namespace rapidnet {
namespace myproto {

class Myproto;

class MyprotoHelper: public RapidNetApplicationHelper
{
public:
  MyprotoHelper ()
  {
    m_factory.SetTypeId (Myproto::GetTypeId ());
  }
  virtual ~MyprotoHelper ()
  {
  }

protected:
  Ptr<RapidNetApplicationBase> CreateNewApplication ()
  {
    return m_factory.Create<Myproto> ();
  }
};

} // namespace myproto
} // namespace rapidnet
} // namespace ns3

#endif // MYPROTO_HELPER_H

