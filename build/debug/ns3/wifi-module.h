
#ifdef NS3_MODULE_COMPILATION
# error "Do not include ns3 module aggregator headers from other modules; these are meant only for end user scripts."
#endif

#ifndef NS3_MODULE_WIFI
    

// Module headers:
#include "aarf-wifi-manager.h"
#include "adhoc-wifi-mac.h"
#include "amrr-wifi-manager.h"
#include "amsdu-subframe-header.h"
#include "arf-wifi-manager.h"
#include "constant-rate-wifi-manager.h"
#include "dca-txop.h"
#include "edca-txop-n.h"
#include "error-rate-model.h"
#include "ideal-wifi-manager.h"
#include "interference-helper.h"
#include "jakes-propagation-loss-model.h"
#include "msdu-aggregator.h"
#include "nqap-wifi-mac.h"
#include "nqsta-wifi-mac.h"
#include "onoe-wifi-manager.h"
#include "propagation-delay-model.h"
#include "propagation-loss-model.h"
#include "qadhoc-wifi-mac.h"
#include "qap-wifi-mac.h"
#include "qos-tag.h"
#include "qos-utils.h"
#include "qsta-wifi-mac.h"
#include "rraa-wifi-manager.h"
#include "ssid.h"
#include "supported-rates.h"
#include "wifi-channel.h"
#include "wifi-mac-header.h"
#include "wifi-mac.h"
#include "wifi-mode.h"
#include "wifi-net-device.h"
#include "wifi-phy-standard.h"
#include "wifi-phy.h"
#include "wifi-phy.h"
#include "wifi-preamble.h"
#include "wifi-remote-station-manager.h"
#include "yans-error-rate-model.h"
#include "yans-wifi-channel.h"
#include "yans-wifi-phy.h"
#endif
