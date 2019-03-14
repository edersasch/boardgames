import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Rectangle {
    id: hsl_sliders

    property alias hue: hue_slider.value
    property alias hue_from: hue_slider.from
    property alias hue_to: hue_slider.to
    property alias hue_visible: hue_slider.visible
    property alias saturation: saturation_slider.value
    property alias saturation_from: saturation_slider.from
    property alias saturation_to: saturation_slider.to
    property alias saturation_visible: saturation_slider.visible
    property alias lightness: lightness_slider.value
    property alias lightness_from: lightness_slider.from
    property alias lightness_to: lightness_slider.to
    property alias lightness_visible: lightness_slider.visible

    radius: 10
    border.width: 1
    border.color: "#000000"

    onColorChanged: {
        hue_slider.value = color.hslHue;
        saturation_slider.value = color.hslSaturation;
        lightness_slider.value = color.hslLightness;
    }

    width: main_layout.implicitWidth + 4
    height: main_layout.implicitHeight

    ButtonGroup {
        id: engine_group
    }

    ColumnLayout {
        id: main_layout

        spacing: -10

        Slider {
            id: hue_slider
            onValueChanged: hsl_sliders.color.hslHue = value

            background: Rectangle {
                x: lightness_slider.leftPadding + height / 2 - width / 2
                y: lightness_slider.topPadding + lightness_slider.availableHeight / 2 - height / 2
                implicitWidth: 200
                implicitHeight: 12
                width: implicitHeight
                height: lightness_slider.availableWidth
                radius: 3
                rotation: 90
                gradient: Gradient {
                    GradientStop {
                        position: 1.00;
                        color: Qt.hsla(hue_slider.from, saturation_slider.value, lightness_slider.value, 1)
                    }
                    GradientStop {
                        position: 0.75;
                        color: Qt.hsla(hue_slider.from + (hue_slider.to - hue_slider.from) / 4, saturation_slider.value, lightness_slider.value, 1)
                    }
                    GradientStop {
                        position: 0.50;
                        color: Qt.hsla(hue_slider.from + (hue_slider.to - hue_slider.from) / 2, saturation_slider.value, lightness_slider.value, 1)
                    }
                    GradientStop {
                        position: 0.25;
                        color: Qt.hsla(hue_slider.from + (hue_slider.to - hue_slider.from) / (4/3), saturation_slider.value, lightness_slider.value, 1)
                    }
                    GradientStop {
                        position: 0.00;
                        color: Qt.hsla(hue_slider.to, saturation_slider.value, lightness_slider.value, 1)
                    }
                }
            }
        }

        Slider {
            id: saturation_slider
            onValueChanged: hsl_sliders.color.hslSaturation = value

            background: Rectangle {
                x: lightness_slider.leftPadding + height / 2 - width / 2
                y: lightness_slider.topPadding + lightness_slider.availableHeight / 2 - height / 2
                implicitWidth: hue_slider.implicitWidth
                implicitHeight: 12
                width: implicitHeight
                height: lightness_slider.availableWidth
                radius: 3
                rotation: 90
                border.width: 1
                border.color: "black"
                gradient: Gradient {
                    GradientStop {
                        position: 1.00;
                        color: Qt.hsla(hsl_sliders.color.hslHue, saturation_slider.from, hsl_sliders.color.hslLightness, 1)
                    }
                    GradientStop {
                        position: 0.00;
                        color: Qt.hsla(hsl_sliders.color.hslHue, saturation_slider.to, hsl_sliders.color.hslLightness, 1)
                    }
                }
            }
        }

        Slider {
            id: lightness_slider
            onValueChanged: hsl_sliders.color.hslLightness = value

            background: Rectangle {
                x: lightness_slider.leftPadding + height / 2 - width / 2
                y: lightness_slider.topPadding + lightness_slider.availableHeight / 2 - height / 2
                implicitWidth: hue_slider.implicitWidth
                implicitHeight: 12
                width: implicitHeight
                height: lightness_slider.availableWidth
                radius: 3
                rotation: 90
                border.width: 1
                border.color: "black"
                gradient: Gradient {
                    GradientStop {
                        position: 1.00;
                        color: Qt.hsla(hsl_sliders.color.hslHue, hsl_sliders.color.hslSaturation, lightness_slider.from, 1)
                    }
                    GradientStop {
                        position: 0.00;
                        color: Qt.hsla(hsl_sliders.color.hslHue, hsl_sliders.color.hslSaturation, lightness_slider.to, 1)
                    }
                }
            }
        }
    }
}
