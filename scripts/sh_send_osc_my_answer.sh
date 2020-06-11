#!/bin/bash

DATAVIZ_WINDOW_INDEX=0

NUMBER_ONE=$((RANDOM % 50))

echo oscsend osc.udp://localhost:31337 /dataviz/${DATAVIZ_WINDOW_INDEX}/my_answer i \
  $NUMBER_ONE

oscsend osc.udp://localhost:31337 /dataviz/${DATAVIZ_WINDOW_INDEX}/my_answer i \
  $NUMBER_ONE

