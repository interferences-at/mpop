# Developer notes

Here are the developer notes for this project.

## Building

You need Qt 5 to build this project, including QtQuick.

Open the .pro file in Qt Creator. Build it.

Another option is to do:

```
qmake .
make
```

## Install dependencies on Ubuntu

```
sudo apt-get install -y libqt5sql5-mysql qt5-default libqt5opengl5-dev libqt5serialport5-dev libqt5websockets5-dev libqt5quickcontrols2-5 liblo-tools qtdeclarative5-dev mysql-client clang-8
```


## Deployment on Windows

To use the mpop_dataviz program in production on Windows, you should run the 
window-deploy-qt-app.ps1 script. It copies the required DLL files in the current directory.

Right-click on the scripts/window-deploy-qt-app.ps1 Powershell script and choose to run it with Powershell.

You can also do this:

Launch the PowerShell application.

Run this command:

```
Set-ExecutionPolicy -Scope CurrentUser RemoteSigned
```

And then this:

```
.\scripts\window-deploy-qt-app.ps1
```

A dialog will prompt you for the location of your Qt installation. C:\Qt is typical.


# Coding style

We follow the Qt style guide for C++ and QML.

