import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Rectangle {
    id: engine_settings

    property alias engine_depth: depth_spinbox.value
    property alias engine_time_in_s: time_spinbox.value
    property alias engine_mode_depth: depth_button.checked
    property alias engine_mode_time: time_button.checked

    radius: 10
    border.width: 1
    border.color: "#000000"

    width: main_layout.implicitWidth + 4
    height: main_layout.implicitHeight

    ButtonGroup {
        id: engine_group
    }

    RowLayout {
        id: main_layout

        ColumnLayout {
            RowLayout {
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

            from: 1
            to: 100
            value: 6
            visible: depth_button.checked
        }

        SpinBox {
            id: time_spinbox

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
