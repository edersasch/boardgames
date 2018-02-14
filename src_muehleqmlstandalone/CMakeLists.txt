project(muehle)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Quick)
find_package(Qt5Gui)
qt5_add_resources(SRC_LIST muehlecontrol.qrc)
qt5_add_resources(SRC_LIST ../pic/pic.qrc)
include(prjexe)
target_link_libraries(${PROJECT_NAME}
    boardgames_muehleqml
    Qt5::Gui
)
