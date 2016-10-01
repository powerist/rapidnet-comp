echo 'Starting RMITM TEST'
nohup ./waf --run "pktfwd-large-scale-dist-hlist-rmitm-bandwidth-test --hostPairs=$1 --packetNum=$2 --storePath=$3/prov_storage_pktfwd_dist_hlist_rmitm/ --dataSize=$4"
echo 'Starting NOCOMP TEST'
nohup ./waf --run "pktfwd-norm-large-scale-nocomp-ete-bandwidth-test --hostPairs=$1 --packetNum=$2 --storePath=$3/prov_storage_pktfwd_nocomp_ete/ --dataSize=$4"
echo 'Starting ONLINE TEST'
nohup ./waf --run "pktfwd-large-scale-dist-hlist-rmitm-bandwidth-test --hostPairs=$1 --packetNum=$2 --storePath=$3/prov_storage_pktfwd_dist_hlist_rmitm/ --dataSize=$4"