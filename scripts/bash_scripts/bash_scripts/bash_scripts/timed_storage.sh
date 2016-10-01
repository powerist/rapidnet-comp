#!/bin/bash
# Argument 1: name of the test file
# Argument 2: number of pairs
# Argument 3: number of packets
# Argument 4: short name for the experiment, e.g., nocomp

# Default experiment directory
exprDir=/home/chen/research/rapidnet-comp

# Directory for serialized files
DIRECTORY=/localdrive1/chen/prov_storage/*
storeDir=/localdrive1/chen/prov_storage
mkdir -p $storeDir

# The file contains the final results, i.e.
# the storage sequences with elapsed time
outputFile="/localdrive1/chen/calc_prov_storage/timed_storage_100n_"$2"prs_"$3"pkts_"$4".dat"

# Directory for storage snapshot
snapshotDirPfx=/localdrive1/chen/raw_prov_storage_data/
# Prefix for files for storage snapshot
snapshotPrefix="time_slice_storage_100n_"
snapshotSuffix=".dat"
snapshotFilePfx="$snapshotDirPfx""$snapshotPrefix"$2"prs_"$3"pkts_"$4"_"


# Whenever the experiment finishes, do three things:
# Record the storage data of all serialized files
# - Compute the average storage based on the collected data
#     - This step could be done along with the first step
# - Output the computed data into a specific file for graphing

rm -f $outputFile
touch $outputFile
lastAvg=0 # Average storage of the last experiment
for exprTime in {5..500..5}
do
    rm -f $DIRECTORY
    cd $exprDir
    # Run the experiment
    ./waf --run "$1 --hostPairs=$2 --packetNum=$3 --finishTime=$exprTime"
    snapshotFileName="$snapshotFilePfx""$exprTime""$snapshotSuffix"
    rm -f $snapshotFileName
    touch $snapshotFileName
    # Record the storage data of all serialized files
    # At the same time, compute the average storage
    count=0
    sum=0
    cd $storeDir
    allFileNames=$(ls *)
    for file in $allFileNames
    do
	fsize=$(stat --printf="%s" "$file")
	fileSizes[$count]=$fsize
	((sum+=fsize))
	((count+=1))
	printf "%s %s \n" "$file" "$fsize" >> "$snapshotFileName"	
    done
    fileNum=$count
    if [ $fileNum -eq 0 ]; then
	avg=0
    else
	avg=$((sum/fileNum))	
    fi
    if [ "$avg" = "$lastAvg" ]; then
	if [ ! $avg -eq 0 ]; then
	    break
	fi
    fi
    lastAvg=$avg
    printf "%s \t %s\n" "$exprTime" "$avg">> "$outputFile"
done
