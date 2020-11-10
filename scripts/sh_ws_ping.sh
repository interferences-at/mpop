#!/bin/bash
#
# Test the ping JSON-RPC 2.0 method over websocket.
#
# Installation:
# 
# sudo apt install nodejs npm
# sudo npm install -g wscat
#
#
# To use it manually:
# wscat --conect localhost:3333
# {"jsonrpc":"2.0", "method":"ping", "params":[], "id": 1}
#

SERVICE_HOST=192.200.200.66

wscat --connect ${SERVICE_HOST}:3333 --execute '{"jsonrpc":"2.0", "method":"ping", "params":[], "id": 1}'

