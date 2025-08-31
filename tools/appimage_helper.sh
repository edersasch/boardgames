#! /bin/bash

DOCKER_LINUX_NAME="edersasch/qt6-gcc"
DOCKER_LINUX_TAG="6.9-1"

SCRIPTNAME=$(readlink -nf "$0")
TOOLSDIR=$(dirname "$SCRIPTNAME")
BOARDGAMESDIR=$(dirname "$TOOLSDIR")
DOCKERDIR_LINUX="$TOOLSDIR/docker-gcc-aqt"
BOARDGAMESMOUNTDEST="/boardgames"
BOARDGAMESBINDMOUNT="$BOARDGAMESDIR:$BOARDGAMESMOUNTDEST"

linux_devimage()
{
    docker --debug buildx build --pull --force-rm=true -t $DOCKER_LINUX_NAME:$DOCKER_LINUX_TAG $DOCKERDIR_LINUX && \
    docker tag $DOCKER_LINUX_NAME:$DOCKER_LINUX_TAG $DOCKER_LINUX_NAME:latest
}

linux_devshell()
{
    docker run --rm -it -v "$BOARDGAMESBINDMOUNT" $DOCKER_LINUX_NAME:$DOCKER_LINUX_TAG bash
}

linux_appimage()
{
    DOCKER_CMD="\
        cd "$BOARDGAMESMOUNTDEST" && \
        mkdir -p out/build/linux-appimage && \
        cd out/build/linux-appimage && \
        rm -rf * && \
        mkdir AppDir && \
        cmake -DCMAKE_BUILD_TYPE=Release -GNinja -DBOARDGAMES_BUILD_TESTS=OFF \
            -DCMAKE_TOOLCHAIN_FILE=\${QT_PATH}/\${QT_VERSION}/gcc_64/lib/cmake/Qt6/qt.toolchain.cmake\
            -DCPM_SOURCE_CACHE="$BOARDGAMESMOUNTDEST/out/build/cpm-source-cache" ../../.. && \
        cmake --build . --target all && \
        cmake --install . --prefix=\$PWD/AppDir/usr &&\
        LD_LIBRARY_PATH=\$PWD/AppDir/usr/lib linuxdeploy --appimage-extract-and-run --appdir AppDir --output appimage \
    "

    docker run --rm -v "$BOARDGAMESBINDMOUNT" $DOCKER_LINUX_NAME:$DOCKER_LINUX_TAG bash -c "$DOCKER_CMD"
}

CMD="linux_appimage"
if [ -n "$1" ]; then
   CMD="$1"
fi
$CMD
