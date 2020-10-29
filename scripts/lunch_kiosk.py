#!/usr/bin/env python3

"""
Lunch script for the kiosk
"""

import socket

# Global configuration:
DATAVIZ_HOST_A = '127.0.0.1'
DATAVIZ_HOST_B = '127.0.0.1'
DATAVIZ_PORT_LEFT = 31337
DATAVIZ_PORT_RIGHT = 31338
SERVICE_HOST = "192.200.200.66"


# Configuration for this instance only:
KIOSK_MODE = "central"
DATAVIZ_HOST = DATAVIZ_HOST_A # default
DATAVIZ_PORT = DATAVIZ_PORT_LEFT # default


def get_hostname():
    return socket.gethostname()

my_hostname = get_hostname()

# According to the hostname for this computer
# set the different options to use
if my_hostname == 'kiosk-entrance':
    KIOSK_MODE = 'entrance'

elif my_hostname == 'kiosk-exit':
    KIOSK_MODE = 'exit'

elif my_hostname == 'kiosk-central-1':
    KIOSK_MODE = 'central'
    DATAVIZ_HOST = DATAVIZ_HOST_A
    DATAVIZ_PORT = DATAVIZ_PORT_LEFT
    # We run the MPOP Service on host central-1
    add_command("docker-compose --file ~/src/mpop/docker-compose.yml up")

elif my_hostname == 'kiosk-central-2':
    KIOSK_MODE = 'central'
    DATAVIZ_HOST = DATAVIZ_HOST_A
    DATAVIZ_PORT = DATAVIZ_PORT_RIGHT

elif my_hostname == 'kiosk-central-3':
    KIOSK_MODE = 'central'
    DATAVIZ_HOST = DATAVIZ_HOST_B
    DATAVIZ_PORT = DATAVIZ_PORT_LEFT

elif my_hostname == 'kiosk-central-4':
    KIOSK_MODE = 'central'
    DATAVIZ_HOST = DATAVIZ_HOST_B
    DATAVIZ_PORT = DATAVIZ_PORT_RIGHT


# Add the command to Lunch
add_command("~/src/mpop/mpop_kiosk/mpop_kiosk --kiosk-mode {kiosk_mode} --service-host {service_host} --dataviz-host {dataviz_host} --dataviz-port {dataviz_port} --fullscreen".format(
        kiosk_mode=KIOSK_MODE,
        service_host=SERVICE_HOST,
        dataviz_host=DATAVIZ_HOST,
        dataviz_port=DATAVIZ_PORT))

