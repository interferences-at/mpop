#!/bin/bash

DATAVIZ_WINDOW_INDEX=0

set -o verbose 

oscsend osc.udp://localhost:31337 /dataviz/${DATAVIZ_WINDOW_INDEX}/goto_screensaver

