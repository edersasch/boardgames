#! /bin/bash

QMLLINT=/usr/lib/qt6/bin/qmllint

# arg should be ${CMAKE_BINARY_DIR}
if [ -n "$1" ]; then
    cd "$1" && $QMLLINT --resource src/boardgame_qml/.rcc/qmake_src_boardgame_qml.qrc src/muehle_qml/*.qml \
    src/boardgame_qml/*.qml
fi
