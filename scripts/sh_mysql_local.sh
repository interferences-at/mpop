#!/bin/bash
#
# Connects to the local MySQL server.
#

CONFIG_MYSQL_USER=root
CONFIG_MYSQL_DATABASE=mpop_database
CONFIG_MYSQL_PASSWORD=secret

mysql -u ${CONFIG_MYSQL_USER} -p${CONFIG_MYSQL_PASSWORD} ${CONFIG_MYSQL_DATABASE}

