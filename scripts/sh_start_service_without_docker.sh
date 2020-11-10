#!/bin/bash
#
# Starts the MPOP Service
#

CONFIG_MYSQL_PORT=13306 \
    CONFIG_MYSQL_HOST=127.0.0.1 \
    ~/src/mpop/mpop_service/mpop_service

