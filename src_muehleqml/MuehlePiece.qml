import QtQuick 2.7

Rectangle {
    id: piece
    signal selected
    signal removed
    property var nextParent
    property real npWidth: nextParent ? nextParent.width : 0
    property real npHeight: nextParent ? nextParent.height : 0
    property real wFactor: 0.8
    onNextParentChanged: {
        if (parent && nextParent) {
            anchors.horizontalCenter = undefined;
            anchors.verticalCenter = undefined;
            xypos();
        } else {
            parent = nextParent;
        }
    }
    onNpWidthChanged: {
        if (parent && nextParent && parent !== nextParent && (xanim.running || yanim.running)) {
            xypos();
        }
    }
    onNpHeightChanged: {
        if (parent && nextParent && parent !== nextParent && (xanim.running || yanim.running)) {
            xypos();
        }
    }

    function xypos() {
        var nextPoint = mapFromItem(nextParent,
                                    nextParent.width / 2 - width / 2 + x,
                                    nextParent.height / 2 - height / 2 + y)
        width = width;
        x = nextPoint.x;
        y = nextPoint.y;
        z = 1;
    }

    function anchorparent() {
        if (!xanim.running && !yanim.running) {
            z = 0;
            if (nextParent) {
                parent = nextParent;
            }
            if (parent) {
                anchors.horizontalCenter = Qt.binding(function() { return parent.horizontalCenter; });
                anchors.verticalCenter = Qt.binding(function() { return parent.verticalCenter; });
                width = Qt.binding(function() { return Math.min(parent.width, parent.height) * wFactor; });
            }
        }
    }

    anchors.horizontalCenter: parent ? parent.horizontalCenter : undefined
    anchors.verticalCenter: parent ? parent.verticalCenter : undefined;
    width: parent ? Math.min(parent.width, parent.height) * wFactor : 0
    height: width
    radius: width / 2
    border.color: "black"
    border.width: 1
    opacity: 0
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
                wFactor: 1
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
                opacity: 1
            }
            PropertyChanges {
                target: removehint
                opacity: 1
            }
            PropertyChanges {
                target: mouseArea
                onClicked: { piece.removed() }
            }
        }
    ]
    transitions: [
        Transition {
            NumberAnimation { properties: "opacity" }
        }
    ]

    Behavior on x {
        NumberAnimation {
            id: xanim
            easing.type: Easing.OutElastic
            easing.period: 0.9
            onRunningChanged: {
                anchorparent()
            }
        }
    }
    Behavior on y {
        NumberAnimation {
            id: yanim
            easing.type: Easing.OutElastic
            easing.period: 0.9
            onRunningChanged: {
                anchorparent()
            }
        }
    }
    Behavior on width {
        NumberAnimation { duration: 100 }
    }

    MouseArea {
        id: mouseArea
        enabled: true
        anchors.fill: parent
        drag.axis: Drag.XAndYAxis
        states: [
            State {
                when: mouseArea.pressed && mouseArea.drag.active
                PropertyChanges {
                    target: mouseArea
                    cursorShape: Qt.ClosedHandCursor
                }
                StateChangeScript {
                    script: {
                        piece.z = 1;
                        piece.anchors.horizontalCenter = undefined;
                        piece.anchors.verticalCenter = undefined;
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
                    if (nextParent !== piece.parent) {
                        nextParent = piece.parent;  // set to correct position in case of layout change
                    } else {
                        piece.xypos();
                    }
                }
            }
        }
    }

    Item {
        id: removehint
        anchors.fill: parent
        opacity: 0
        z: -1

        Behavior on opacity {
            NumberAnimation {}
        }

        Rectangle {
            id: rh1
            anchors.fill: parent
            color: "red"
            radius: 10
            opacity: removehint.opacity
        }
    }
}
