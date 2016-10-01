set terminal pdf

set output "storage_cdf.pdf"

set title "100-node topology with 500 pairs of hosts, each sending 1000 packets"

set xlabel "Storage Size (GB)"
set ylabel "Percentage of nodes (%)"
set logscale x 10

set key right bottom

plot "/localdrive1/harshal/pktfwd_storage_nocomp/total_storage_cdf_100n.dat" using ($1/1000000):2 title "no compression" with lines,\
    "/localdrive1/harshal/pktfwd_storage_online/total_storage_cdf_100n.dat" using ($1/1000000):2 title "online compression" with lines,\
    "/localdrive1/harshal/pktfwd_storage_rmitm/total_storage_cdf_100n.dat" using ($1/1000000):2 title "rmitm compression" with lines
