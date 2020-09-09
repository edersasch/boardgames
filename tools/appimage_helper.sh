#! /bin/bash

DEBOOTSTRAPSTART="$HOME/opt"
DEBODIR="ubuntu-xenial"

init()
{
    sudo debootstrap xenial "$DEBODIR"/

    sudo chroot "$DEBODIR" adduser xenialuser
    cd "$DEBODIR"/home/xenialuser || exit 1
    git clone ~/Dokumente/Alexander/boardgames
    mkdir -p bin
    wget -P bin https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
    wget -P bin https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
    chmod +x bin/linuxdeploy-x86_64.AppImage
    chmod +x bin/linuxdeploy-plugin-qt-x86_64.AppImage

    cat << EOF > bin/mkboardgames-appimage.sh
mkdir -p build-boardgames
cd build-boardgames || exit 1
rm -rf *
mkdir AppDir
. /opt/qt512/bin/qt512-env.sh
CC=gcc-7 CXX=g++-7 cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DBOARDGAMES_BUILD_TESTS=OFF ../boardgames
make -j6
make DESTDIR=AppDir install
export QML_SOURCES_PATHS="../boardgames"
linuxdeploy-x86_64.AppImage --appdir AppDir --plugin qt --output appimage
EOF

    chmod +x bin/mkboardgames-appimage.sh
    cd - || exit 2

    echo "deb http://archive.ubuntu.com/ubuntu xenial universe" | sudo tee -a "$DEBODIR"/etc/apt/sources.list
    echo "deb http://archive.ubuntu.com/ubuntu xenial-security main universe" | sudo tee -a "$DEBODIR"/etc/apt/sources.list
    sudo chroot "$DEBODIR" apt-get -y install software-properties-common
    sudo chroot "$DEBODIR" add-apt-repository ppa:beineri/opt-qt-5.12.2-xenial -y
    sudo chroot "$DEBODIR" add-apt-repository ppa:ubuntu-toolchain-r/test -y
    sudo chroot "$DEBODIR" apt-get -y update
    sudo chroot "$DEBODIR" apt-get -y upgrade
    sudo chroot "$DEBODIR" apt-get -y install build-essential g++-7 qt512base qt512declarative qt512quickcontrols qt512quickcontrols2 qt512svg mesa-common-dev libglu1-mesa-dev fuse pandoc

    wget https://github.com/Kitware/CMake/releases/download/v3.14.1/cmake-3.14.1-Linux-x86_64.tar.gz
    sudo tar -C "$DEBODIR"/usr/local --strip-components=1 -xzf cmake-3.14.1-Linux-x86_64.tar.gz
}

prepare()
{
    sudo mount proc "$DEBODIR"/proc -t proc
    sudo mount --bind /dev "$DEBODIR"/dev
}

build()
{
    sudo chroot "$DEBODIR" su - xenialuser -c mkboardgames-appimage.sh
}

start_muehle()
{
    "$DEBODIR"/home/xenialuser/build-boardgames/Muehle-x86_64.AppImage
}

CMD="build"
if [ -n "$1" ]; then
   CMD="$1"
fi
cd "$DEBOOTSTRAPSTART" || exit 3
$CMD
