#!/bin/bash

DELAY=8

while true
do
  ./sh_send_osc_goto_screensaver.sh
  sleep ${DELAY}
  ./sh_send_osc_view_answers.sh
  sleep ${DELAY}
  ./sh_send_osc_view_answers.sh
  sleep ${DELAY}
  ./sh_send_osc_view_answers.sh
  sleep ${DELAY}
  ./sh_send_osc_view_answers.sh
  sleep ${DELAY}
  ./sh_send_osc_view_answers.sh
  sleep ${DELAY}
  ./sh_send_osc_view_answers.sh
  sleep ${DELAY}
  ./sh_send_osc_view_answers.sh
  sleep ${DELAY}
  ./sh_send_osc_goto_screensaver.sh
done
