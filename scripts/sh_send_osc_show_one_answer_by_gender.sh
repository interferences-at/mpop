#!/bin/bash

DATAVIZ_WINDOW_INDEX=0
MY_ROW=$((RANDOM % 3))
MY_ANSWER=$((RANDOM % 100))
ALL_VALUES=() # array
GENDERS=(Hommes Femmes Autres)

for i in {1..3};
do
  ALL_VALUES+=($((RANDOM % 100)))
done

set -o verbose

oscsend osc.udp://localhost:31337 /dataviz/${DATAVIZ_WINDOW_INDEX}/view_answer_by_gender iisisisi \
  $MY_ANSWER \
  $MY_ROW \
  ${GENDERS[0]} \
  ${ALL_VALUES[0]} \
  ${GENDERS[1]} \
  ${ALL_VALUES[1]} \
  ${GENDERS[2]} \
  ${ALL_VALUES[2]}

