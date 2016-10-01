#!/bin/bash

inFile="$1/storage_distribution.dat"
outFile="$1/total_storage_cdf_100n.dat"

# Collect sizes of all provenance tables and store in fileSizes
count=0
while read line;do
    tokens=( $line )
    fileSizes[$count]=${tokens[1]}
    ((count++))
done < $inFile
fileNum=$count

# Sort the size array
sortRes=( $(printf "%s\n" "${fileSizes[@]}" | sort -g))

# Create the cumulative result
rm -f $outFile
touch $outFile
for ((i=0;i<fileNum;i++))
do
    fileIdx=$((i+1))
    percentage=$(($fileIdx*100/$fileNum))
    printf "%s %s \n" "${sortRes[$i]}" "$percentage" >> "$outFile"
done
