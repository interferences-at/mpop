# Script that install Trim SemiBold font on the system
$FONTS_DOWNLOAD_URL="https://raw.githubusercontent.com/aalex/mpop-private/baydam-patch-1/Trim-SemiBold.otf?token=AA7DZ25WF5NDC2AM657RJLK7TQ6YI"
$FONT_FILENAME="Trim-SemiBold.otf"

# Create shell app that point to fonts folder eg: C:\Windows\Fonts
$FontsDirectory = (New-Object -ComObject Shell.Application).Namespace(0x14)
$Font = Get-Item $FONT_FILENAME

# Pull the fonts from private repository
Invoke-WebRequest -Uri $FONTS_DOWNLOAD_URL -OutFile $FONT_FILENAME
# Install the fonts
Write-Host('Installing font: {0}' -f $Font.BaseName)
$FontsDirectory.CopyHere($Font.Fullname)   