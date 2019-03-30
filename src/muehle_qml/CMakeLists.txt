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
    pthread
)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${CMAKE_SOURCE_DIR}/data/muehle.desktop DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
install(FILES ${CMAKE_SOURCE_DIR}/data/engine.svg DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/apps)
