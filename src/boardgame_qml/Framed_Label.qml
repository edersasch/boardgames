import QtQuick
import QtQuick.Controls

Label {
    property alias bg: bg
    padding: 10
    background: Rectangle {
        id: bg
        border.color: "black"
        border.width: 1
        radius: 10
    }
}
