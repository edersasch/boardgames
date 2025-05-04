project(muehle)
include(prjlib)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # -Wshadow problems in sml.hpp
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-shadow)
endif()
target_compile_definitions(${PROJECT_NAME}
	PRIVATE
	MUEHLE_LIB
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    boardgame
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
