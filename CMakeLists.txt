cmake_minimum_required(VERSION 3.5.1)
project(boardgames)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
enable_testing()
include(CTest)
add_compile_options(
    -Wall -Wextra -Werror
)

if(COVERAGE_REPORTS)
    #include(CTest)
    add_compile_options(--coverage)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} --coverage")
    configure_file(CTestCustom.cmake CTestCustom.cmake)
    # run tests from root of build directory: ctest -D ExperimentalMemCheck
endif(COVERAGE_REPORTS)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)

add_subdirectory(src_muehle)
add_subdirectory(src_muehleqml)
add_subdirectory(src_muehleqmlstandalone)
add_subdirectory(test_gmock)
add_subdirectory(test_muehle)
