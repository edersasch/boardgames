# if not found, add Qt installation dir (containing lib/cmake) to CMAKE_PREFIX_PATH
find_package(Qt6 REQUIRED COMPONENTS Core Quick Svg)
qt_policy(SET QTP0001 NEW)
qt_standard_project_setup()
set(CMAKE_AUTORCC ON)
