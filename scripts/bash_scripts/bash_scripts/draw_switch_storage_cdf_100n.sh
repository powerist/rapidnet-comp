#!/bin/bash

inFile="/localdrive1/chen/calc_prov_storage/storage_distribution.dat"
outFile="/localdrive1/chen/calc_prov_storage/switch_storage_cdf_100n.dat"

# Collect sizes of all switch provenance tables and store in fileSizes
count=0
while read line;do
    tokens=( $line )
    if [ ${tokens[0]} -lt 101 ]
    then
	fileSizes[$count]=${tokens[1]}
	((count++))
    fi
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
