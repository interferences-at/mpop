#!/bin/bash

DATAVIZ_WINDOW_INDEX=0

NUMBER_ONE=$((RANDOM % 50))
NUMBER_TWO=$((RANDOM % 50))
NUMBER_THREE=$((100 - $NUMBER_ONE - $NUMBER_TWO))

echo oscsend osc.udp://localhost:31337 /dataviz/${DATAVIZ_WINDOW_INDEX}/my_answer iii \
  $NUMBER_ONE \
  $NUMBER_TWO \
  $NUMBER_THREE

oscsend osc.udp://localhost:31337 /dataviz/${DATAVIZ_WINDOW_INDEX}/my_answer iii \
  $NUMBER_ONE \
  $NUMBER_TWO \
  $NUMBER_THREE

