project(boardgame_qml)
include(prjqt)
qtquick_compiler_add_resources(SRC_LIST boardgame_qml.qrc)
include(prjlib)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    boardgame
    Qt5::Quick
)
