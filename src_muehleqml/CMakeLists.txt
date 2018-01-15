project(boardgames_muehleqml)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Gui)
find_package(Qt5Quick)
qt5_add_resources(SRC_LIST resources.qrc)
include(prjlib)
target_link_libraries(${PROJECT_NAME}
    boardgames_muehle
    Qt5::Gui
    Qt5::Quick
)
