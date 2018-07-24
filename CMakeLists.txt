cmake_minimum_required(VERSION 3.5.1)
project(boardgames)

add_compile_options(
    -Wall -Wextra -Werror -Wshadow
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(MEMORYCHECK_SUPPRESSIONS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/data/memcheck.supp")
include(CTest)

set(PROCESSOR_COUNT 1)
include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
  set(PROCESSOR_COUNT ${N})
endif()

if(COVERAGE_REPORT)
    include(CodeCoverage)
    APPEND_COVERAGE_COMPILER_FLAGS()
    set(COVERAGE_LCOV_INCLUDES '${CMAKE_CURRENT_SOURCE_DIR}/src/*')
    SETUP_TARGET_FOR_COVERAGE_LCOV(
        NAME
        coverage_report
        EXECUTABLE
        ctest -j ${PROCESSOR_COUNT}
    )
endif(COVERAGE_REPORT)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(apidoc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endif(DOXYGEN_FOUND)

include_directories(src)
add_subdirectory(src)

if(BUILD_TESTING)
    include(AddGoogleTest)
    add_subdirectory(tests)
endif()
