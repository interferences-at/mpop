#!/bin/bash

DATAVIZ_WINDOW_INDEX=0
MY_ROW=$((RANDOM % 5))
MY_ANSWER=$((RANDOM % 100))
ALL_VALUES=() # array
TITLES=("Québécoise" "Canadienne" "Autochtone" "Européenne" "Autre")

for i in {1..5};
do
  ALL_VALUES+=($((RANDOM % 100)))
done

set -o verbose

oscsend osc.udp://localhost:31337 /dataviz/${DATAVIZ_WINDOW_INDEX}/view_answer_by_culture iiisisisisisi \
  $DATAVIZ_WINDOW_INDEX \
  $MY_ANSWER \
  $MY_ROW \
  ${TITLES[0]} \
  ${ALL_VALUES[0]} \
  "${TITLES[1]}" \
  ${ALL_VALUES[1]} \
  "${TITLES[2]}" \
  ${ALL_VALUES[2]} \
  "${TITLES[3]}" \
  ${ALL_VALUES[3]} \
  "${TITLES[4]}" \
  ${ALL_VALUES[4]}

