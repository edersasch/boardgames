import QtQuick
import QtQuick.Controls

ToolButton {
    id: move_button

    property int move_id: -1
    property color color
    property bool highlight: false
    readonly property int p_width: parent ? parent.width : 0

    signal request_set_move_id(int m_id)

    function scroll_to() {
        if (highlight && visible && parent && parent.parent) {
            parent.parent.scroll_to(y);
        }
    }

    onColorChanged: contentItem.color = color.hslLightness < 0.5 ? "white" : "black"
    onClicked: request_set_move_id(move_id)
    onHighlightChanged: scroll_to()
    onP_widthChanged: scroll_to()

    background: Rectangle {
        id: bg
        radius: 5
        color: Qt.darker(move_button.color, move_button.pressed ? 1.5 : 1.0)
        border.color: color.hslLightness < 0.5 ? "lightgreen" : "darkgreen";
        border.width: move_button.highlight ? 2 : 0
    }
}
