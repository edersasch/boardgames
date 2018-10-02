import QtQuick 2.7
import QtQuick.Layouts 1.3

Rectangle {
    property bool can_hide: true
    property real size_factor: 1
    property alias fields: fields

    id: box
    radius: 10
    border.width: 1
    Layout.fillWidth: true
    Layout.preferredHeight: (can_hide) ? 0 : width / fields.model * size_factor

    Behavior on Layout.preferredHeight {
        NumberAnimation {
            duration: 100
        }
    }

    RowLayout {
        anchors.fill: parent

        Repeater {
            id: fields

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                onZChanged: {
                    box.z = z * 2; // higher than the board with z = 1
                }
            }
        }
    }
}
