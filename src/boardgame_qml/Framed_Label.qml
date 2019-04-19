import QtQuick 2.0
import QtQuick.Controls 2.3

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
