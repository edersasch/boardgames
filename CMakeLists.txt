cmake_minimum_required(VERSION 3.11.0)
project(boardgames VERSION 0.1.1)

option(BOARDGAMES_BUILD_TESTS                       "Build unit tests, downloads google test"                   ON)
option(BOARDGAMES_COVERAGE_REPORT                   "Compile with coverage instrumentation"                     OFF)
option(BOARDGAMES_USE_EXTERNAL_JSON                 "Use external nlohmann json library, download otherwise"    OFF)
option(BOARDGAMES_USE_EXTERNAL_ROBIN_HOOD_HASH      "Use external robin_hood hash table, download otherwise"    OFF)
option(BOARDGAMES_USE_EXTERNAL_SML                  "Use external boost-ext sml, download otherwise"            OFF)

set(PROJECT_RELEASE_YEAR    2022)
set(PROJECT_RELEASE_MONTH   02)
set(PROJECT_RELEASE_DAY     28)
configure_file (
    "${CMAKE_SOURCE_DIR}/src/boardgame/version.h.in"
    "${CMAKE_BINARY_DIR}/include/boardgame/version.h"
    )

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if(BOARDGAMES_BUILD_TESTS)
    set(MEMORYCHECK_SUPPRESSIONS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/data/memcheck.supp")
    include(CTest)
endif()

include(ProcessorCount)
ProcessorCount(PROCESSOR_COUNT)
if(NOT PROCESSOR_COUNT)
    set(PROCESSOR_COUNT 1)
endif()

if(BOARDGAMES_COVERAGE_REPORT)
    include(CodeCoverage)
    append_coverage_compiler_flags()
    setup_target_for_coverage_lcov(
        NAME coverage_report
        EXECUTABLE ctest -j ${PROCESSOR_COUNT}
        EXCLUDE "src/*"
        )
endif()

set(DOXYGEN_EXCLUDE_PATTERNS "*/*.md")
include(FindDoxygen)
if(DOXYGEN_FOUND)
    doxygen_add_docs(
        apidoc
        ${CMAKE_SOURCE_DIR}
        )
endif()

include(GNUInstallDirs)

if(BOARDGAMES_USE_EXTERNAL_JSON)
    find_package(nlohmann_json 3.4.0 REQUIRED)
else()
    include(FetchContent)
    FetchContent_Declare(nlohmann_json
        URL                 https://github.com/nlohmann/json/releases/download/v3.4.0/include.zip
        URL_HASH            SHA256=bfec46fc0cee01c509cf064d2254517e7fa80d1e7647fea37cf81d97c5682bdc
        UPDATE_DISCONNECTED 1
        QUIET
        )
    FetchContent_GetProperties(nlohmann_json)
    if(NOT nlohmann_json_POPULATED)
        FetchContent_Populate(nlohmann_json)
    endif()
    include_directories(${nlohmann_json_SOURCE_DIR})
    FILE(DOWNLOAD https://raw.githubusercontent.com/nlohmann/json/v3.4.0/LICENSE.MIT ${CMAKE_BINARY_DIR}/docs/License_nlohmann-json.txt
        EXPECTED_HASH SHA256=7b875bb7a4e28abb7956ea821b4b7d88ff6a2a4fabf261c3993730a365ec56dd
        )
    install (FILES ${CMAKE_BINARY_DIR}/docs/License_nlohmann-json.txt DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/doc/boardgames)
endif()

if(NOT BOARDGAMES_USE_EXTERNAL_ROBIN_HOOD_HASH)
    FILE(DOWNLOAD https://raw.githubusercontent.com/martinus/robin-hood-hashing/3.11.5/src/include/robin_hood.h ${CMAKE_BINARY_DIR}/include/robin_hood.h
        EXPECTED_HASH SHA256=96d1da494c0cafd7f83d4531ab517b0d043f0fe9328a9a89e6c096f1cf8f23cf
        )
    FILE(DOWNLOAD https://raw.githubusercontent.com/martinus/robin-hood-hashing/3.11.5/LICENSE ${CMAKE_BINARY_DIR}/docs/License_martinus-robin-hood-hashing.txt
        EXPECTED_HASH SHA256=686cf8d62cc44fecff4be967238aba89665f9b6acf8a728987801ffc3a626bc7
        )
    install (FILES ${CMAKE_BINARY_DIR}/docs/License_martinus-robin-hood-hashing.txt DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/doc/boardgames)
endif()

if(NOT BOARDGAMES_USE_EXTERNAL_SML)
    FILE(DOWNLOAD https://raw.githubusercontent.com/boost-ext/sml/v1.1.3/include/boost/sml.hpp ${CMAKE_BINARY_DIR}/include/boost/sml.hpp
        EXPECTED_HASH SHA256=5b0c6e8a6199f986d5c81728b0af030777792f12d5da73d1928d0c28e6071a8f
        )
    FILE(DOWNLOAD https://raw.githubusercontent.com/boost-ext/sml/v1.1.4/LICENSE.md ${CMAKE_BINARY_DIR}/docs/License_boost-ext-sml.txt
        EXPECTED_HASH SHA256=c9bff75738922193e67fa726fa225535870d2aa1059f91452c411736284ad566
        )
    install (FILES ${CMAKE_BINARY_DIR}/docs/License_boost-ext-sml.txt DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/doc/boardgames)
endif()

FILE(DOWNLOAD https://raw.githubusercontent.com/qt/qt5/5.15.2/LICENSE.LGPLv3 ${CMAKE_BINARY_DIR}/docs/License_Qt.txt
    EXPECTED_HASH SHA256=5ceb37d1c7c1d92878b82af3c0fd5558087f3d5a08a3a4d43850bad4ad265a52
    )
install (FILES ${CMAKE_BINARY_DIR}/docs/License_Qt.txt DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/doc/boardgames)

find_program(PANDOC pandoc)
if(PANDOC)
    configure_file (
        "${CMAKE_SOURCE_DIR}/docs/manuals/version_footer.md.in"
        "${CMAKE_BINARY_DIR}/docs/manuals/version_footer.md"
        )
    # see https://cmake.org/pipermail/cmake/2011-June/044651.html
    # and https://stackoverflow.com/questions/38293535/generic-rule-from-makefile-to-cmake
    file(GLOB MANUALS_MD ${CMAKE_SOURCE_DIR}/docs/manuals/*.md)
    set(MANUALS_HTML)
    foreach(f IN LISTS MANUALS_MD)
        if(NOT IS_ABSOLUTE "${f}")
            set(f "${CMAKE_CURRENT_SOURCE_DIR}/${f}")
        endif()
        get_filename_component(fn "${f}" NAME_WE)
        set(o "${CMAKE_CURRENT_BINARY_DIR}/${fn}.html")
        add_custom_command(OUTPUT "${o}"
            COMMAND ${PANDOC} -o "${o}" -s --self-contained -c ${CMAKE_SOURCE_DIR}/data/pandoc.css "${f}" ${CMAKE_BINARY_DIR}/docs/manuals/version_footer.md
            DEPENDS "${f}"
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs/manuals
            COMMENT "Generating ${o}"
            VERBATIM
            )
        list(APPEND MANUALS_HTML "${o}")
        install(FILES "${o}" DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/doc/boardgames)
    endforeach()
    add_custom_target(manuals ALL
        DEPENDS "${MANUALS_HTML}"
        COMMENT "Generated HTML manuals with pandoc"
        VERBATIM
        )
endif()

include(FindThreads)

if(WIN32)
    include(DeployQt)
endif()

include_directories(src)
include_directories(${PROJECT_BINARY_DIR}/include)
add_subdirectory(src)

if(BOARDGAMES_BUILD_TESTS AND BUILD_TESTING)
    include(AddGoogleTest)
    add_subdirectory(tests)
endif()

install (FILES ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/doc/boardgames RENAME License.txt)
