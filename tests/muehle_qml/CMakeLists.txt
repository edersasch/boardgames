project(muehle_qml_test)
include(prjqt)
include(prjstandardpre)
qt_add_executable(${PROJECT_NAME} ${SRC_LIST} ${CMAKE_SOURCE_DIR}/data/pic.qrc)
include(prjstandardpost)
add_gtest(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    muehle_qml
    boardgame_qml
    Qt6::Quick
)

if(WIN32)
    windeployqt(${PROJECT_NAME})
endif()
