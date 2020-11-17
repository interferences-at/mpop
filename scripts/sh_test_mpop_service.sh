#!/bin/bash
cd $(dirname ${0})/..

docker-compose run service ./test_mpop_service/test_mpop_service

