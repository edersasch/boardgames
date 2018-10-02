import QtQuick 2.7

DropArea {
    id: destination

    property alias field_color: field.color
    property alias field_opacity: field.opacity
    property alias hint_color: hint.color
    signal occupy

    anchors.fill: parent
    enabled: false

    Rectangle {
        id: field
        anchors.fill: parent
        opacity: 0
        radius: 10
        Behavior on opacity {
            NumberAnimation {}
        }
    }

    Piece {
        id: hint
        states: [
            State {
                when: destination.enabled === true && destination.containsDrag === false
                PropertyChanges { target: hint; opacity: 0.2 }
            },
            State {
                when: destination.containsDrag === true
                PropertyChanges { target: hint; opacity: 1 }
            }
        ]
        transitions: [
            Transition {
                NumberAnimation { properties: "opacity" }
            }
        ]
        Behavior on color {
            ColorAnimation {}
        }

        MouseArea {
            anchors.fill: parent
            onClicked: { destination.occupy() }
        }
    }
}
