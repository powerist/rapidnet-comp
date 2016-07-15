#!/bin/bash
# Argument 1: name of the test file
# Argument 2: packet number
# Argument 3: name of the output file
# Argument 4: prov file directory


DIRECTORY="$4/*"

outputFile=$3

rm -f outputFile

packetNum=$2
for (( pairNum=1;pairNum<="$7";pairNum+="$8"))
do
    rm -f $DIRECTORY
    # Run the experiment
    ./waf --run "$1 --hostPairs=$pairNum --packetNum=$2 --storePath=$4/ --pathLength=$5 --dataSize=$6" > /dev/null 2>&1
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
    avg=$((sum ))
    #/fileNum))
    printf "%s \t %s\n" "$pairNum" "$avg">> "$outputFile"
done
