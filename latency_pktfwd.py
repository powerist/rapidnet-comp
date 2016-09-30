import sys


#First argument is the log file location
#Second argument is the first rule to start the time count from

logFile = open(sys.argv[1])
initRule = sys.argv[2]
endStatement = sys.argv[3]

lastRuleTime = "0"

initTime = "0"

initTimeFound = False

def extractTime(line):
    timeEntry = line.split()[0]
    time = timeEntry[1:-3]
    return time

for line in logFile:
    if len(line.strip()) == 0:
        continue

    if endStatement in line:
        #print 'LAST RULE TIME',lastRuleTime
        break

    if initRule in line and not initTimeFound:
        #Start counting the timer
        initTimeFound = True
        initTime = extractTime(line)
        #print 'INIT TIME',initTime

    if initTimeFound:
        lastRuleTime = extractTime(line)

print int(lastRuleTime)-int(initTime)
logFile.close()