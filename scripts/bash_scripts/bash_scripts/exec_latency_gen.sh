#!/bin/bash
# Argument 1: name of the test file
# Argument 2: packet number
# Argument 3: name of the output file
# Argument 4: prov file directory
# Argument 5: Number of hops
# Argument 6: size of data
# Argument 7: log file

DIRECTORY="$4/*"

outputFile=$3

rm -f outputFile
for pairNum in {0..500..10}
do
    rm -f $DIRECTORY
    # Run the experiment
    ./waf --run "$1 --hostPairs=$pairNum --packetNum=$2 --storePath=$4/ --pathLength=$5 --dataSize=$6" > $7 2>&1
    execTime="$(python latency_pktfwd.py $7 initPacket[ Serialization)"
    printf "%s\t%s\n" "$pairNum" "$execTime" >> "$outputFile"
done
