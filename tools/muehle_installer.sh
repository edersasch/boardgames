#!/usr/bin/env bash

# adapted from https://github.com/laurent22/joplin/blob/a571d38862a8799e46568dd32cada2c6042418d4/Joplin_install_and_update.sh
# licensed under MIT by Laurent Cozic, see https://github.com/laurent22/joplin/blob/a571d38862a8799e46568dd32cada2c6042418d4/LICENSE

echo "Checking architecture ..."
ARCHITECTURE=$(uname -m -p -i || echo "NO CHECK")
if [[ $ARCHITECTURE = "NO CHECK" ]] ; then
  echo "WARNING: Can't get system architecture, skipping check"
elif [[ $ARCHITECTURE =~ .*aarch.*|.*arm.* ]] ; then
  echo "Sorry: Arm systems are not supported"
  exit 1
elif [[ $ARCHITECTURE =~ .*i386.*|.*i686.* ]] ; then
  echo "Sorry: 32-bit systems are not supported"
  exit 1
fi

echo "Checking version ..."
RELEASE_VERSION=$(wget -qO - "https://api.github.com/repos/edersasch/boardgames/releases/latest" | grep -Po '"tag_name": ?"v\K.*?(?=")')
if [[ -e ~/.muehle/VERSION ]] && [[ $(< ~/.muehle/VERSION) == "${RELEASE_VERSION}" ]]; then
    echo "You already have the latest version ${RELEASE_VERSION} installed."
    exit 0
else
    [[ -e ~/.muehle/VERSION ]] && CURRENT_VERSION=$(< ~/.muehle/VERSION)
    echo "The latest version is ${RELEASE_VERSION}, but you have ${CURRENT_VERSION:-no version} installed."
fi

echo 'Downloading ...'
TEMP_DIR=$(mktemp -d)
wget -O ${TEMP_DIR}/Muehle.AppImage https://github.com/edersasch/boardgames/releases/download/v${RELEASE_VERSION}/Muehle-x86_64.AppImage
wget -O ${TEMP_DIR}/muehle.svg https://raw.githubusercontent.com/edersasch/boardgames/master/data/muehle_field_bright.svg

echo 'Installing ...'
rm -f ~/.muehle/*.AppImage ~/.muehle/VERSION ~/.local/share/applications/appimagekit-muehle.desktop
mkdir -p ~/.muehle/
mv ${TEMP_DIR}/Muehle.AppImage ~/.muehle/Muehle.AppImage
chmod +x ~/.muehle/Muehle.AppImage
mkdir -p ~/.local/share/icons/hicolor/scalable/apps
mv ${TEMP_DIR}/muehle.svg ~/.local/share/icons/hicolor/scalable/apps/muehle.svg
rm -rf $TEMP_DIR

echo 'Create menu entry ...'
SANDBOXPARAM=""
if command -v lsb_release &> /dev/null; then
  DISTVER=$(lsb_release -is) && DISTVER=$DISTVER$(lsb_release -rs)
  DISTCODENAME=$(lsb_release -cs)
  # Check for "The SUID sandbox helper binary was found, but is not configured correctly" problem.
  if [[ $DISTVER =~ Debian1. ]] || [ "$DISTVER" = "Linuxmint4" ] && [ "$DISTCODENAME" = "debbie" ]; then
    SANDBOXPARAM=" --no-sandbox"
  fi
fi
mkdir -p ~/.local/share/applications
echo -e "[Desktop Entry]\nEncoding=UTF-8\nName=Muehle\nComment=A classical boardgame, also known as nine men's morris\nComment[de]=Ein klassisches Brettspiel\nExec=${HOME}/.muehle/Muehle.AppImage${SANDBOXPARAM}\nIcon=muehle\nStartupWMClass=Muehle\nType=Application\nCategories=Game;\nKeywords=muehle;morris;boardgame" >> ~/.local/share/applications/appimagekit-muehle.desktop
[[ `command -v update-desktop-database` ]] && update-desktop-database ~/.local/share/applications && update-desktop-database ~/.local/share/icons
echo $RELEASE_VERSION > ~/.muehle/VERSION
echo "Muehle version ${RELEASE_VERSION} installed."
