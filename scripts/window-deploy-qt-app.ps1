#  MIT License
#
#  Copyright (c) 2020 Dame Diongue
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in all
#  copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.
#

<#
 .SYNOPSIS 

  Automate deployment of app and their dependencies

 .DESCRIPTION
  
  This script automate the process of creating a deployable folder containing
  the Qt-related dependencies(libraries, QML, imports, plugins and translation)
  required to run the application from specific folder

  Requirements:
  - Windows PowerShell 3.0 or higher.

 .PARAMETER QtStaticDir

  Root directory where Qt is installed.
  By default, use C:\Qt.
#>

[CmdletBinding()]
param (
  # Command line parameters
  $QtDir = ""
)

function Main () {
  # Load Assembly to be able to use windows GUI explorer
  Add-Type -AssemblyName System.Windows.Forms | Out-Null

  # Get Qt root directory, if not specified on the command line
  if (-not $QtDir) {
    # Show message in order to get the user ready for take an action
    ShowMessage "A window will open very soon asking you to set the directory where Qt is installed!"
    $QtDir = [string](GetQtDirectory) # Open up folder dialog
    # Search all instances of windeployqt.exe from $QtDir installation folder.
    $exeFolders = @(Get-ChildItem -Path $QtDir -Filter windeployqt.exe -Recurse | ForEach-Object FullName | Sort-Object)

    if ($exeFolders -eq 0) {
      ExitScript "WinDeployQt command not found, You may not selected the right Qt directory"
    }
    # Get the directory path of the command
    $windeployqtDir = Split-Path -Path $exeFolders[0]

    Write-Host "Qt directory is: $QtDir"
    # Setting up environment for Qt usage
    SetQtEnvironment $windeployqtDir
  }
  
  
  # Show message in order to get the user ready for take an action
  ShowMessage "A window will open very soon asking you to load the executable file!"

  # Open file dialog and asking for executable file
  $fileDialog = New-Object System.Windows.Forms.OpenFileDialog
  # Make sure the the user didn't cancel the operation
  if ($fileDialog.ShowDialog() -ne "Cancel") {
    # Get the parent directory of the executable file
    $exeFolder = Split-Path -Path $fileDialog.FileName
    # Run windeployqt to get the work done 
    windeployqt $exeFolder

    ExitScript "Everything is setted up"
  }
  else {
    ExitScript "You need to select the executable file that missing DLLs"
  }

}

function SetQtEnvironment ($environmentPath) {
  Write-Host "Setting up environment for Qt usage..."
  Write-Host "Environment PATH set to $environmentPath"
  # Adding a new path to PATH
  $env:PATH = "$environmentPath;$env:PATH"
}

function GetQtDirectory () {
  $QtDir = ""
  # Open folder dialog and asking for Qt installation directory
  $folderDialog = New-Object System.Windows.Forms.FolderBrowserDialog
  # Make sure the the user didn't cancel the operation
  if ($folderDialog.ShowDialog() -ne "Cancel") {
    # Get the selected path
    $QtDir = $folderDialog.SelectedPath
  }
  else {
    # Exit
    ExitScript "You need to specify the root directory where Qt are installed"
  }

  return $QtDir
}

function ShowMessage ([string]$message) {
  Write-Host "`n`n$message"
  Write-Host -NoNewline "Press any key to continue..."
  $null = $Host.UI.RawUI.ReadKey('NoEcho, IncludeKeyDown')
}

function ExitScript ([string]$message = "") {
  $errorCode = 0

  if ($message -ne "") {
    ShowMessage $message
    $errorCode = 1
  }

  exit $errorCode
}


# Execute main function
. Main
