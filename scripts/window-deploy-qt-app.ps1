<#
 .SYNOPSIS 

  Automate deployment of app and their dependencies

 .DESCRIPTION
  
  This script automate the process of creating a deployable folder containing
  the Qt-related dependencies(libraries, QML, imports, plugins and translation)
  required to run the application from specific folder

  Requirements:
  - Windows PowerShell 5.0 or higher.

 .PARAMETER QtDir

  Root directory where Qt is installed.
  By default, use C:\Qt.
#>

[CmdletBinding()]
param (
  # Command line parameters
  $QtDir = "", # Qt default installation directory path
  $ExeFile = "" # Path of the executable file to deploy 
)

function Main () {
  # Load Assembly to be able to use windows GUI explorer
  Add-Type -AssemblyName System.Windows.Forms | Out-Null

  # Get Qt root directory, if not specified on the command line
  if (-not $QtDir -or !(Test-Path -PathType Container $QtDir)) {
    # Show message in order to get the user ready for take an action
    ShowMessage "A window will open very soon asking you to set the directory where Qt is installed!"
    $QtDir = [string](GetQtDirectory) # Open up folder dialog
  }
  # Search all instances of windeployqt.exe from $QtDir installation folder.
  $windeployqtPath = @(Get-ChildItem -Path $QtDir -Filter windeployqt.exe -Recurse | ForEach-Object FullName | Sort-Object)

  if ($windeployqtPath -eq 0) {
    ExitScript "WinDeployQt command not found, You may not selected the right Qt directory"
  }

  # Search all instances of gcc.exe from $QtDir installation folder.
  $gccPath = @(Get-ChildItem -Path $QtDir -Filter gcc.exe -Recurse | ForEach-Object FullName | Sort-Object)

  if ($gccPath -eq 0) {
    ExitScript "GCC command not found, You may not selected the right Qt directory"
  }

  Write-Host "`nQt directory is: $QtDir"
  Write-Host "Setting up environment for Qt usage..."
  # Setting up environment for Qt usage
  SetQtEnvironment @(Split-Path -Path $windeployqtPath[0])
  SetQtEnvironment @(Split-Path -Path $gccPath[0])
  
  if (-not $ExeFile -or !(Test-Path -PathType Leaf $ExeFile)) {
    # Show message in order to get the user ready for take an action
    ShowMessage "A window will open very soon asking you to load the executable file!"
    $ExeFile = [string](GetExecutableFile) # Open up file dialog
  }

  # Run windeployqt to get the work done 
  windeployqt $ExeFile

  ExitScript "Everything is setted up"
}

function SetQtEnvironment ($environmentPath) {
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

function GetExecutableFile () {
  $exeFile = ""
  # Open file dialog and asking for executable file
  $fileDialog = New-Object System.Windows.Forms.OpenFileDialog
  # Make sure the the user didn't cancel the operation
  if ($fileDialog.ShowDialog() -ne "Cancel") {
    # Get the parent directory of the executable file
    $exeFile = Split-Path -Path $fileDialog.FileName
  }
  else {
    ExitScript "You need to select the executable file that missing DLLs"
  }

  return $exeFile
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
