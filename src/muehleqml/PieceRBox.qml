import QtQuick 2.7
import QtQuick.Layouts 1.3

Rectangle {
    property bool canHide: true
    property real sizeFactor: 1
    property alias fields: fields

    id: pieceRBox
    radius: 10
    border.width: 1
    Layout.fillWidth: true
    Layout.preferredHeight: (canHide) ? 0 : width / fields.model * sizeFactor

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
                    pieceRBox.z = z * 2; // higher than the board with z = 1
                }
            }
        }
    }
}
