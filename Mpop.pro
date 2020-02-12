TEMPLATE = subdirs

CONFIG += sdk_no_version_check

# qosc:
# sub_qosc.subdir = qosc
# sub_qosc = qosc
# SUBDIRS += qosc

# mpop_kiosk:
sub_mpop_kiosk.subdir = mpop_kiosk
sub_mpop_kiosk.target = sub_mpop_kiosk
sub_mpop_kiosk.depends = qosc
SUBDIRS += mpop_kiosk

# test_mpop_kiosk:
sub_test_mpop_kiosk.subdir = test_mpop_kiosk
sub_test_mpop_kiosk.target = sub_test_mpop_kiosk
sub_test_mpop_kiosk.depends += sub_mpop_kiosk
sub_test_mpop_kiosk.depends += qosc
SUBDIRS += test_mpop_kiosk
