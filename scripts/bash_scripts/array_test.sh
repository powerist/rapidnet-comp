declare -a previousStorage=($(for i in {1..100}; do echo 0; done))
declare -a avgRate=($(for i in {1..100}; do echo 0; done))
interval=1
for file in $1/*
do
    baseName="$(basename $file)"
    fsize=$(stat --printf="%s" "$file")
    currentRate=$((($fsize-${previousStorage[$(($baseName-1))]})/$interval))
    avgRate[$(($baseName-1))]=$((${avgRate[$(($baseName-1))]}+$currentRate))
done
for i in {1..100}
do
    printf "%s \t %s\n" "$i" "$((${avgRate[$(($i-1))]}/20))"
done