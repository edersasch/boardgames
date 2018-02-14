import QtQuick 2.7
import QtQuick.Layouts 1.3

RowLayout {
    id: rowLayout
    width: 640
    height: 480
    spacing: 1
    anchors.fill: parent ? parent : undefined
    property alias boardFields: boardFields
    property alias whiteHFields: whiteHFields
    property alias whiteVFields: whiteVFields
    property alias whiteHPFields: whiteHPFields
    property alias whiteVPFields: whiteVPFields
    property alias blackHFields: blackHFields
    property alias blackVFields: blackVFields
    property alias blackHPFields: blackHPFields
    property alias blackVPFields: blackVPFields
    property color whiteColor: "white"
    property color blackColor: "black"
    property bool whiteDrawerCanHide: true
    property bool blackDrawerCanHide: true
    property bool whitePrisonCanHide: true
    property bool blackPrisonCanHide: true
    property bool horOrientation: true

    Rectangle {
        id: whiteHPrison
        radius: 10
        border.width: 1
        color: blackColor // pieces for white prison are captured by black, so the prison has black's color
        implicitWidth: (horOrientation && !whitePrisonCanHide) ? height / whiteHFields.model : 0 // divide by number of pieces, not by number of prison places
        Layout.fillHeight: true
        Behavior on implicitWidth { NumberAnimation { duration: 100 } }

        ColumnLayout {
            id: whiteHPLayout
            anchors.fill: parent

            Repeater {
                id: whiteHPFields
                model: 7

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    onZChanged: {
                        whiteHPrison.z = z;
                    }
                }
            }
        }
    }

    Rectangle {
        id: whiteHDrawer
        radius: 10
        border.width: 1
        implicitWidth: (horOrientation && !whiteDrawerCanHide) ? height / whiteHFields.model : 0
        Layout.fillHeight: true
        Behavior on implicitWidth { NumberAnimation { duration: 100 } }

        ColumnLayout {
            id: whiteHLayout
            anchors.fill: parent

            Repeater {
                id: whiteHFields
                model: 9

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    onZChanged: {
                        whiteHDrawer.z = z;
                    }
                }
            }
        }
    }

    ColumnLayout {
        id: columnLayout
        width: 100
        height: 100
        spacing: 1

        Rectangle {
            id: whiteVPrison
            radius: 10
            border.width: 1
            color: blackColor
            implicitHeight: (!horOrientation && !whitePrisonCanHide) ? width / whiteVFields.model : 0
            Layout.fillWidth: true
            Behavior on implicitHeight { NumberAnimation { duration: 100 } }

            RowLayout {
                id: whiteVPLayout
                anchors.fill: parent

                Repeater {
                    id: whiteVPFields
                    model: 7

                    Item {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        onZChanged: {
                            whiteVPrison.z = z;
                        }
                    }
                }
            }
        }

        Rectangle {
            id: whiteVDrawer
            radius: 10
            border.width: 1
            implicitHeight: (!horOrientation && !whiteDrawerCanHide) ? width / whiteVFields.model : 0
            Layout.fillWidth: true
            Behavior on implicitHeight { NumberAnimation { duration: 100 } }

            RowLayout {
                id: whiteVLayout
                anchors.fill: parent

                Repeater {
                    id: whiteVFields
                    model: 9

                    Item {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        onZChanged: {
                            whiteVDrawer.z = z;
                        }
                    }
                }
            }
        }

        Rectangle {
            id: bg
            width: 200
            height: 200
            color: "#ffffff"
            Layout.fillHeight: true
            Layout.fillWidth: true

            Rectangle {
                id: outer
                width: parent.width
                height: parent.height
                color: "#000000"
                anchors.fill: parent
                scale: 0.88
            }

            Rectangle {
                id: bg1
                width: parent.width
                height: parent.height
                color: "#ffffff"
                scale: outer.scale - 0.03
                anchors.fill: parent
            }

            Rectangle {
                id: middle
                width: parent.width
                height: parent.height
                color: "#000000"
                scale: 0.59
                anchors.fill: parent
            }

            Rectangle {
                id: bg2
                width: parent.width
                height: parent.height
                color: "#ffffff"
                scale: middle.scale - 0.03
            }

            Rectangle {
                id: inner
                width: parent.width
                height: parent.height
                color: "#000000"
                scale: 0.3
            }

            Rectangle {
                id: verticalBar
                width: parent.width / 50
                height: parent.height
                color: "#000000"
                scale: outer.scale
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Rectangle {
                id: horizontalBar
                width: parent.width
                height: parent.height / 50
                color: "#000000"
                anchors.verticalCenter: parent.verticalCenter
                scale: outer.scale
            }

            Rectangle {
                id: bgCenter
                width: parent.width
                height: parent.height
                color: "#ffffff"
                scale: inner.scale - 0.03
            }

            GridLayout {
                id: fieldLayout
                anchors.fill: parent
                columns: 7

                Repeater {
                    id: boardFields
                    model: 49

                    Item {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        onZChanged: {
                            bg.z = z;
                            columnLayout.z = z;
                        }
                    }
                }
            }
        }

        Rectangle {
            id: blackVDrawer
            radius: 10
            border.width: 1
            implicitHeight: (!horOrientation && !blackDrawerCanHide) ? width / blackVFields.model : 0
            Layout.fillWidth: true
            Behavior on implicitHeight { NumberAnimation { duration: 100 } }

            RowLayout {
                id: blackVLayout
                anchors.fill: parent

                Repeater {
                    id: blackVFields
                    model: 9

                    Item {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        onZChanged: {
                            blackVDrawer.z = z;
                        }
                    }
                }
            }
        }

        Rectangle {
            id: blackVPrison
            radius: 10
            border.width: 1
            color: whiteColor
            implicitHeight: (!horOrientation && !blackPrisonCanHide) ? width / blackVFields.model : 0
            Layout.fillWidth: true
            Behavior on implicitHeight { NumberAnimation { duration: 100 } }

            RowLayout {
                id: blackVPLayout
                anchors.fill: parent

                Repeater {
                    id: blackVPFields
                    model: 7

                    Item {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        onZChanged: {
                            blackVPrison.z = z;
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: blackHDrawer
        radius: 10
        border.width: 1
        implicitWidth: (horOrientation && !blackDrawerCanHide) ? height / blackHFields.model : 0
        Layout.fillHeight: true
        Behavior on implicitWidth { NumberAnimation { duration: 100 } }

        ColumnLayout {
            id: blackHLayout
            anchors.fill: parent

            Repeater {
                id: blackHFields
                model: 9

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    onZChanged: {
                        blackHDrawer.z = z;
                    }
                }
            }
        }
    }

    Rectangle {
        id: blackHPrison
        radius: 10
        border.width: 1
        color: whiteColor
        implicitWidth: (horOrientation && !blackPrisonCanHide) ? height / blackHFields.model : 0
        Layout.fillHeight: true
        Behavior on implicitWidth { NumberAnimation { duration: 100 } }

        ColumnLayout {
            id: blackHPLayout
            anchors.fill: parent

            Repeater {
                id: blackHPFields
                model: 7

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    onZChanged: {
                        blackHPrison.z = z;
                    }
                }
            }
        }
    }
}
