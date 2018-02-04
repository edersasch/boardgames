import QtQuick 2.7
import QtQuick.Layouts 1.3

RowLayout {
    id: rowLayout
    width: 640
    height: 480
    onParentChanged: {
        if (parent) {
            anchors.fill = parent;
        }
    }
    property alias boardFields: boardFields
    property alias whiteHFields: whiteHFields
    property alias whiteVFields: whiteVFields
    property alias blackHFields: blackHFields
    property alias blackVFields: blackVFields
    signal horizontal;
    signal vertical;

    Rectangle {
        id: whiteHDrawer
        width: rowLayout.width / 24
        height: 200
        radius: 10
        border.width: 1
        visible: (rowLayout.width > rowLayout.height)
        Layout.fillHeight: true
        Layout.fillWidth: true
        onVisibleChanged: {
            if (visible === true) {
                rowLayout.horizontal();
            } else {
                rowLayout.vertical();
            }
        }

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

        Rectangle {
            id: whiteVDrawer
            width: 200
            height: rowLayout.height / 24
            radius: 10
            border.width: 1
            visible: !whiteHDrawer.visible
            Layout.fillHeight: true
            Layout.fillWidth: true

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
            width: 200
            height: rowLayout.height / 24
            radius: 10
            border.width: 1
            visible: !whiteHDrawer.visible
            Layout.fillHeight: true
            Layout.fillWidth: true

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
    }

    Rectangle {
        id: blackHDrawer
        width: rowLayout.width / 24
        height: 200
        radius: 10
        border.width: 1
        visible: whiteHDrawer.visible
        Layout.fillHeight: true
        Layout.fillWidth: true

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
}
