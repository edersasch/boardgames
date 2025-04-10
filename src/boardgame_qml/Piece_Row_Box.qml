import QtQuick
import QtQuick.Layouts

Rectangle {
    property bool can_hide: true
    property real size_factor: 1
    property alias fields: fields

    id: box
    radius: 10
    border.width: 1
    Layout.fillWidth: true
    Layout.preferredHeight: 0

    states: [
        State {
            when: box.can_hide === false
            PropertyChanges {
                target: box
                Layout.preferredHeight: width / fields.model * size_factor
            }
        }
    ]
    transitions: [
        Transition {
            NumberAnimation {
                properties: "Layout.preferredHeight"
                easing.type: Easing.OutBack
                duration: 200
            }
        }
    ]

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
