#!/bin/bash

DIRECTORY="$2/*"

outputFile="$5"

rm -f outputFile
rm -f $2/../timer.log

for (( interval="$6";interval<="$7";interval+="$8"))
do
    rm -f $DIRECTORY
    # Run the experiment
    echo "Interval $interval starts at $(date +"%T")" >> "$2/../timer.log"
    nohup ./waf --run "$1 --storePth=$2/ --depth=$3 --fanout=$4 --numNodes=100 --numRequests=$interval"
    echo "Interval $interval ends at $(date +"%T")" >> "$2/../timer.log"
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
