#! /bin/bash

DOCKER_LINUX_NAME="edersasch/qt6-gcc"
DOCKER_LINUX_TAG="6.9.3-1"
DOCKER_WINDOWS_NAME="edersasch/qt6-mingw"
DOCKER_WINDOWS_TAG="6.9.3-1"

SCRIPTNAME=$(readlink -nf "$0")
TOOLSDIR=$(dirname "$SCRIPTNAME")
BOARDGAMESDIR=$(dirname "$TOOLSDIR")
DOCKERDIR_LINUX="$TOOLSDIR/docker-gcc-aqt"
DOCKERDIR_WINDOWS="$TOOLSDIR/docker-mingw-aqt"
BOARDGAMESMOUNTDEST="/boardgames"
BOARDGAMESBINDMOUNT="$BOARDGAMESDIR:$BOARDGAMESMOUNTDEST"

linux_devimage()
{
    docker --debug buildx build --pull --force-rm=true -t $DOCKER_LINUX_NAME:$DOCKER_LINUX_TAG $DOCKERDIR_LINUX && \
    docker tag $DOCKER_LINUX_NAME:$DOCKER_LINUX_TAG $DOCKER_LINUX_NAME:latest
}

linux_devshell()
{
    docker run --rm -it -v /tmp/.X11-unix:/tmp/.X11-unix -v "$BOARDGAMESBINDMOUNT" $DOCKER_LINUX_NAME:$DOCKER_LINUX_TAG bash
}

linux_appimage()
{
    DOCKER_CMD="\
        cd "$BOARDGAMESMOUNTDEST" && \
        mkdir -p build/linux-appimage && \
        cd build/linux-appimage && \
        rm -rf * && \
        mkdir AppDir && \
        cmake -DCMAKE_BUILD_TYPE=Release -GNinja -DBOARDGAMES_BUILD_TESTS=OFF \
            -DCMAKE_TOOLCHAIN_FILE=\${QT_PATH}/\${QT_VERSION}/gcc_64/lib/cmake/Qt6/qt.toolchain.cmake \
            -DCPM_SOURCE_CACHE="$BOARDGAMESMOUNTDEST/build/cpm-source-cache" ../.. && \
        cmake --build . --target all && \
        cmake --install . --prefix=\$PWD/AppDir/usr && \
        LD_LIBRARY_PATH=\$PWD/AppDir/usr/lib linuxdeploy --appimage-extract-and-run --appdir AppDir --output appimage \
    "

    docker run --rm -v "$BOARDGAMESBINDMOUNT" $DOCKER_LINUX_NAME:$DOCKER_LINUX_TAG bash -c "$DOCKER_CMD"
}

windows_devimage()
{
    docker --debug buildx build --pull --force-rm=true -t $DOCKER_WINDOWS_NAME:$DOCKER_WINDOWS_TAG $DOCKERDIR_WINDOWS && \
    docker tag $DOCKER_WINDOWS_NAME:$DOCKER_WINDOWS_TAG $DOCKER_WINDOWS_NAME:latest
}

windows_devshell()
{
    docker run --rm -it -v "$BOARDGAMESBINDMOUNT" $DOCKER_WINDOWS_NAME:$DOCKER_WINDOWS_TAG bash
}

windows_archive()
{
    DOCKER_CMD="\
        cd "$BOARDGAMESMOUNTDEST" && \
        mkdir -p build/windows_archive && \
        cd build/windows_archive && \
        rm -rf * && \
        mkdir AppDir && \
        cmake -DCMAKE_BUILD_TYPE=Release -GNinja -DBOARDGAMES_BUILD_TESTS=OFF \
            -DCMAKE_TOOLCHAIN_FILE=\${QT_PATH}/\${QT_VERSION}/mingw_64/lib/cmake/Qt6/qt.toolchain.cmake \
            -DCPM_SOURCE_CACHE="$BOARDGAMESMOUNTDEST/build/cpm-source-cache" ../.. && \
        cmake --build . --target all && \
        cmake --install . --prefix=\$PWD/AppDir/usr
    "
    docker run --rm -v "$BOARDGAMESBINDMOUNT" $DOCKER_WINDOWS_NAME:$DOCKER_WINDOWS_TAG bash -c "$DOCKER_CMD"
}

CMD="linux_appimage"
if [ -n "$1" ]; then
   CMD="$1"
fi
$CMD
