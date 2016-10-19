#!/bin/bash
set -e
set -u

make

for t in barrier chase-lev-deque dekker-fences linuxrwlocks mcs-lock mpmc-queue ms-queue spsc-queue ; do
  cd $t
  echo -n "$t " 
  ../test.sh ./$t
  cd ..
done

