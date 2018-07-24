import QtQuick 2.3
import QtQuick.Controls 2.3

ToolButton {
    id: move_button

    property int move_id: -1
    property color color
    property bool highlight: false

    signal request_set_move_id(int m_id)

    onColorChanged: bg.color = Qt.binding(function() { return Qt.darker(color, move_button.pressed ? 1.5 : 1.0); });

    onHighlightChanged: {
        bg.border.width = Qt.binding(function() { return highlight ? 2 : 0; });
    }

    onClicked: request_set_move_id(move_id)

    background: Rectangle {
        id: bg
        radius: 5
        border.color: "black"
        border.width: 0
    }
}
