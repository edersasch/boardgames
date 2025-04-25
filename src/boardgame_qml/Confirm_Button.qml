import QtQuick
import QtQuick.Controls

Item {
    id: root

    readonly property string direction_down: "down"
    readonly property string direction_right: "right"
    property string direction: direction_down
    property bool confirm: true
    property bool checkable: false
    property bool checked: false
    property alias icon: trigger_button.icon
    property alias flat: trigger_button.flat

    signal confirmed()

    implicitWidth: cancel_button.implicitWidth
    implicitHeight: cancel_button.implicitHeight

    function target_x() {
        switch (direction) {
        case "right":
            return cancel_button.x + cancel_button.width;
        case "down":
        default:
            return cancel_button.x;
        }
    }

    function target_y() {
        switch (direction) {
        case "right":
            return cancel_button.y
        case "down":
        default:
            return cancel_button.y + cancel_button.height;
        }
    }

    onCheckedChanged: {
        if (checkable) {
            if (checked) {
                trigger_button.checkable = true;
                trigger_button.checked = true;
                confirmed();
            } else {
                trigger_button.checked = false;
                trigger_button.checkable = false;
            }
        }
    }

    Timer {
        id: auto_cancel

        interval: 3000
        onTriggered: trigger_button.state = ""
    }

    ToolButton {
        id: trigger_button
        background: Rectangle {
            id: bg
            opacity: trigger_button.down ? 1.0 : 0.5
            color: trigger_button.down || trigger_button.checked || trigger_button.highlighted ? trigger_button.palette.mid : trigger_button.palette.button

            Behavior on color { ColorAnimation {} }
        }

        onClicked: {
            if (root.checkable && root.checked) {
                root.checked = false;
            } else {
                if (!root.confirm || state === "show") {
                    state = "";
                    if (root.checkable) {
                        root.checked = true
                    } else {
                        root.confirmed()
                    }
                } else {
                    state = "show";
                }
            }
        }

        states: [
            State {
                name: "show"
                PropertyChanges {
                    trigger_button {
                        x: root.target_x()
                        y: root.target_y()
                    }
                }
                PropertyChanges {
                    bg {
                        opacity: 1
                        color: "red"
                        radius: 5
                    }
                }
                PropertyChanges {
                    cancel_button {
                        opacity: 1
                    }
                }
                PropertyChanges {
                    auto_cancel {
                        running: true
                    }
                }
            }
        ]
        transitions: [
            Transition {
                NumberAnimation { easing.type: Easing.OutBack; properties: "x, y, opacity, background.opacity, background.radius" }
            }
        ]
    }

    ToolButton {
        id: cancel_button

        flat: trigger_button.flat
        icon.source: "qrc:/cancel.svg"
        opacity: 0
        visible: opacity !== 0
        onClicked: {
            trigger_button.state = "";
        }
    }
}
