#!/bin/bash
set -e
set -u

# Paul: skip `spsc-queue` as it deadlocks.

for t in barrier chase-lev-deque dekker-fences linuxrwlocks mcs-lock mpmc-queue ms-queue; do
  cd $t
  echo -n "$t " 
  ../test.sh ./$t
  cd ..
done

