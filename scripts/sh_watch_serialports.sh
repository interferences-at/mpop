#!/bin/bash

# Watches the serial ports for new devices

DEVICEFILES=/dev/*
MYTEMPORARYFILE==$(mktemp listdevices-)
PREVIOUSFILE=${MYTEMPORARYFILE}.previous.tmp
CURRENTFILE=${MYTEMPORARYFILE}.current.tmp

# Create/empty temporary file
echo > ${CURRENTFILE}
echo > ${PREVIOUSFILE}

while true
do
  # List files
  ls ${DEVICEFILES} > ${CURRENTFILE}
  # Show the difference, if any.
  diff ${PREVIOUSFILE} ${CURRENTFILE}
  # Store the new list into the old one
  cp ${CURRENTFILE} ${PREVIOUSFILE}
  # Important to wait a bit between each check, to save CPU
  # and to make it legible.
  sleep 1
done


