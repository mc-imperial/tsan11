#!/bin/bash

ITER=1000
RESULTS=()
RES_COUNT=()

for i in `seq 1 1 $ITER` ; do
  OUTPUT=$(./$1)

  # Find it in result array
  for idx in `seq 0 1 ${#RESULTS[@]}` ; do
    if [[ "$OUTPUT" == "${RESULTS[$idx]}" ]] ; then
      break
    fi
  done

  # If not found, add
  if [[ $idx -eq ${#RESULTS[@]} ]] ; then
    RESULTS[$idx]="$OUTPUT"
    RES_COUNT[$idx]=0
  fi
  ((RES_COUNT[$idx]++))
done

# Print everything
((LEN=${#RESULTS[@]}-1))
for i in `seq 0 1 $LEN` ; do
  echo "---------------------- Seen: ${RES_COUNT[$i]}"
  echo "${RESULTS[$i]}"
done
