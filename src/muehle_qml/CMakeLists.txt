project(muehle_qml)
include(prjqt)
include(prjstandardpre)
qt_add_library(${PROJECT_NAME} ${SRC_LIST} STATIC)
include(prjstandardpost)

file(GLOB QML_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*.qml)
qt_add_qml_module(${PROJECT_NAME}
    URI src.muehle_qml
    VERSION 0.1
    QML_FILES ${QML_LIST}
    NO_PLUGIN
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wno-shadow)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    boardgame
    boardgame_qml
    muehle
    Qt6::Quick
)

if(WIN32)
    windeployqt(${PROJECT_NAME})
endif()
