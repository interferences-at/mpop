#!/bin/bash

set -o verbose

oscsend osc.udp://localhost:31337 /dataviz/0/hide_testcard
oscsend osc.udp://localhost:31338 /dataviz/0/hide_testcard

