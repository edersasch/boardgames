project(muehle_qml)
include(prjqt)
qtquick_compiler_add_resources(SRC_LIST muehle.qrc ${CMAKE_SOURCE_DIR}/data/pic.qrc)
include(prjexe)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt5::Quick
    boardgame
    boardgame_qml
    muehle
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