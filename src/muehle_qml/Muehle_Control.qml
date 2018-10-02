import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3

RowLayout {
    width: 640
    height: 480
    spacing: 0
    anchors.fill: parent ? parent : undefined
    property alias board: board
    property color white_engine_color
    property color black_engine_color
    property bool setup_mode_active: false
    property bool white_engine_active: false
    property bool black_engine_active: false
    property bool white_player_active: false
    property bool black_player_active: false
    readonly property bool hor_orientation: width >= height
    signal horizontal
    signal vertical
    signal new_game
    signal show_move_list(bool show)
    signal request_white_engine_active(bool is_active)
    signal request_black_engine_active(bool is_active)
    signal request_setup_mode_active(bool is_active)

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

    NumberAnimation {
        targets: [
            white_v_bg,
            white_h_bg
        ]
        properties: "border.width"
        from: 4
        to: 2
        alwaysRunToEnd: true
        loops: Animation.Infinite
        running: white_engine_active && white_player_active
        onStopped: {
            white_v_bg.border.width = Qt.binding(function() { return white_engine_v_button.checked ? 2 : 0; })
            white_h_bg.border.width = Qt.binding(function() { return white_engine_v_button.checked ? 2 : 0; })
        }
   }

    NumberAnimation {
        targets: [
            black_v_bg,
            black_h_bg
        ]
        properties: "border.width"
        from: 4
        to: 2
        alwaysRunToEnd: true
        loops: Animation.Infinite
        running: black_engine_active && black_player_active
        onStopped: {
            black_v_bg.border.width = Qt.binding(function() { return black_engine_v_button.checked ? 2 : 0; })
            black_h_bg.border.width = Qt.binding(function() { return black_engine_v_button.checked ? 2 : 0; })
        }
   }

    ColumnLayout {
        spacing: 0
        visible: hor_orientation
        z: 100

        ToolButton {
            flat: true
            icon.source: "qrc:/refresh.svg"
            onClicked: new_game()
        }

        ToolButton {
            id: move_list_v_button
            flat: true
            checkable: true
            icon.source: "qrc:/list.svg"
            onCheckedChanged: show_move_list(checked)
        }

        ToolButton {
            flat: true
            checkable: true
            icon.source: "qrc:/setup.svg"
            checked: setup_mode_active;
            onCheckedChanged: setup_mode_active = checked
        }

        ToolButton {
            id: white_engine_v_button
            flat: true
            checkable: true
            icon.source: "qrc:/engine.svg"
            checked: white_engine_active
            onCheckedChanged: white_engine_active = checked

            background: Rectangle {
                id: white_v_bg
                color: white_engine_color
                border.color: "green"
                border.width: white_engine_v_button.checked ? 2 : 0
            }
        }

        ToolButton {
            id: black_engine_v_button
            flat: true
            checkable: true
            icon.source: "qrc:/engine.svg"
            checked: black_engine_active
            onCheckedChanged: black_engine_active = checked

            background: Rectangle {
                id: black_v_bg
                color: black_engine_color
                border.color: "green"
                border.width: black_engine_v_button.checked ? 2 : 0
            }
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

            ToolButton {
                flat: true
                icon.source: "qrc:/refresh.svg"
                onClicked: new_game()
            }

            ToolButton {
                flat: true
                checkable: true
                icon.source: "qrc:/list.svg"
                checked: move_list_v_button.checked
                onCheckedChanged: move_list_v_button.checked = checked
            }

            ToolButton {
                flat: true
                checkable: true
                icon.source: "qrc:/setup.svg"
                checked: setup_mode_active;
                onCheckedChanged: setup_mode_active = checked;
            }

            ToolButton {
                id: white_engine_h_button
                flat: true
                checkable: true
                icon.source: "qrc:/engine.svg"
                checked: white_engine_active
                onCheckedChanged: white_engine_active = checked

                background: Rectangle {
                    id: white_h_bg
                    color: white_engine_color
                    border.color: "green"
                    border.width: white_engine_h_button.checked ? 2 : 0
                }
            }

            ToolButton {
                id: black_engine_h_button
                flat: true
                checkable: true
                icon.source: "qrc:/engine.svg"
                checked: black_engine_active
                onCheckedChanged: black_engine_active = checked

                background: Rectangle {
                    id: black_h_bg
                    color: black_engine_color
                    border.color: "green"
                    border.width: black_engine_h_button.checked ? 2 : 0
                }
            }

            Item {
                Layout.fillWidth: true
            }
        }

        Item {
            id: board
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
