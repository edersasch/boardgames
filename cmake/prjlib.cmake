include(prjstandardpre)
add_library(${PROJECT_NAME} ${SRC_LIST})
target_include_directories(${PROJECT_NAME}
    PUBLIC .
)
include(prjstandardpost)
