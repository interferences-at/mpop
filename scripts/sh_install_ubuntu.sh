#!/bin/bash
#
# Install the necessary libraries and setup the groups on Ubuntu
#

if ! [ $(id -u) = 0 ]; then
   echo "This script must be run with sudo"
   echo "sudo !!"
   exit 1
fi

apt install git terminator screen qt5-default openssh-server docker.io aptitude vim libqt5sql5-mysql qt5-default libqt5opengl5-dev libqt5serialport5-dev libqt5websockets5-dev libqt5quickcontrols2-5 liblo-tools qtdeclarative5-dev mysql-client clang-8 libqt5sql5-mysql build-essential qttools5-dev-tools qtcreator qtdeclarative5-dev qml-module-qttest qml-module-qtquick-localstorage qml-module-qtquick-extras qml-module-qtquick-dialogs qml-module-qtquick-controls2 qml-module-qt-websockets qml-module-qt-labs-settings qml-module-qt-labs-platform docker-compose terminator python3-setuptools python3-twisted clusterssh gitk git-gui gitg curl

usermod -aG dialout ${USER}
usermod -aG docker ${USER}

