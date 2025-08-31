cmake_minimum_required(VERSION 3.21.0)
project(boardgames VERSION 0.1.2)

option(BOARDGAMES_BUILD_TESTS                       "Build unit tests, downloads google test"                   ON)
option(BOARDGAMES_COVERAGE_REPORT                   "Compile with coverage instrumentation"                     OFF)
set(PROJECT_RELEASE_YEAR    2025)
set(PROJECT_RELEASE_MONTH   99)
set(PROJECT_RELEASE_DAY     99)

configure_file (
    "${CMAKE_SOURCE_DIR}/src/boardgame/version.h.in"
    "${CMAKE_BINARY_DIR}/include/boardgame/version.h"
    )
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(BUILD_TESTING ${BOARDGAMES_BUILD_TESTS})

include(GNUInstallDirs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
include(CPM)
CPMUsePackageLock(package-lock.cmake)

if(BOARDGAMES_COVERAGE_REPORT)
    include(ProcessorCount)
    ProcessorCount(PROCESSOR_COUNT)
    if(NOT PROCESSOR_COUNT)
        set(PROCESSOR_COUNT 1)
    endif()

    include(CodeCoverage)
    append_coverage_compiler_flags()
    setup_target_for_coverage_lcov(
        NAME coverage_report
        EXECUTABLE ctest -j ${PROCESSOR_COUNT}
        BASE_DIRECTORY ${CMAKE_SOURCE_DIR}/src
        EXCLUDE ${CMAKE_SOURCE_DIR}/src
        LCOV_ARGS --ignore-errors mismatch,empty
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

CPMGetPackage(nlohmann_json)
include_directories(${nlohmann_json_SOURCE_DIR}/include)
install (FILES ${nlohmann_json_SOURCE_DIR}/LICENSE.MIT DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/doc/boardgames RENAME License_nlohmann_json.txt)

CPMGetPackage(martinus_robin-hood-hashing)
include_directories(${martinus_robin-hood-hashing_SOURCE_DIR}/src/include)
install (FILES ${martinus_robin-hood-hashing_SOURCE_DIR}/LICENSE DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/doc/boardgames RENAME License_martinus_robin-hood-hashing.txt)

CPMGetPackage(boost-ext_sml)
include_directories(${boost-ext_sml_SOURCE_DIR}/include)
install (FILES ${boost-ext_sml_SOURCE_DIR}/LICENSE.md DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/doc/boardgames RENAME License_boost-ext-sml.txt)

# Qt sbom https://doc.qt.io/qt-6/sbom.html
# Ubuntu noble copyright file https://changelogs.ubuntu.com/changelogs/pool/universe/q/qt6-base/qt6-base_6.4.2+dfsg-21.1build5/copyright

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
            # Has to work with Ubuntu 22.04 pandoc 2.9.2, no --embed-resources
            COMMAND ${PANDOC} --from markdown+gfm_auto_identifiers -o "${o}" -s --self-contained --standalone --metadata pagetitle=${fn} -c ${CMAKE_SOURCE_DIR}/data/pandoc.css "${f}" ${CMAKE_BINARY_DIR}/docs/manuals/version_footer.md
            #COMMAND ${PANDOC} --from markdown+gfm_auto_identifiers -o "${o}" -s --embed-resources --standalone --metadata pagetitle=${fn} -c ${CMAKE_SOURCE_DIR}/data/pandoc.css "${f}" ${CMAKE_BINARY_DIR}/docs/manuals/version_footer.md
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

include_directories(src)
include_directories(${CMAKE_BINARY_DIR}/include)
add_subdirectory(src)

if(BOARDGAMES_BUILD_TESTS AND BUILD_TESTING)
    include(CTest)
    include(AddGoogleTest)
    set(MEMORYCHECK_SUPPRESSIONS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/data/memcheck.supp")
    add_subdirectory(tests)
endif()

install (FILES ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/doc/boardgames RENAME License.txt)
