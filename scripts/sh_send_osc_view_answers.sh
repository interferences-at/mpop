#!/bin/bash

DATAVIZ_WINDOW_INDEX=0

MINE_ONE=$((RANDOM % 100))
MINE_TWO=$((RANDOM % 100))
MINE_THREE=$((RANDOM % 100))
MINE_FOUR=$((RANDOM % 100))
MINE_FIVE=$((RANDOM % 100))

THEIRS_ONE=$((RANDOM % 100))
THEIRS_TWO=$((RANDOM % 100))
THEIRS_THREE=$((RANDOM % 100))
THEIRS_FOUR=$((RANDOM % 100))
THEIRS_FIVE=$((RANDOM % 100))

TEXT_ONE="Les victimes"
TEXT_TWO="Les personnes marginalisées"
TEXT_THREE="Les jeunes contrevenants"
TEXT_FOUR="Les gens fortunés"
TEXT_FIVE="Les minorités visibles ou culturelles"

set -o verbose

oscsend osc.udp://localhost:31337 /dataviz/${DATAVIZ_WINDOW_INDEX}/view_answers isiisiisiisiisii \
  5 \
  "${TEXT_ONE}" \
  "${MINE_ONE}" \
  "${THEIRS_ONE}" \
  "${TEXT_TWO}" \
  "${MINE_TWO}" \
  "${THEIRS_TWO}" \
  "${TEXT_THREE}" \
  "${MINE_THREE}" \
  "${THEIRS_THREE}" \
  "${TEXT_FOUR}" \
  "${MINE_FOUR}" \
  "${THEIRS_FOUR}" \
  "${TEXT_FIVE}" \
  "${MINE_FIVE}" \
  "${THEIRS_FIVE}"

