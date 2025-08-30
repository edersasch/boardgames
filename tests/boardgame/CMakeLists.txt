project(boardgame_test)
include(prjtest)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # problems in robin_hood.h
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-array-bounds -Wno-stringop-overflow)
endif()
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    boardgame
    muehle
)
