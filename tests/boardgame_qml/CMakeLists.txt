project(boardgame_qml_test)
include(prjqt)
include(prjstandardpre)
qt_add_executable(${PROJECT_NAME} ${SRC_LIST} ${CMAKE_SOURCE_DIR}/data/pic.qrc)
include(prjstandardpost)
add_gtest(${PROJECT_NAME})

file(GLOB QML_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*.qml)
# STATIC qmlmodule pleases qmllint
qt_add_qml_module(${PROJECT_NAME}-qmlmodule STATIC
    URI tests.boardgame_qml
    QML_FILES ${QML_LIST}
    NO_PLUGIN
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${PROJECT_NAME}-qmlmodule
    boardgame_qml
    Qt::Quick
)
