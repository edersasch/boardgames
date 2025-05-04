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

add_custom_target(qmllint
    COMMAND ${CMAKE_SOURCE_DIR}/tools/qmllint_helper.sh ${CMAKE_BINARY_DIR}
    COMMENT "Run qmllint on all QML files"
    VERBATIM
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${CMAKE_SOURCE_DIR}/data/muehle.desktop DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
install(FILES ${CMAKE_SOURCE_DIR}/data/muehle_field_bright.svg DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/apps)

if (WIN32)
    set(QT_DEPLOY_TOOL_OPTIONS
        --no-system-d3d-compiler
        --no-opengl-sw
        --qmldir ${CMAKE_BINARY_DIR}/src/muehle_qml
        --qmldir ${CMAKE_BINARY_DIR}/src/boardgame_qml
    )
endif()

qt_generate_deploy_qml_app_script(TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_TRANSLATIONS
)
install(SCRIPT ${deploy_script})
