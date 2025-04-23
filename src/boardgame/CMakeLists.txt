project(boardgame)

include(prjlib)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE
	BOARDGAME_LIB
)
