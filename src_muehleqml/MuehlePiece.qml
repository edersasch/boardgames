import QtQuick 2.7

Rectangle {
    id: piece
    signal selected
    signal removed
    property var nextParent
    onNextParentChanged: {
        if (parent && nextParent) {
            var oldscale = scale
            scale = 1
            xypos(mapFromItem(nextParent,
                              nextParent.width / 2 - width / 2 + x,
                              nextParent.height / 2 - height / 2 + y))
            scale = oldscale
        } else {
            parent = nextParent;
        }
    }

    function xypos(nextPoint) {
        anchors.horizontalCenter = undefined;
        anchors.verticalCenter = undefined;
        x = nextPoint.x
        y = nextPoint.y
        z = 1
    }

    anchors.horizontalCenter: parent ? parent.horizontalCenter : undefined
    anchors.verticalCenter: parent ? parent.verticalCenter : undefined;
    width: parent ? Math.min(parent.width, parent.height) / 1.3 : 0
    height: width
    radius: width / 2
    border.color: "black"
    border.width: 1
    opacity: 0
    scale: 0
    z: 0
    onZChanged: {
        if (parent) {
            parent.z = z;
        }
    }

    Drag.hotSpot.x: width / 2
    Drag.hotSpot.y: height / 2
    Drag.active: mouseArea.drag.active

    states: [
        State {
            name: "pin"
            PropertyChanges {
                target: piece
                opacity: 1
                scale: 1
            }
            PropertyChanges {
                target: mouseArea
                cursorShape: Qt.ArrowCursor
            }
        },
        State {
            name: "selectable"
            PropertyChanges {
                target: piece
                opacity: 1
                scale: 1.3
            }
            PropertyChanges {
                target: mouseArea
                drag.target: piece
                cursorShape: Qt.OpenHandCursor
                onClicked: { piece.selected() }
            }
        },
        State {
            name: "removable"
            PropertyChanges {
                target: piece
                opacity: 0.9
                scale: 1.3
            }
            PropertyChanges {
                target: mouseArea
                onClicked: { piece.removed() }
            }
        }
    ]
    transitions: [
        Transition {
            NumberAnimation { properties: "opacity, scale" }
        }
    ]

    Behavior on x {
        NumberAnimation {
            easing.type: Easing.OutElastic
            easing.period: 0.9
            onRunningChanged: {
                if (!running && parent) {
                    z = 0
                    if (nextParent && parent !== nextParent) {
                        parent = nextParent;
                    }
                    anchors.horizontalCenter = parent.horizontalCenter;
                    anchors.verticalCenter = parent.verticalCenter;
                }
            }
        }
    }
    Behavior on y {
        NumberAnimation {
            easing.type: Easing.OutElastic
            easing.period: 0.9
        }
    }
    Behavior on width {
        NumberAnimation {}
    }

     MouseArea {
        id: mouseArea
        enabled: true
        anchors.fill: parent
        drag.axis: Drag.XAndYAxis
        states: [
            State {
                when: mouseArea.pressed && mouseArea.drag.target === piece
                PropertyChanges {
                    target: mouseArea
                    cursorShape: Qt.ClosedHandCursor
                }
                StateChangeScript {
                    script: {
                        piece.z = 1
                        piece.anchors.horizontalCenter = undefined
                        piece.anchors.verticalCenter = undefined
                        piece.selected();
                    }
                }
            }
        ]
        onReleased: {
            if (piece.Drag.target) {
                piece.Drag.target.occupy();
            } else {
                if (drag.active && piece.parent) {
                    piece.xypos(Qt.point(piece.parent.width / 2 - piece.width / 2,
                                         piece.parent.height / 2 - piece.height / 2))
                }
            }
        }
    }
}
