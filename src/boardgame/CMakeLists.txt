project(boardgame)
include(prjlib)
target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json)
