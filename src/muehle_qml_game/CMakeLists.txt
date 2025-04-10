project(muehle_qml_game)
include(prjqt)
include(prjstandardpre)
qt_add_executable(${PROJECT_NAME} ${SRC_LIST} ${CMAKE_SOURCE_DIR}/data/pic.qrc)
include(prjstandardpost)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    boardgame
    boardgame_qml
    muehle
    muehle_qml
    Qt6::Quick
    ${CMAKE_THREAD_LIBS_INIT}
)

if(WIN32)
    windeployqt(${PROJECT_NAME})
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE true)
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${CMAKE_SOURCE_DIR}/data/muehle.desktop DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
install(FILES ${CMAKE_SOURCE_DIR}/data/muehle_field_bright.svg DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/apps)
if(WIN32)
    install(FILES ${CMAKE_SOURCE_DIR}/tools/muehle.vbs DESTINATION ${CMAKE_INSTALL_PREFIX})
    set(RUNWINDEPLOYQT "execute_process(COMMAND ${WINDEPLOYQT_EXECUTABLE}
                --qmldir ${CMAKE_SOURCE_DIR}/src/muehle_qml
                --qmldir ${CMAKE_SOURCE_DIR}/src/boardgame_qml
                --verbose 0
                --no-compiler-runtime
                --no-angle
                --no-opengl-sw
                ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}.exe)")
    install(CODE ${RUNWINDEPLOYQT})
endif()
