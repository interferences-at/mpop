#!/bin/bash
#
# Database backup

DB_USER=root
DB_PASS=secret
DB_NAME=mpop_database
DUMP_DIR=~/backups
DATE_NOW=$(date +%Y-%m-%d_%Hh%Mm%S)
DELETE_OLDER_THAN=180 # days
DO_DELETE_OLD_FILES=false

echo Date and time: ${DATE_NOW}

mkdir -p ${DUMP_DIR}

mysqldump -u ${DB_USER} -p${DB_PASS} ${DB_NAME} > \
    ${DUMP_DIR}/${DB_NAME}-${DATE_NOW}.sql

# Delete files older than N days:

echo "Old files:"
find ${DUMP_DIR} -type f -mtime +${DELETE_OLDER_THAN}
if [ ${DO_DELETE_OLD_FILES} == true ];
then
    find ${DUMP_DIR} -type f -mtime +${DELETE_OLDER_THAN} \
         -exec rm {} +
fi

