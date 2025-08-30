import QtQuick

Rectangle {
    id: piece

    property int piece_id
    property var next_parent
    property color next_color
    readonly property real parent_width: parent ? parent.width : 0
    readonly property real parent_height: parent ? parent.height : 0
    readonly property real next_width: Math.min(parent_width, parent_height)
    readonly property real x_offset: (parent_width - next_width) / 2
    readonly property real y_offset: (parent_height - next_width) / 2
    signal selected(int p_id)
    signal removed(int p_id)

    onNext_parentChanged: {
        var next_point;
        var old_z;
        if (next_parent && parent !== next_parent) {
            if (parent) {
                next_point = piece.mapToItem(next_parent, 0, 0)
                old_z = parent.z;
                parent.z = 0;
                next_parent.z = old_z; // in case parent and next_parent have the same parent, set potential 1 after 0
                x = next_point.x;
                y = next_point.y;
            }
            parent = next_parent;
        }
    }

    onNext_colorChanged: {
        win_animation.stop();
    }

    onParentChanged: {
        xypos();
    }

    function xypos() {
        xbeh.enabled = x !== x_offset || y !== y_offset;
        x = Qt.binding(function() { return x_offset; });
        y = Qt.binding(function() { return y_offset; });
    }

    function anchorparent() {
        xbeh.enabled = xanim.running || yanim.running;
        if (parent) {
            parent.z = xbeh.enabled || mouse_area.drag.active ? 1 : 0;
        }
    }

    width: next_width
    height: width
    radius: width / 2
    border.color: "black"
    border.width: 1
    scale: 0.8
    opacity: 1
    color: next_color

    Drag.hotSpot.x: width / 2
    Drag.hotSpot.y: height / 2
    Drag.active: mouse_area.drag.active

    states: [
        State {
            name: "selectable"
            PropertyChanges {
                piece {
                    scale: 1
                }
            }
            PropertyChanges {
                mouse_area {
                    drag.target: piece
                    cursorShape: Qt.OpenHandCursor // looks the same as ClosedHandCursor, changing to CrossCursor shows effect
                    onClicked: { piece.selected(piece.piece_id) }
                }
            }
        },
        State {
            name: "removable"
            PropertyChanges {
                remove_hint {
                    opacity: 1
                }
            }
            PropertyChanges {
                mouse_area {
                    cursorShape: Qt.CrossCursor
                    onClicked: { piece.removed(piece.piece_id) }
                }
            }
        },
        State {
            name: "win"
            StateChangeScript {
                script: {
                    piece.color = piece.next_color; // break binding to next_color without flicker
                }
            }
        },
        State {
            name: "draw"
            PropertyChanges {
                piece {
                    scale: 0.3
                }
            }
        }
    ]

    SequentialAnimation {
        id: win_animation

        running: piece.state === "win"
        alwaysRunToEnd: true
        loops: Animation.Infinite
        ColorAnimation {
            target: piece
            properties: "color"
            from: piece.next_color
            to: piece.next_color.hslLightness > 0.5 ? "black" : "white"
            duration: 500
        }
        ColorAnimation {
            target: piece
            properties: "color"
            to: piece.next_color
            duration: 1000
        }
        onStopped: {
            if (piece.state === "win") {
                win_animation.start();
            } else {
                piece.color = Qt.binding(function() { return piece.next_color; });
            }
        }
    }

    Behavior on x {
        id: xbeh
        enabled: false
        NumberAnimation {
            id: xanim
            easing.type: Easing.OutElastic
            easing.period: 0.9
            duration: 500
            onRunningChanged: {
                piece.anchorparent()
            }
        }
    }
    Behavior on y {
        enabled: xbeh.enabled
        NumberAnimation {
            id: yanim
            easing.type: Easing.OutElastic
            easing.period: 0.9
            duration: 500
            onRunningChanged: {
                piece.anchorparent()
            }
        }
    }
    Behavior on width {
        enabled: xbeh.enabled
        NumberAnimation { duration: 100 }
    }
    Behavior on scale { NumberAnimation { duration: 100 } }
    Behavior on opacity { NumberAnimation { duration: 100 } }


    MouseArea {
        id: mouse_area
        enabled: true
        anchors.fill: parent
        drag.axis: Drag.XAndYAxis
        cursorShape: Qt.ArrowCursor
        states: [
            State {
                when: mouse_area.pressed && mouse_area.drag.active
                PropertyChanges {
                    mouse_area {
                        cursorShape: Qt.ClosedHandCursor
                    }
                }
                StateChangeScript {
                    script: {
                        if (piece.parent) {
                            piece.parent.z = 1;
                        }
                        piece.selected(piece.piece_id);
                    }
                }
            }
        ]
        onReleased: {
            const is_field = piece.Drag.target instanceof Field
            if (is_field) {
                (piece.Drag.target as Field).occupy((piece.Drag.target as Field).field_id);
            } else {
                if (drag.active) {
                    piece.xypos();
                }
            }
        }
    }

    Rectangle {
        id: remove_hint
        anchors.fill: parent
        color: "red"
        radius: 10
        opacity: 0
        scale: 1.1
        z: -1

        Behavior on opacity { NumberAnimation {} }
    }
}
