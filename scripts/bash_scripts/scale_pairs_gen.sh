#!/bin/bash
# Argument 1: name of the test file
# Argument 2: packet number
# Argument 3: name of the output file
# Argument 4: prov file directory


DIRECTORY="$4/*"

outputFile=$3

rm -f outputFile
for pairNum in {0..500..50}
do
    rm -f $DIRECTORY
    # Run the experiment
    #nohup ./waf --run "$1 --hostPairs=$pairNum --packetNum=$2 --storePath=$4/ --pathLength=$5 --dataSize=$6"
    nohup ./waf --run "$1 --hostPairs=$pairNum --packetNum=$2 --storePath=$4/"
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
    printf "%s \t %s\n" "$pairNum" "$avg">> "$outputFile"
done
