#!/bin/bash
# Compiles the MSC file with mscgen

mscgen -i sequence-diagram-rpc-and-osc.msc -T png -o sequence-diagram-rpc-and-osc.png

# Depending on the platform:
open sequence-diagram-rpc-and-osc.png
xdg-open sequence-diagram-rpc-and-osc.png

