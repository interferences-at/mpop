#!/bin/bash

NUMBER_ONE=$((RANDOM % 50))
NUMBER_TWO=$((RANDOM % 50))

oscsend osc.udp://localhost:31337 /dataviz/0/my_answer i $NUMBER_ONE
oscsend osc.udp://localhost:31338 /dataviz/0/my_answer i $NUMBER_TWO

