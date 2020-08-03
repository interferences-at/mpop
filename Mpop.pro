TEMPLATE = subdirs

# Silent a warning on macOS:
CONFIG += sdk_no_version_check

# qosc:
#sub_qosc.subdir = qosc
#sub_qosc = qosc
#SUBDIRS += qosc

# mpop_kiosk:
sub_mpop_kiosk.subdir = mpop_kiosk
sub_mpop_kiosk.target = sub_mpop_kiosk
SUBDIRS += mpop_kiosk

# mpop_service:
sub_mpop_service.subdir = mpop_service
sub_mpop_service.target = sub_mpop_service
SUBDIRS += mpop_service

# mpop_dataviz:
sub_mpop_dataviz.subdir = mpop_dataviz
sub_mpop_dataviz.target = sub_mpop_dataviz
SUBDIRS += mpop_dataviz

# test_mpop_kiosk:
sub_test_mpop_kiosk.subdir = test_mpop_kiosk
sub_test_mpop_kiosk.target = sub_test_mpop_kiosk
sub_test_mpop_kiosk.depends += sub_mpop_kiosk
SUBDIRS += test_mpop_kiosk

# test_mpop_service:
sub_test_mpop_service.subdir = test_mpop_service
sub_test_mpop_service.target = sub_test_mpop_service
sub_test_mpop_service.depends += sub_mpop_service
SUBDIRS += test_mpop_service
