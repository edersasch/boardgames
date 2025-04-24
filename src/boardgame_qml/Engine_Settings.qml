import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: engine_settings

    property alias engine_depth: depth_spinbox.value
    property alias engine_time_in_s: time_spinbox.value
    property alias engine_mode_depth: depth_button.checked
    property alias engine_mode_time: time_button.checked

    radius: 10
    border.width: 1
    border.color: "#000000"

    width: childrenRect.width
    height: childrenRect.height

    ButtonGroup {
        id: engine_group
    }

    RowLayout {
        id: main_layout

        ColumnLayout {

            Layout.margins: 10

            RowLayout {
                id: depth_layout

                RadioButton {
                    id: depth_button

                    checked: true
                    ButtonGroup.group: engine_group
                }

                Image {
                    source: "qrc:/engine.svg"
                }

                Image {
                    source: "qrc:/depth.svg"
                }
            }

            RowLayout {
                id: time_layout

                RadioButton {
                    id: time_button

                    checked: false
                    ButtonGroup.group: engine_group
                }

                Image {
                    source: "qrc:/engine.svg"
                }

                Image {
                    source: "qrc:/time.svg"
                }
            }
        }

        SpinBox {
            id: depth_spinbox

            Layout.rightMargin: 10

            from: 1
            to: 100
            value: 6
            visible: depth_button.checked
        }

        SpinBox {
            id: time_spinbox

            Layout.rightMargin: 10

            from: 1
            to: 999
            value: 4
            visible: time_button.checked
            textFromValue: function(value, locale) {
                return value + "s"
            }
        }
    }
}
