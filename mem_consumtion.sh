while true
do
MEM=$(top -b -n1 | grep "$1" | head -1 | awk '{print $10}')
if [ -z "$MEM" ]; then
  break
else
    printf "%s\t%s\n" "$(date +"%T")" "$MEM" >> "$2"
fi
sleep $3
done