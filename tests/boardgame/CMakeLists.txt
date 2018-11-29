project(boardgame_test)
include(prjtest)
target_link_libraries(${PROJECT_NAME} PRIVATE boardgame boardgames_muehle)
