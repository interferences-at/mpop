#!/bin/bash
#
# sh_install__trim_font_linux.sh
# Script that install Trim SemiBold font on the system

FONTS_INSTALL_PATH="$HOME/.local/share/fonts"
FONTS_DOWNLOAD_URL="https://raw.githubusercontent.com/aalex/mpop-private/dev/Trim-SemiBold.otf?token=AA7DZ27CBTECQ6H7HCZC2CK7X3ECY"
FONT_FILENAME="Trim-SemiBold.otf"

# Pull the fonts from private repository
curl -L $FONTS_DOWNLOAD_URL -o $FONT_FILENAME
# Make sure the font folder exists
mkdir -p $FONTS_INSTALL_PATH
# Install the fonts
mv $FONT_FILENAME $FONTS_INSTALL_PATH
# Clear and regenerate your font cache
fc-cache -f -v
# Check if font is well installed
if [[ $(fc-list | grep "Trim-SemiBold") ]]; then
	echo "Done"
else
	echo "You may need to restart your computer"
fi
