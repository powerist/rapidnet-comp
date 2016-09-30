#!/bin/bash
#1;2c Argument 1: name of the test file
# Argument 2: packet number
# Argument 3: name of the output file
# Argument 4: store path for the output of the experiment
# Argument 5: path Length

DIRECTORY="$4/*"

outputFile="$3"

rm -f outputFile
declare -a previousStorage=( $(for i in {1..100}; do echo 0; done) )
declare -a avgRate=( $(for i in {1..100}; do echo 0; done) )
intervalTime=10
#printf "%s \t %s\n" "0" "0">> "$outputFile"
for interval in {1..100..10}
do
    rm -f $DIRECTORY
    # Run the experiment
    echo "$interval" >> "$4/../timer.log" 
    ./waf --run "$1 --hostPairs=$2 --packetNum=$7 --storePath=$4/ --pathLength=$5 --dataSize=$6 --stopTime=$interval" > /dev/null 2>&1
    
    for file in $DIRECTORY
    do
	baseName="$(basename $file)"
	fsize=$(stat --printf="%s" "$file")
	currentRate=$((($fsize-${previousStorage[$(($baseName-1))]})/$intervalTime))
	avgRate[$(($baseName-1))]=$((${avgRate[$(($baseName-1))]}+$currentRate))
    done
done
for i in {1..100}
do
    printf "%s \t %s\n" "$i" "$((${avgRate[$(($i-1))]}/10))">> "$outputFile"
done