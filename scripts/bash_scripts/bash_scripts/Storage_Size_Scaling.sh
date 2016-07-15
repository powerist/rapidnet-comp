DIRECTORY="$1/*"
outputFile=$2

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
printf "%s \t %s\n" "$3" "$avg">> "$outputFile"
rm -f $DIRECTORY