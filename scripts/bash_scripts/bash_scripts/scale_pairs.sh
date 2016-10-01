#!/bin/bash
# Argument 1: name of the test file
# Argument 2: packet number
# Argument 3: name of the output file

DIRECTORY=/localdrive1/chen/prov_storage/*
storeDir=/localdrive1/chen/prov_storage
mkdir -p $storeDir

outputFile=/localdrive1/chen/calc_prov_storage/$3
rm -f $outputFile
for pairNum in {500..500..100}
do
    rm -f $DIRECTORY
    # Run the experiment
    ./waf --run "$1 --hostPairs=$pairNum --packetNum=$2"
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
    avg=$((sum/fileNum))
    printf "%s \t %s\n" "$pairNum" "$avg">> "$outputFile"
done
