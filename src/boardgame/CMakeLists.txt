project(boardgame)

include(prjlib)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE
	BOARDGAME_LIB
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
