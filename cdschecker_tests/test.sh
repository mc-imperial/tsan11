#!/bin/bash

EXE=$1
TOTAL_RUN=1000

COUNT_DATA_RACE=0
COUNT_TIME=0

for i in `seq 1 1 $TOTAL_RUN` ; do
  OUTPUT="$(/usr/bin/time -f "time: %U %S" $EXE 2>&1)"
  RACE="$(echo "$OUTPUT" | grep "data race")"
  if [ -n "$RACE" ] ; then
    ((++COUNT_DATA_RACE))
  fi
  TIME="$(echo "$OUTPUT" | grep -o "time: .\... .\...")"
  TIME_USER_S="$(echo "$TIME" | cut -d' ' -f2 | cut -d'.' -f1)"
  TIME_USER_CS="$(echo "$TIME" | cut -d' ' -f2 | cut -d'.' -f2)"
  TIME_SYSTEM_S="$(echo "$TIME" | cut -d' ' -f3 | cut -d'.' -f1)"
  TIME_SYSTEM_CS="$(echo "$TIME" | cut -d' ' -f3 | cut -d'.' -f2)"
  TIME_EXE=$((10#$TIME_USER_S * 1000 + 10#$TIME_USER_CS * 10 + 10#$TIME_SYSTEM_S * 1000 + 10#$TIME_SYSTEM_CS * 10))
  COUNT_TIME=$((COUNT_TIME + TIME_EXE))
done

AVG_DATA_RACE=$((COUNT_DATA_RACE * 100 / TOTAL_RUN))
AVG_TIME=$((COUNT_TIME / TOTAL_RUN))

echo "Runs: $TOTAL_RUN Data races: $COUNT_DATA_RACE Time: ${COUNT_TIME}ms"
echo "Data race rate: ${AVG_DATA_RACE}% Avg time: ${AVG_TIME}ms"
