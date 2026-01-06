#!/bin/bash
# Copies the backups to another computer

rsync --archive \
    --verbose \
    --human-readable \
    --delete-before \
    ~/backups/ dataviz-b:backups/

