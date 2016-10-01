# Aim : To measure the increase in storage wrt increasing number of communicating host pairs with constant number of packets in the network

#!/bin/bash
# Argument 1: name of the test file
# Argument 2: stopTime of the Experiment
# Argument 3: name of the output file
# Argument 4: store path for the output of the experiment
# Argument 5: path Length
# Argument 6: Size of the payload
# Argument 7: Number of packets to be sent
# Argumwnr 8: Lower limit for experiment 
# Argument 9: Upper Limit for experiment
# Argument 10: Interval length for the experiment
# Argument 11: Mode of the experiment online, rmitm and nocomp

DIRECTORY="$4/*"

outputFile="$3"

rm -f outputFile
rm -f $4/../timer.log

totalPackets=$7
for (( interval="$8";interval<="$9";interval+="${10}"))
do
    rm -f $DIRECTORY
    numPackets=$((totalPackets/interval))
    # Run the experiment
    echo $numPackets
    echo "Interval $interval starts at $(date +"%T")" >> "$4/../timer.log"
    nohup ./waf --run "$1 --hostPairs=$interval --packetNum=$numPackets --storePath=$4/ --pathLength=$5 --dataSize=$6 --stopTime=$2 --mode=${11}"
    echo "Interval $interval ends at $(date +"%T")" >> "$4/../timer.log"
    # Calculate the average size of all files
    count=0
    sum=0
    for file in $DIRECTORY
    do
	fsize=$(stat --printf="%s" "$file")
	((sum+=fsize))
	((count+=1))
    done
    fileNum=$count
    avg=$((sum))
    #/fileNum))
    printf "%s \t %s\n" "$interval" "$avg">> "$outputFile"
done
