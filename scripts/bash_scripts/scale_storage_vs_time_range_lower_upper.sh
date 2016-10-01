#!/bin/bash
#1;2c Argument 1: name of the test file
# Argument 2: packet number
# Argument 3: name of the output file
# Argument 4: store path for the output of the experiment
# Argument 5: path Length

DIRECTORY="$4/*"

outputFile="$3"

rm -f outputFile


for interval in {80..100..10}
#( interval=80;interval<=100;interval+=10))
do
    rm -f $DIRECTORY
    # Run the experiment
    echo "Interval $interval starts at $(date +"%T")" >> "$4/../timer.log"
    nohup ./waf --run "$1 --hostPairs=$2 --packetNum=$7 --storePath=$4/ --pathLength=$5 --dataSize=$6 --stopTime=$interval"
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
