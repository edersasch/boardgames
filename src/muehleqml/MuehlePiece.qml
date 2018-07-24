import QtQuick 2.7

Rectangle {
    id: piece
    signal selected
    signal removed
    property var nextParent
    readonly property real pWidth: parent ? parent.width : 0
    readonly property real pHeight: parent ? parent.height : 0
    onNextParentChanged: {
        var nextPoint;
        if (nextParent && parent !== nextParent) {
            if (parent) {
                nextPoint = piece.mapToItem(nextParent, 0, 0)
                anchors.horizontalCenter = undefined;
                anchors.verticalCenter = undefined;
                nextParent.z = parent.z
                parent.z = 0;
                x = nextPoint.x;
                y = nextPoint.y;
            }
            parent = nextParent;
        }
    }
    onParentChanged:  { xypos(); }
    onPWidthChanged:  { adjxy(); }
    onPHeightChanged: { adjxy(); }

    function adjxy() {
        if (xbeh.enabled) {
            xypos();
        }
    }

    function xypos() {
        var nextWidth = Math.min(pWidth, pHeight); // end value for computation needed now, same as future width
        var nextX = (pWidth - nextWidth) / 2;
        var nextY = (pHeight - nextWidth) / 2;
        if (x !== nextX || y !== nextY) {
            xbeh.enabled = true;
            ybeh.enabled = true;
            x = nextX;
            y = nextY;
            parent.z = 1;
        }
    }

    function anchorparent() {
        if (!xanim.running && !yanim.running && parent) {
            parent.z = 0;
            xbeh.enabled = false;
            ybeh.enabled = false;
            anchors.horizontalCenter = Qt.binding(function() { return parent.horizontalCenter; });
            anchors.verticalCenter = Qt.binding(function() { return parent.verticalCenter; });
        }
    }

    anchors.horizontalCenter: parent ? parent.horizontalCenter : undefined
    anchors.verticalCenter: parent ? parent.verticalCenter : undefined;
    width: Math.min(pWidth, pHeight)
    height: width
    radius: width / 2
    border.color: "black"
    border.width: 1
    scale: 0.8
    opacity: 0
    z: 1

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
                scale: 1
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
        id: xbeh
        enabled: false
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
        id: ybeh
        enabled: false
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
    Behavior on scale {
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
                        piece.parent.z = 1;
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
                    piece.xypos();
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
