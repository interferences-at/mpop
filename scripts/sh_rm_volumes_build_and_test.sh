#!/bin/bash

cd $(dirname ${0})/..

# Remove the former database
docker-compose rm --force --stop -v db

# Rebuild the docker image
docker-compose build

# Run the unit tests for the MPOP service
docker-compose run mpop-service ./test_mpop_service/test_mpop_service

