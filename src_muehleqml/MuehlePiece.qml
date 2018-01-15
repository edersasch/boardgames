import QtQuick 2.7

Rectangle {
    signal removed
    signal selected
    signal deselected
    id: piece
    onParentChanged: {
        if (parent) {
            anchors.horizontalCenter = Qt.binding(function() {return parent.horizontalCenter});
            anchors.verticalCenter = Qt.binding(function() {return parent.verticalCenter});
            width = Qt.binding(function() {return Math.min(parent.width, parent.height) / 1.3});
        } else {
            anchors.horizontalCenter = undefined;
            anchors.verticalCenter = undefined;
            width = 0
        }
    }
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
    Drag.onActiveChanged: {
        if (Drag.active === true && piece.state === "playerMoveable") {
            piece.selected();
        }
    }

    states: [
        State {
            name: "playerVisible"
            PropertyChanges {
                target: piece
                opacity: 1
                scale: 1
                z: 0
            }
        },
        State {
            name: "playerMoveable"
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
            name: "playerSelected"
            PropertyChanges {
                target: piece
                opacity: 1
                scale: 1.3
                z: 1
            }
            PropertyChanges {
                target: mouseArea
                drag.target: piece
                cursorShape: Qt.OpenHandCursor
                onClicked: { piece.deselected() }
            }
        },
        State {
            name: "removeable"
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
    MouseArea {
        id: mouseArea
        enabled: true
        anchors.fill: parent
        drag.axis: Drag.XAndYAxis
        onPressed: {
            if (piece.state === "playerSelected" || piece.state === "playerMoveable") {
                piece.anchors.horizontalCenter = undefined;
                piece.anchors.verticalCenter = undefined;
            }
            mouse.accepted = true;
        }
        onReleased: {
            if (drag.active === true) {
                if (piece.Drag.target !== null) {
                    piece.x = piece.Drag.target.drag.x;
                    piece.y = piece.Drag.target.drag.y;
                    piece.Drag.target.selected();
                } else {
                    if (piece.parent) {
                        piece.anchors.horizontalCenter = piece.parent.horizontalCenter;
                        piece.anchors.verticalCenter = piece.parent.verticalCenter;
                    }
                    piece.deselected();
                }
            }
        }
    }
}
