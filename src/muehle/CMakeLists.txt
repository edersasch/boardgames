project(muehle)
include(prjlib)
# -Wshadow problems in sml.hpp
target_compile_options(${PROJECT_NAME} PRIVATE -Wno-shadow)
target_link_libraries(${PROJECT_NAME} PRIVATE boardgame)
