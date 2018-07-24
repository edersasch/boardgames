project(boardgame_qml_test)
set(CMAKE_AUTOMOC ON)
cmake_policy(SET CMP0071 NEW)
find_package(Qt5Quick)
find_package(Qt5QuickCompiler)
qtquick_compiler_add_resources(SRC_LIST move_list_qml_test.qrc ${CMAKE_SOURCE_DIR}/data/pic.qrc)
include(prjtest)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    boardgames_qml
    Qt5::Quick
)
