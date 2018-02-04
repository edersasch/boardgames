import QtQuick 2.7

DropArea {
    signal occupy
    property alias color: field.color
    id: destination
    anchors.fill: parent
    enabled: false
    MuehlePiece {
        id: field
        scale: 1
        states: [
            State {
                when: destination.enabled === true && destination.containsDrag === false
                PropertyChanges { target: field; opacity: 0.2 }
            },
            State {
                when: destination.containsDrag === true
                PropertyChanges { target: field; opacity: 1 }
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
