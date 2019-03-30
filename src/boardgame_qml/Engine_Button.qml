import QtQuick 2.3
import QtQuick.Controls 2.3

ToolButton {
    id: button

    property color color: "grey"
    property bool working: false
    property color border_color: "black"

    flat: true
    checkable: true
    icon.source: "qrc:/engine.svg"
    icon.color: color.hslLightness > 0.5 ? "black" : "white"

    onColorChanged: {
        engine_working_animation.stop();
    }

    states: [
        State {
            name: "working"
            when: working && checked
            StateChangeScript {
                script: {
                    bg.color = button.color; // break binding to color without flicker
                }
            }
        }
    ]

    background: Rectangle {
        id: bg

        color: button.color
        border.width: button.checked ? 2 : 0
        border.color: button.border_color
    }

    SequentialAnimation {
        id: engine_working_animation

        running: button.state === "working"
        alwaysRunToEnd: true
        loops: Animation.Infinite
        ColorAnimation {
            target: bg
            properties: "color"
            from: button.color
            to: button.color.hslLightness > 0.5 ? "black" : "white"
            duration: 500
        }
        ColorAnimation {
            target: bg
            properties: "color"
            to: button.color
            duration: 1000
        }
        onStopped: {
            if (button.state === "working") {
                engine_working_animation.start();
            } else {
                bg.color = Qt.binding(function() { return button.color; });
            }
        }
    }
}
