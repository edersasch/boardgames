project(boardgames_test_gmock)
list(APPEND SRC_LIST "/usr/src/gmock/src/gmock-all.cc" "/usr/src/gtest/src/gtest-all.cc")
include(prjlib)
target_include_directories(${PROJECT_NAME}
    PUBLIC /usr/src/gmock
    PUBLIC /usr/src/gtest
)
target_link_libraries(${PROJECT_NAME}
    pthread
)
