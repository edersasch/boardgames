#! /bin/bash

DEBOOTSTRAPSTART="$HOME/opt"
DEBODIR="ubuntu-bionic"

init()
{
    sudo debootstrap bionic "$DEBODIR"/

    sudo chroot "$DEBODIR" adduser boardgamesbuilder
    cd "$DEBODIR"/home/boardgamesbuilder || exit 1
    git clone ~/ar/Dokumente/boardgames
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
. /opt/qt515/bin/qt515-env.sh
CC=gcc-7 CXX=g++-7 cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DBOARDGAMES_BUILD_TESTS=OFF ../boardgames
make -j6
make DESTDIR=AppDir install
export QML_SOURCES_PATHS="../boardgames"
linuxdeploy-x86_64.AppImage --appdir AppDir --plugin qt --output appimage
EOF

    chmod +x bin/mkboardgames-appimage.sh
    cd - || exit 2

    echo "deb http://archive.ubuntu.com/ubuntu bionic universe" | sudo tee -a "$DEBODIR"/etc/apt/sources.list
    echo "deb http://archive.ubuntu.com/ubuntu bionic-updates main universe" | sudo tee -a "$DEBODIR"/etc/apt/sources.list
    echo "deb http://archive.ubuntu.com/ubuntu bionic-security main universe" | sudo tee -a "$DEBODIR"/etc/apt/sources.list
    sudo chroot "$DEBODIR" apt -y install software-properties-common
    sudo chroot "$DEBODIR" add-apt-repository ppa:beineri/opt-qt-5.15.2-bionic -y
    sudo chroot "$DEBODIR" apt -y update
    sudo chroot "$DEBODIR" apt -y upgrade
    sudo chroot "$DEBODIR" apt -y install build-essential g++ qt515base qt515declarative qt515quickcontrols qt515quickcontrols2 qt515svg mesa-common-dev libglu1-mesa-dev fuse pandoc

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
    cd "$DEBODIR"/home/boardgamesbuilder/boardgames || exit 1
    git pull
    cd -
    sudo chroot "$DEBODIR" su - boardgamesbuilder -c mkboardgames-appimage.sh
}

start_muehle()
{
    "$DEBODIR"/home/boardgamesbuilder/build-boardgames/Muehle-x86_64.AppImage
}

CMD="build"
if [ -n "$1" ]; then
   CMD="$1"
fi
cd "$DEBOOTSTRAPSTART" || exit 3
$CMD
