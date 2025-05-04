project(boardgame_qml)
include(prjqt)
include(prjstandardpre)
qt_add_library(${PROJECT_NAME} ${SRC_LIST} STATIC)
include(prjstandardpost)

file(GLOB QML_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*.qml)
qt_add_qml_module(${PROJECT_NAME}
    URI src.boardgame_qml
    QML_FILES ${QML_LIST}
    NO_PLUGIN
)

if (NOT MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-shadow)
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    boardgame
    Qt6::Quick
)
