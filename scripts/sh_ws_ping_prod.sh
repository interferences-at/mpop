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

wscat --connect localhost:3333 --execute '{"jsonrpc":"2.0", "method":"ping", "params":[], "id": 1}'

