#!/bin/bash
cd $(dirname ${0})

while true;
do
    sleep 1
    ./sh_send_osc_barchart.sh
done

