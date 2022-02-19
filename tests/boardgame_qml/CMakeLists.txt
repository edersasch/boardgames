project(boardgame_qml_test)
include(prjqt)
qtquick_compiler_add_resources(SRC_LIST move_list_qml_test.qrc ${CMAKE_SOURCE_DIR}/data/pic.qrc)
include(prjtest)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    boardgame_qml
    Qt5::Quick
)

if(WIN32)
    windeployqt(${PROJECT_NAME})
endif()
