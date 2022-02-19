set(CMAKE_AUTOMOC ON)
cmake_policy(SET CMP0071 NEW)
# if not found, add Qt installation dir (containing lib/cmake) to CMAKE_PREFIX_PATH
find_package(Qt5 REQUIRED COMPONENTS Core Quick QuickCompiler)