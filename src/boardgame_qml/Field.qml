import QtQuick 2.7

DropArea {
    id: destination

    property int field_id
    property var normal_parent
    property var alternative_parent
    property bool use_alternative_parent: false
    property alias hint_color: hint.color
    signal occupy(int f_id)

    anchors.fill: parent
    enabled: false

    onUse_alternative_parentChanged: {
        var next_parent;
        var old_parent = parent;
        if (parent && normal_parent && alternative_parent) {
            next_parent = use_alternative_parent ? alternative_parent : normal_parent;
            while (old_parent.children.length) {
                old_parent.children[0].parent = next_parent;
            }
        }
    }

    states: [
        State {
            name: "occupiable"
            PropertyChanges {
                target: destination
                enabled: true
            }
            PropertyChanges {
                target: field
                opacity: 0
            }
        },
        State {
            name: "highlight"
            PropertyChanges {
                target: field
                color: "green"
                opacity: 0.5
            }
        }
    ]

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
            onClicked: { destination.occupy(destination.field_id) }
        }
    }
}
