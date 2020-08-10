#!/bin/bash
cd $(dirname ${0})/..

docker-compose run mpop-service ./test_mpop_service/test_mpop_service

