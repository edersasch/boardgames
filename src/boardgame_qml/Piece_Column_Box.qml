import QtQuick 2.7
import QtQuick.Layouts 1.3

Rectangle {
    property bool can_hide: true
    property real size_factor: 1
    property alias fields: fields

    id: box
    radius: 10
    border.width: 1
    Layout.fillHeight: true
    Layout.preferredWidth: (can_hide) ? 0 : height / fields.model * size_factor

    Behavior on Layout.preferredWidth {
        NumberAnimation { easing.type: Easing.OutBack; duration: 200 }
    }

    ColumnLayout {
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
