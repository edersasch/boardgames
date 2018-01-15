project(boardgames_test_muehle)
include(prjtest)
target_link_libraries(${PROJECT_NAME}
    boardgames_muehle
    boardgames_test_gmock
)
