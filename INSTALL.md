# Installation instructions

See the CONTRIBUTING.md file for information about:

- How to build it with Qt Creator
- How to install its dependencies on GNU/Linux
- How to copy the DLL files for the Qt library on Windows
- Coding style, etc.

Now, in this file, we mostly focus on how to deploy the software, once it's built.

## Running the database server

Our database engine is MySQL, and we run it within Docker Compose.

Do `docker-compose up` or `docker-compose up -d` to run it.

It will also run its database migrations scripts. Those script create the tables and populate them.

To reset your database: (for example, if a migration script has beed modified)

`docker-compose down --volumes`

The migration scripts are in the flyway/sql folder.


## Deploying MPOP service

Mpop service offers a JSON-RPC 2.0 Websocket API.

Build the service with:

`qmake . && make`

And then, run the service with:

`./run-service.sh`

It expects to find the MySQL server running, in its Docker Compose container.

To configure the service, provide some environment variables.


## Deploying MPOP kiosk

MPOP kiosk is the user interface to answer to all questions and control the dataviz.

We plan to use Lunch to launch it. For now, we parse command-line options.

There are three (3) flavours of MPOP kiosk:

- entrance: Only asks visitors the demographic questions.
- central: Asks demographic questions, if not answered, and then allows visitors to answer all question and control the dataviz.
- exit: Frees the RFID tag, so that other users can use it. (they also all expire at midnight)

The mpop_kiosk is configured mainly via environment variables. Here is the list of variables with their default value:
(see in KioskConfig.h and main.cpp for the most up-to-date list)

```
SEND_OSC_DATAVIZ_PORT=14444
SEND_OSC_DATAVIZ_HOST=127.0.0.1
MPOP_SERVICE_HOST=0.0.0.0
MPOP_SERVICE_PORT_NUMBER=3333
RECEIVE_OSC_PORT=15555
CONFIG_IS_VERBOSE=true
CONFIG_IS_FULLSCREEN=false
MPOP_KIOSK_MODE=central
```


## Deploying MPOP dataviz

Mpop service offers an OSC API.

