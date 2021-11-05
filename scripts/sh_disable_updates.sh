#!/bin/bash
#
# Disable all updates on Ubuntu 20.04

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 
   exit 1
fi

APT_AUTO_UPGRADE_CONFIG_FILE=/etc/apt/apt.conf.d/20auto-upgrades

# To enable them:
# cat << EOF > ${APT_AUTO_UPGRADE_CONFIG_FILE}
# APT::Periodic::Update-Package-Lists "1";
# APT::Periodic::Unattended-Upgrade "1";
# EOF

# To disable them:
cat << EOF > ${APT_AUTO_UPGRADE_CONFIG_FILE}
APT::Periodic::Update-Package-Lists "0";
APT::Periodic::Download-Upgradeable-Packages "0";
APT::Periodic::AutocleanInterval "0";
APT::Periodic::Unattended-Upgrade "1";
EOF

cat ${APT_AUTO_UPGRADE_CONFIG_FILE}
echo
echo Updated ${APT_AUTO_UPGRADE_CONFIG_FILE}

