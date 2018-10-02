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

include(FindDoxygen)
find_package(Doxygen)
doxygen_add_docs(
    apidoc
    ${PROJECT_SOURCE_DIR}
)

include_directories(src)
add_subdirectory(src)

if(BUILD_TESTING)
    include(AddGoogleTest)
    add_subdirectory(tests)
endif()
