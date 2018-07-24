project(boardgames_qml)
set(CMAKE_AUTOMOC ON)
cmake_policy(SET CMP0071 NEW)
find_package(Qt5Quick)
find_package(Qt5QuickCompiler)
qtquick_compiler_add_resources(SRC_LIST boardgame_qml.qrc)
include(prjlib)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt5::Quick
)
