#!/bin/bash

DATAVIZ_WINDOW_INDEX=0
MY_AGE=$((RANDOM % 100))
MY_ANSWER=$((RANDOM % 100))
ALL_VALUES=() # array

for i in {1..20};
do
  ALL_VALUES+=($((RANDOM % 100)))
done

echo ${ALL_VALUES[@]}

set -o verbose

oscsend osc.udp://localhost:31337 /dataviz/${DATAVIZ_WINDOW_INDEX}/view_answer_by_age iiiiiiiiiiiiiiiiiiiiii \
  $MY_ANSWER \
  $MY_AGE \
  ${ALL_VALUES[@]}

