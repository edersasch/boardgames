import QtQuick 2.7

Rectangle {
    id: piece

    property var next_parent
    readonly property real parent_width: parent ? parent.width : 0
    readonly property real parent_height: parent ? parent.height : 0
    signal selected
    signal removed

    onNext_parentChanged: {
        var next_point;
        if (next_parent && parent !== next_parent) {
            if (parent) {
                next_point = piece.mapToItem(next_parent, 0, 0)
                anchors.horizontalCenter = undefined;
                anchors.verticalCenter = undefined;
                next_parent.z = parent.z
                parent.z = 0;
                x = next_point.x;
                y = next_point.y;
            }
            parent = next_parent;
        }
    }

    onParentChanged:        { xypos(); }
    onParent_widthChanged:  { adjxy(); }
    onParent_heightChanged: { adjxy(); }

    function adjxy() {
        if (xbeh.enabled) {
            xypos();
        }
    }

    function xypos() {
        var next_width = Math.min(parent_width, parent_height); // end value for computation needed now, same as future width
        var next_x = (parent_width - next_width) / 2;
        var next_y = (parent_height - next_width) / 2;
        if (x !== next_x || y !== next_y) {
            xbeh.enabled = true;
            ybeh.enabled = true;
            x = next_x;
            y = next_y;
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
    width: Math.min(parent_width, parent_height)
    height: width
    radius: width / 2
    border.color: "black"
    border.width: 1
    scale: 0.8
    opacity: 0
    z: 1

    Drag.hotSpot.x: width / 2
    Drag.hotSpot.y: height / 2
    Drag.active: mouse_area.drag.active

    states: [
        State {
            name: "lock"
            PropertyChanges {
                target: piece
                opacity: 1
            }
            PropertyChanges {
                target: mouse_area
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
                target: mouse_area
                drag.target: piece
                cursorShape: Qt.OpenHandCursor // looks the same as ClosedHandCursor, changing to CrossCursor shows effect
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
                target: remove_hint
                opacity: 1
            }
            PropertyChanges {
                target: mouse_area
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
            duration: 500
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
            duration: 500
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
        id: mouse_area
        enabled: true
        anchors.fill: parent
        drag.axis: Drag.XAndYAxis
        states: [
            State {
                when: mouse_area.pressed && mouse_area.drag.active
                PropertyChanges {
                    target: mouse_area
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
        id: remove_hint
        anchors.fill: parent
        opacity: 0
        z: -1

        Behavior on opacity {
            NumberAnimation {}
        }

        Rectangle {
            anchors.fill: parent
            color: "red"
            radius: 10
            opacity: remove_hint.opacity
        }
    }
}
