#!/bin/sh -xe

[ "$AQT_VERSION" ] || exit 1
[ "$QT_VERSION" ] || exit 2
[ "$QT_PATH" ] || exit 3

apt update
apt upgrade -y
apt install -y --no-install-suggests --no-install-recommends $ADDITIONAL_PACKAGES

# linuxdeploy
LINUXDEPLOY_URL="https://github.com/linuxdeploy/linuxdeploy/releases/download/$LINUXDEPLOY_VERSION/linuxdeploy-x86_64.AppImage"
curl -L "$LINUXDEPLOY_URL" -o /usr/local/bin/linuxdeploy
chmod a+x /usr/local/bin/linuxdeploy

# Qt
root_dir=$PWD
[ "$root_dir" != '/' ] || root_dir=""

dpkg --get-selections | cut -f 1 > /tmp/packages_orig.lst

apt install -y git python3-pip libglib2.0-0
pip3 install --no-cache-dir "$AQT_VERSION"

aqt install-qt -O "$QT_PATH" linux desktop "$QT_VERSION" linux_gcc_64
aqt install-tool -O "$QT_PATH" linux desktop tools_cmake
aqt install-tool -O "$QT_PATH" linux desktop tools_ninja

pip3 freeze | xargs pip3 uninstall -y

dpkg --get-selections | cut -f 1 > /tmp/packages_curr.lst
grep -Fxv -f /tmp/packages_orig.lst /tmp/packages_curr.lst | xargs apt remove -y --purge

find "$QT_PATH" /usr/local -executable -type f -o -name '*.so' | xargs ldd 2>/dev/null | \
    grep -F '=> not found' | tr '\t' ' ' | cut -d' ' -f2 | sort -u | tee /tmp/not_found_libs.lst

apt install -y apt-file
apt-file update

while read line ; do apt-file find $line | grep '^lib' | head -1; done < /tmp/not_found_libs.lst | tee /tmp/to_install_libs.lst
apt autoremove -y --purge apt-file
cat /tmp/to_install_libs.lst | cut -d: -f 1 | xargs apt install -y --no-install-suggests --no-install-recommends

# Cleanup
apt -qq clean
rm -rf /var/lib/apt/lists/*

locale-gen en_US.UTF-8 && dpkg-reconfigure locales
groupadd -r user && useradd --create-home --gid user user && echo 'user ALL=NOPASSWD: ALL' > /etc/sudoers.d/user