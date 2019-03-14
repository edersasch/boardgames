import QtQuick 2.3
import QtQuick.Controls 2.3

ToolButton {
    id: move_button

    property int move_id: -1
    property color color
    property bool highlight: false

    signal request_set_move_id(int m_id)

    onColorChanged: {
        contentItem.color = color.hslLightness < 0.5 ? "white" : "black";
    }

    onClicked: request_set_move_id(move_id)

    background: Rectangle {
        id: bg
        radius: 5
        color: Qt.darker(move_button.color, move_button.pressed ? 1.5 : 1.0)
        border.color: color.hslLightness < 0.5 ? "grey" : "black";
        border.width: highlight ? 4 : 0
    }
}
