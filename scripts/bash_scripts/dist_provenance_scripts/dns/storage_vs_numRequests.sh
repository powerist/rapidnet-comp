# Aim : Experiment to find how storage changes wrt changes in number of URL selected from the zipfian distribution

#!/bin/bash
# Argument 1 : Driver File Name
# Argument 2 : Storage Directory
# Argument 3 : Depth of the DNS Tree
# Argument 4 : Fanout 
# Argument 5 : Output File for results
# Argument 6 : Number of nodes in the DNS tree
# Argument 7 : Mode of Experiment (online, rmitm , nocomp)
# Argument 8 : Lower limit of Experiment
# Argument 9 : Upper limit of Experiment
# Argument 10 : Increment Size
# Argument 11 : Stop Time of the Experiment

DIRECTORY="$2/*"

outputFile="$5"

rm -f outputFile
rm -f $2/../timer.log

for (( interval="$8";interval<="$9";interval+="${10}"))
do
    rm -f $DIRECTORY
    # Run the experiment
    echo "Interval $interval starts at $(date +"%T")" >> "$2/../timer.log"
    nohup ./waf --run "$1 --storePth=$2/ --depth=$3 --fanout=$4 --stopTime=${11} --numNodes=$6 --type=$7 --numURL=$interval"
    echo "Interval $interval ends at $(date +"%T")" >> "$2/../timer.log"
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
