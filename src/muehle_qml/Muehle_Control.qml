import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3

RowLayout {
    id: root

    width: 640
    height: 480
    spacing: 0
    anchors.fill: parent ? parent : undefined
    property alias board: board
    property alias current_engine_depth: engine_settings.engine_depth
    property alias current_engine_time_in_s: engine_settings.engine_time_in_s
    property alias engine_mode_depth: engine_settings.engine_mode_depth
    property alias engine_mode_time: engine_settings.engine_mode_time
    property alias white_color: white_color_select.color
    property alias black_color: black_color_select.color
    property alias move_list_visible: move_list_v_button.checked
    property alias setup_mode_active: v_setup.checked
    property alias info_visible: info.visible
    property bool white_engine_active: false
    property bool black_engine_active: false
    property bool white_player_active: false
    property bool black_player_active: false
    property bool confirm: false
    property string release_info: "0.0.0"
    readonly property bool hor_orientation: width >= height
    signal horizontal
    signal vertical
    signal new_game
    signal request_white_engine_active(bool is_active)
    signal request_black_engine_active(bool is_active)
    signal request_setup_mode_active(bool is_active)
    signal engine_depth(int depth)
    signal engine_time_in_s(int time_in_s)
    signal white_color_changed(string new_white_color)
    signal black_color_changed(string new_black_color)
    signal show_help()

    onHor_orientationChanged: {
        hor_orientation ? horizontal() : vertical()
    }

    onSetup_mode_activeChanged: {
        request_setup_mode_active(setup_mode_active);
    }

    onWhite_engine_activeChanged: {
        request_white_engine_active(white_engine_active);
    }

    onBlack_engine_activeChanged: {
        request_black_engine_active(black_engine_active);
    }

    ColumnLayout {
        spacing: 0
        visible: hor_orientation
        z: 100

        Confirm_Button {
            id: v_new_game

            flat: true
            direction: v_new_game.direction_right
            icon.source: "qrc:/refresh.svg"
            confirm: root.confirm
            onConfirmed: new_game()
        }

        Engine_Button {
            id: white_engine_v_button

            color: white_color
            border_color: "darkgreen"
            working: white_player_active
            checked: white_engine_active
            onCheckedChanged: white_engine_active = checked
        }

        Engine_Button {
            id: black_engine_v_button

            color: black_color
            border_color: "lightgreen"
            working: black_player_active
            checked: black_engine_active
            onCheckedChanged: black_engine_active = checked
        }

        ToolButton {
            id: move_list_v_button
            flat: true
            checkable: true
            icon.source: "qrc:/list.svg"
        }

        Confirm_Button {
            id: v_setup

            flat: true
            checkable: true
            direction: v_setup.direction_right
            icon.source: "qrc:/setup.svg"
            confirm: root.confirm
            onCheckedChanged: h_setup.checked = checked // why is this necessary?
        }

        ToolButton {
            id: settings_v_button
            flat: true
            checkable: true
            icon.source: "qrc:/settings.svg"
        }

        ToolButton {
            id: help_v_button
            flat: true
            icon.source: "qrc:/help.svg"
            onClicked: show_help()
        }

        Item {
            Layout.fillHeight: true
        }
    }

    ColumnLayout {
        spacing: 0

        RowLayout {
            spacing: 0
            visible: !hor_orientation
            z: 100

            Confirm_Button {
                id: h_new_game

                flat: true
                direction: h_new_game.direction_down
                icon.source: "qrc:/refresh.svg"
                confirm: root.confirm
                onConfirmed: new_game()
            }

            Engine_Button {
                id: white_engine_h_button

                color: white_color
                border_color: "darkgreen"
                working: white_player_active
                checked: white_engine_active
                onCheckedChanged: white_engine_active = checked
            }

            Engine_Button {
                id: black_engine_h_button

                color: black_color
                border_color: "lightgreen"
                working: black_player_active
                checked: black_engine_active
                onCheckedChanged: black_engine_active = checked
            }

            ToolButton {
                flat: true
                checkable: true
                icon.source: "qrc:/list.svg"
                checked: move_list_v_button.checked
                onCheckedChanged: move_list_v_button.checked = checked
            }

            Confirm_Button {
                id: h_setup

                flat: true
                checkable: true
                direction: h_setup.direction_down
                icon.source: "qrc:/setup.svg"
                confirm: root.confirm
                checked: v_setup.checked
                onCheckedChanged: v_setup.checked = checked
            }

            ToolButton {
                id: settings_h_button
                flat: true
                checkable: true
                icon.source: "qrc:/settings.svg"
                checked: settings_v_button.checked
                onCheckedChanged: settings_v_button.checked = checked
            }

            ToolButton {
                id: help_h_button
                flat: true
                icon.source: "qrc:/help.svg"
                onClicked: show_help()
            }

            Item {
                Layout.fillWidth: true
            }
        }

        Flickable {
            id: settings_flickable

            Layout.fillWidth: true
            implicitHeight: settings_v_button.checked ? engine_settings.height + 10 : 0
            contentHeight: contentItem.childrenRect.height
            clip: true
            boundsBehavior: Flickable.OvershootBounds
            ScrollBar.vertical: ScrollBar {}

            Behavior on implicitHeight { NumberAnimation { easing.type: Easing.OutBack; duration: 200 } }

            Flow {
                width: parent.width

                Engine_Settings {
                    id: engine_settings

                    onEngine_mode_depthChanged: {
                        if (engine_mode_depth) {
                            root.engine_depth(engine_depth);
                        }
                    }

                    onEngine_mode_timeChanged: {
                        if (engine_mode_time) {
                            root.engine_time_in_s(engine_time_in_s);
                        }
                    }

                    onEngine_depthChanged: root.engine_depth(engine_depth)
                    onEngine_time_in_sChanged: root.engine_time_in_s(engine_time_in_s)
                }

                Column {
                    HSL_Sliders {
                        id: white_color_select

                        saturation_visible: false
                        lightness_visible: false
                        lightness: 0.8
                        onColorChanged: {
                            root.white_color_changed(color);
                        }
                    }

                    HSL_Sliders {
                        id: black_color_select

                        saturation_visible: false
                        lightness_visible: false
                        lightness: 0.4
                        onColorChanged: {
                            root.black_color_changed(color);
                        }
                    }
                }
            }
        }

        Rectangle {
            id: info

            visible: false
            implicitHeight: visible ? infotext.implicitHeight * 2 : 0
            Layout.fillWidth: true
            Text {
                id: infotext

                text: "Muehle " + release_info + " / Alexander Eder";
                anchors.centerIn: parent
            }
        }

        Muehle_Board {
            id: board

            hor_orientation: root.width > root.height
            white_color: root.white_color
            black_color: root.black_color
            move_list_visible: move_list_v_button.checked

            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
