#!/bin/bash

# This script collects sizes of all provenance tables
# Output file: /localdrive1/chen/calc_prov_storage/storage_distribution.dat

storageFiles="/localdrive1/harshal/prov_test_strawman/*"
storageDir="/localdrive1/harshal/prov_test_strawman"
outputPath="/localdrive1/harshal/calc_prov_storage_strawman"
outputFile="/localdrive1/harshal/calc_prov_storage_strawman/storage_distribution.dat"

mkdir -p $storageDir
mkdir -p $outputPath
rm -f $outputFile
touch $outputFile
# Collect sizes of all files
cd $storageDir
allFileNames=$(ls *)
for file in $allFileNames
do
    fsize=$(stat --printf="%s" "$file")
    printf "%s %s \n" "$file" "$fsize" >> "$outputFile"
done

rm $storageFiles