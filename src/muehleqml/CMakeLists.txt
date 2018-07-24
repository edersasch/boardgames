project(muehle)
set(CMAKE_AUTOMOC ON)
cmake_policy(SET CMP0071 NEW)
find_package(Qt5Quick)
find_package(Qt5QuickCompiler)
qtquick_compiler_add_resources(SRC_LIST muehle.qrc ${CMAKE_SOURCE_DIR}/data/pic.qrc)
set(OBJLIBS $<TARGET_OBJECTS:boardgames_muehle>)
include(prjexe)
target_link_libraries(${PROJECT_NAME}
    Qt5::Quick
    boardgames_qml
)
