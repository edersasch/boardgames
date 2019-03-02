cmake_minimum_required(VERSION 3.5.1)
project(boardgames)

option(BOARDGAMES_BUILD_TESTS "Build unit tests" ON)
option(BOARDGAMES_COVERAGE_REPORT "Compile with coverage instrumentation" OFF)
option(BOARDGAMES_USE_EXTERNAL_JSON "Use an external JSON library" OFF)

add_compile_options(
    -Wall -Wextra -Werror -Wshadow
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if(BOARDGAMES_BUILD_TESTS)
    set(MEMORYCHECK_SUPPRESSIONS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/data/memcheck.supp")
    include(CTest)
endif()

set(PROCESSOR_COUNT 1)
include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
    set(PROCESSOR_COUNT ${N})
endif()

if(BOARDGAMES_COVERAGE_REPORT)
    include(CodeCoverage)
    APPEND_COVERAGE_COMPILER_FLAGS()
    set(COVERAGE_LCOV_INCLUDES '${CMAKE_CURRENT_SOURCE_DIR}/src/*')
    SETUP_TARGET_FOR_COVERAGE_LCOV(
        NAME
        coverage_report
        EXECUTABLE
        ctest -j ${PROCESSOR_COUNT}
    )
endif()

set(DOXYGEN_EXCLUDE_PATTERNS "*/*.md")
include(FindDoxygen)
find_package(Doxygen)
if(DOXYGEN_FOUND)
    doxygen_add_docs(
        apidoc
        ${PROJECT_SOURCE_DIR}
    )
endif()

if(BOARDGAMES_USE_EXTERNAL_JSON)
    find_package(nlohmann_json 3.4.0 REQUIRED)
else()
    include(DownloadProject)
    download_project(PROJ   nlohmann_json
        GIT_REPOSITORY      https://github.com/nlohmann/json.git
        GIT_TAG             v3.4.0
        UPDATE_DISCONNECTED 1
        QUIET
    )
    set(JSON_BuildTests OFF CACHE INTERNAL "")
    add_subdirectory(${nlohmann_json_SOURCE_DIR} ${nlohmann_json_BINARY_DIR})
endif()

include_directories(src)
add_subdirectory(src)

if(BOARDGAMES_BUILD_TESTS AND BUILD_TESTING)
    include(AddGoogleTest)
    add_subdirectory(tests)
endif()
