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


## Deploying MPOP Service

MPOP Service offers a JSON-RPC 2.0 Websocket API.

Previously, we used Docker Compose to build mpop\_service and deploy it.

Now, we build it by hand and deploy it with a shell script.

`./sh_start_service_without_docker.sh`

Lunch launches the service.

See `scripts/lunch_kiosk.py`

See `tail -F /tmp/odroid/lunch-child-service.log`


## Deploying MPOP Kiosk

MPOP Kiosk is the user interface to answer to all questions and control the dataviz.

It can be launched with Lunch - the distributed process launcher - or manually. It can parse command-line options and/or environment variables.

There are three (3) flavours of MPOP kiosk:

- entrance: Only asks visitors the demographic questions.
- central: Asks demographic questions, if not answered, and then allows visitors to answer all question and control the dataviz.
- exit: Frees the RFID tag, so that other users can use it. (they also all expire at midnight)

The mpop\_kiosk is configured mainly via command-line options. Run it with the --help option to view the command-line arguments you can provide it.

The MPOP Kiosk should know on which host to find the MPOP Service and the MPOP Dataviz.
The devops engineer who deploys it must make sure that its configuration options are set properly.


## Deploying MPOP Dataviz

Mpop Dataviz offers an OSC API.

Launch it directly, or with Lunch.


## Setup MySQL without Docker

```
sudo apt install mysql-server
```

```
sudo mysql_secure_installation
```

Follow the inscructions in https://www.digitalocean.com/community/tutorials/how-to-install-mysql-on-ubuntu-20-04

```
mysql -u root -p
```

```
CREATE USER 'root'@'host' IDENTIFIED WITH authentication_plugin BY 'secret';
```

```
FLUSH PRIVILEGES;
```

```
systemctl status mysql.service
```

Then, edit the crontab:

```
crontab -e
```

Add this:

```
# minutes hours monthday month weekday command
4 11 * * * ~/src/mpop/scripts/sh_backup_db.sh
```

It will run the SQL backup every day at 11h04.

