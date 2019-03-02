import QtQuick 2.3
import QtQuick.Controls 2.3

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

    Timer {
        id: auto_cancel

        interval: 3000
        onTriggered: trigger_button.state = ""
    }

    ToolButton {
        id: trigger_button

        checked: root.checked

        onClicked: {
            if (!root.checkable) {
                if (!root.confirm || state === "show") {
                    root.confirmed();
                    state = "";
                } else {
                    state = "show";
                }
            } else {
                if (root.checked) {
                    root.checked = false;
                    checkable = false;
                } else {
                    if (!root.confirm || state === "show") {
                        root.confirmed()
                        state = "";
                        checkable = true;
                        root.checked = true;
                    } else {
                        state = "show";
                    }
                }
            }
        }

        states: [
            State {
                name: "show"
                PropertyChanges {
                    target: trigger_button
                    background.opacity: 1
                    background.color: "red"
                    background.radius: 5
                    x: root.target_x()
                    y: root.target_y()
                }
                PropertyChanges {
                    target: cancel_button
                    opacity: 1
                }
                PropertyChanges {
                    target: auto_cancel
                    running: true
                }
            }
        ]
        transitions: [
            Transition {
                NumberAnimation { easing.type: Easing.OutBack; properties: "x, y, opacity, background.opacity, background.radius" }
                ColorAnimation {}
            }
        ]
    }

    ToolButton {
        id: cancel_button

        flat: trigger_button.flat
        icon.source: "qrc:/cancel.svg"
        opacity: 0
        visible: opacity != 0
        onClicked: {
            trigger_button.state = "";
        }
    }
}
