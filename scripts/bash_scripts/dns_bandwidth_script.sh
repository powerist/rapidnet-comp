nohup ./waf --run "dns-prov-dhlist-layerhs-online-bandwidth-test --storePth=/localdrive1/harshal/dns/bandwidth/dns_storage_online/ --depth=7 --fanout=2 --numNodes=100 --numRequests=100000"
nohup ./waf --run "dns-prov-dist-hlist-rmitm-bandwidth-test --storePth=/localdrive1/harshal/dns/bandwidth/dns_storage_rmitm/ --depth=7 --fanout=2 --numNodes=100 --numRequests=100000"
nohup ./waf --run "dns-prov-bandwidth-test --storePth=/localdrive1/harshal/dns/bandwidth/dns_storage_nocomp/ --depth=7 --fanout=2 --numNodes=100 --numRequests=100000"
