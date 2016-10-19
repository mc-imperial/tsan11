#!/bin/bash
set -e
set -u

FILES=(*.cpp)

for t in "${FILES[@]}"; do
  f="${t%%.cpp}"
  echo -e "\n\nTEST: ${f}"
  ./run.sh "${f}"
done


