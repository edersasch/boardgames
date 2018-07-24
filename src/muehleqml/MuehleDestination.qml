import QtQuick 2.7

DropArea {
    signal occupy
    property alias fieldcolor: field.color
    property alias fieldopacity: field.opacity
    property alias color: hint.color
    id: destination
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

    MuehlePiece {
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
