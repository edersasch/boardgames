import src.boardgame_qml

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

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
    property alias game_info_visible: info_v_button.checked
    property alias no_help_available_visible: no_help_available.visible
    property bool white_engine_active: false
    property bool black_engine_active: false
    property bool white_player_active: false
    property bool black_player_active: false
    property bool confirm: false
    property bool time_accounting_correct: true
    property int movecount: 0
    property string white_time: "0:00"
    property string black_time: "0:00"
    property string engine_forecast1: ""
    property string engine_forecast2: ""
    property string engine_forecast3: ""
    property int engine_score: 0
    property int reached_depth: 0
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
        visible: root.hor_orientation
        z: 100

        Confirm_Button {
            id: v_new_game

            flat: true
            direction: v_new_game.direction_right
            icon.source: "qrc:/refresh.svg"
            confirm: root.confirm
            onConfirmed: root.new_game()
        }

        Engine_Button {
            id: white_engine_v_button

            color: root.white_color
            border_color: "darkgreen"
            working: root.white_player_active
            checked: root.white_engine_active
            onCheckedChanged: root.white_engine_active = checked
        }

        Engine_Button {
            id: black_engine_v_button

            color: root.black_color
            border_color: "lightgreen"
            working: root.black_player_active
            checked: root.black_engine_active
            onCheckedChanged: root.black_engine_active = checked
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
            direction: direction_right
            icon.source: "qrc:/setup.svg"
            confirm: root.confirm
            onCheckedChanged: h_setup.checked = checked
        }

        ToolButton {
            id: info_v_button
            flat: true
            checkable: true
            icon.source: "qrc:/info.svg"
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
            onClicked: root.show_help()
        }

        Item {
            Layout.fillHeight: true
        }
    }

    ColumnLayout {
        id: h_layout
        spacing: 0

        RowLayout {
            spacing: 0
            visible: !root.hor_orientation
            z: 100

            Confirm_Button {
                id: h_new_game

                flat: true
                direction: h_new_game.direction_down
                icon.source: "qrc:/refresh.svg"
                confirm: root.confirm
                onConfirmed: root.new_game()
            }

            Engine_Button {
                id: white_engine_h_button

                color: root.white_color
                border_color: "darkgreen"
                working: root.white_player_active
                checked: root.white_engine_active
                onCheckedChanged: white_engine_active = checked
            }

            Engine_Button {
                id: black_engine_h_button

                color: root.black_color
                border_color: "lightgreen"
                working: root.black_player_active
                checked: root.black_engine_active
                onCheckedChanged: root.black_engine_active = checked
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
                direction: direction_down
                icon.source: "qrc:/setup.svg"
                confirm: root.confirm
                checked: v_setup.checked
                onCheckedChanged: v_setup.checked = checked
            }

            ToolButton {
                id: info_h_button
                flat: true
                checkable: true
                icon.source: "qrc:/info.svg"
                checked: info_v_button.checked
                onCheckedChanged: info_v_button.checked = checked
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
                onClicked: root.show_help()
            }

            Item {
                Layout.fillWidth: true
            }
        }

        Flickable {
            id: settings_flickable

            Layout.fillWidth: true
            implicitHeight: settings_v_button.checked ? contentHeight : 0
            contentHeight: contentItem.childrenRect.height + topMargin + bottomMargin
            clip: true
            leftMargin: 10
            rightMargin: 10
            topMargin: 10
            bottomMargin: 10

            Behavior on implicitHeight { NumberAnimation { easing.type: Easing.OutBack; duration: 200 } }

            Flow {
                width: parent.width
                spacing: 10

                RowLayout {
                    Image {
                        Layout.margins: 16
                        source: "qrc:/settings.svg"
                    }

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

                    Rectangle {
                        radius: 10
                        border.width: 1
                        border.color: "#000000"

                        implicitWidth: color_slider_column.width + 20
                        Layout.fillHeight: true


                        Column {
                            id: color_slider_column
                            spacing: 10

                            anchors.centerIn: parent

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
            }
        }

        Rectangle {
            id: no_help_available

            visible: false
            implicitHeight: visible ? infotext.implicitHeight * 2 : 0
            Layout.fillWidth: true
            Text {
                id: infotext

                text: "Muehle " + root.release_info + " / Alexander Eder";
                anchors.centerIn: parent
            }
        }

        Flickable {
            id: game_info_flickable

            Layout.fillWidth: true
            implicitHeight: info_v_button.checked ? contentHeight : 0
            contentHeight: contentItem.childrenRect.height + topMargin + bottomMargin
            clip: true
            leftMargin: 10
            rightMargin: 10
            topMargin: 10
            bottomMargin: 10

            Behavior on implicitHeight { NumberAnimation { easing.type: Easing.OutBack; duration: 200 } }

            Flow {
                width: parent.width
                spacing: 10

                RowLayout {
                    Image {
                        Layout.margins: 16
                        source: "qrc:/info.svg"
                    }

                    Framed_Label {
                        padding: 20
                        text: "# " + root.movecount
                    }

                    Rectangle {
                        implicitWidth: childrenRect.width
                        implicitHeight: childrenRect.height
                        radius: 10
                        border.color: "black"
                        border.width: 1

                        RowLayout {
                            Image {
                                visible: !root.time_accounting_correct
                                source: "qrc:/warning.svg"
                                Layout.margins: 10
                            }

                            Framed_Label {
                                text: root.white_time
                                color: root.white_color.hslLightness > 0.5 ? "black" : "white"
                                Layout.margins: 10
                                bg.color: root.white_color
                            }

                            Framed_Label {
                                text: root.black_time
                                color: root.black_color.hslLightness > 0.5 ? "black" : "white"
                                Layout.margins: 10
                                bg.color: root.black_color
                            }
                        }
                    }

                    Rectangle {
                        implicitWidth: childrenRect.width
                        visible: root.engine_forecast1.length && ((root.white_player_active && root.white_engine_active) || (root.black_player_active && root.black_engine_active))
                        implicitHeight: childrenRect.height
                        radius: 10
                        border.color: "black"
                        border.width: 1

                        RowLayout {
                            Image {
                                source: "qrc:/engine.svg"
                                Layout.margins: 10
                            }
                            Framed_Label {
                                text: "+ " + root.engine_score * (root.engine_score >= 0 ? 1 : -1)
                                color: bg.color.hslLightness > 0.5 ? "black" : "white"
                                Layout.margins: 10
                                bg.color: root.engine_score >= 0 ? (root.white_player_active ? root.white_color : root.black_color) : (root.white_player_active ? root.black_color : root.white_color)
                            }
                            Image {
                                source: "qrc:/depth.svg"
                                Layout.margins: 10
                            }
                            Text {
                                text: root.reached_depth
                            }
                            Framed_Label {
                                text: root.engine_forecast1
                                color: bg.color.hslLightness > 0.5 ? "black" : "white"
                                Layout.margins: 10
                                bg.color: root.white_player_active ? root.white_color : root.black_color
                            }
                            Framed_Label {
                                text: root.engine_forecast2
                                color: bg.color.hslLightness > 0.5 ? "black" : "white"
                                Layout.margins: 10
                                bg.color: root.white_player_active ? root.black_color : root.white_color
                            }
                            Framed_Label {
                                text: root.engine_forecast3
                                color: bg.color.hslLightness > 0.5 ? "black" : "white"
                                Layout.margins: 10
                                bg.color: root.white_player_active ? root.white_color : root.black_color
                            }
                        }
                    }
                }
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
