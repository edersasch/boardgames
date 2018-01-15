project(muehle)
find_package(Qt5Gui)
include(prjexe)
target_link_libraries(${PROJECT_NAME}
    boardgames_muehleqml
    Qt5::Gui
)
