project(muehle_qml_game)
include(prjqt)
include(prjstandardpre)
qt_add_executable(${PROJECT_NAME} ${SRC_LIST} ${CMAKE_SOURCE_DIR}/data/pic.qrc)
include(prjstandardpost)

file(GLOB QML_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*.qml)
qt_add_qml_module(${PROJECT_NAME}
    URI src.muehle_qml_game
    QML_FILES ${QML_LIST}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    boardgame
    boardgame_qml
    muehle
    muehle_qml
    Qt::Quick
    ${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${CMAKE_SOURCE_DIR}/data/muehle.desktop DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
install(FILES ${CMAKE_SOURCE_DIR}/data/muehle_field_bright.svg DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/apps)

if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE true)
    set(QT_DEPLOY_TOOL_OPTIONS
        --no-system-d3d-compiler
        --no-opengl-sw
    )
endif()

qt_generate_deploy_qml_app_script(TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    DEPLOY_TOOL_OPTIONS ${QT_DEPLOY_TOOL_OPTIONS}
    NO_TRANSLATIONS
)
install(SCRIPT ${deploy_script})
