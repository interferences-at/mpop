#!/bin/bash
# Copies the backups to another computer

rsync -a ~/backups/* dataviz-b:backups/

