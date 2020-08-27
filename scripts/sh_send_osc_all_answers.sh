#!/bin/bash

DATAVIZ_WINDOW_INDEX=0
ALL_ANSWERS=() # array

for i in {1..25};
do
  ALL_ANSWERS+=($((RANDOM % 100)))
done

echo ${ALL_ANSWERS[@]}

set -o verbose

oscsend osc.udp://localhost:31337 /dataviz/${DATAVIZ_WINDOW_INDEX}/all_results iiiiiiiiiiiiiiiiiiiiiiiii ${ALL_ANSWERS[@]}
