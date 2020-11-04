project(muehle)
include(prjlib)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # -Wshadow problems in sml.hpp
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-shadow)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE boardgame)
