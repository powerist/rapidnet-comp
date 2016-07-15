while true
do
PID=$(top -b -n1 | grep "$1" | head -1 | awk '{print $1}')
if [ -z "$PID" ]; then
  break
else
    printf "$(kill $PID)"
fi
sleep 2
done