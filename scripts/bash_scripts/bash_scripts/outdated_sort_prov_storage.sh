#!/bin/bash

DIRECTORY="/localdrive1/chen/prov_storage/*"
storeDir="/localdrive1/chen/prov_storage"
outputFile="/localdrive1/chen/calc_prov_storage/storage_cdf.dat"
mkdir -p $storeDir

count=0
sum=0
# Collect sizes of all files
for file in $DIRECTORY
do
    fsize=$(stat --printf="%s" "$file")
    fileSizes[$count]=$fsize
    ((sum+=fsize))
    ((count+=1))
done
fileNum=$count
avg=$((sum/fileNum))
avgFile="storage_avg.dat"
rm -f $avgFile
echo $avg >> "$avgFile"

# Sort the size array
sortRes=( $(printf "%s\n" "${fileSizes[@]}" | sort -g))

# Create the cumulative result
rm -f $outputFile
for ((i=0;i<fileNum;i++))
do
    fileIdx=$((i+1))
    percentage=$(($fileIdx*100/$fileNum))
    printf "%s %s \n" "${sortRes[$i]}" "$percentage" >> "$outputFile"
done

rm -f $DIRECTORY
