import QtQuick 2.7
import QtQuick.Layouts 1.3

Rectangle {
    property bool canHide: true
    property real sizeFactor: 1
    property alias fields: fields

    id: pieceCBox
    radius: 10
    border.width: 1
    Layout.fillHeight: true
    Layout.preferredWidth: (canHide) ? 0 : height / fields.model * sizeFactor

    Behavior on Layout.preferredWidth {
        NumberAnimation {
            duration: 100
        }
    }

    ColumnLayout {
        anchors.fill: parent

        Repeater {
            id: fields

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                onZChanged: {
                    pieceCBox.z = z * 2; // higher than the board with z = 1
                }
            }
        }
    }
}
