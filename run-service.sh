#!/bin/bash

CONFIG_IS_VERBOSE=true \
    CONFIG_MYSQL_DATABASE=mpop_database \
    CONFIG_MYSQL_HOST=0.0.0.0 \
    CONFIG_MYSQL_PASSWORD=secret \
    CONFIG_MYSQL_PORT=3306 \
    CONFIG_MYSQL_USER=admin \
    CONFIG_USE_SQLITE=false \
    MPOP_SERVICE_PORT_NUMBER=3333 \
    ../build-Mpop-Desktop-Debug/mpop_service/mpop_service

