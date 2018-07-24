import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

//import "../boardgame_qml" // to make Move_List_Root known causes crash

RowLayout {
    id: rowLayout

    property color whiteColor: "white"
    property color blackColor: "black"
    property bool horOrientation: true
    property bool whiteDrawerCanHide: true
    property bool blackDrawerCanHide: true
    property bool whitePrisonCanHide: true
    property bool blackPrisonCanHide: true
    property bool showMoveList: false
    property alias boardFields: boardFields
    property alias whiteHFields: whiteHDrawer.fields
    property alias whiteVFields: whiteVDrawer.fields
    property alias whiteHPFields: whiteHPrison.fields
    property alias whiteVPFields: whiteVPrison.fields
    property alias blackHFields: blackHDrawer.fields
    property alias blackVFields: blackVDrawer.fields
    property alias blackHPFields: blackHPrison.fields
    property alias blackVPFields: blackVPrison.fields
    property alias v_move_list: vHistoryRect.entries
    property alias h_move_list: hHistoryRect.entries

    width: 640
    height: 480
    spacing: 1
    anchors.fill: parent ? parent : undefined

    PieceCBox {
        id: whiteHPrison

        color: blackColor // pieces for white prison are captured by black, so the prison has black's color
        canHide: whitePrisonCanHide || !horOrientation
        fields.model: 7
        sizeFactor: 0.7
    }

    PieceCBox {
        id: whiteHDrawer

        canHide: whiteDrawerCanHide || !horOrientation
        fields.model: 9
        sizeFactor: 0.8
    }

    ColumnLayout {
        id: columnLayout

        width: 100
        height: 100
        spacing: 1

        PieceRBox {
            id: whiteVPrison

            color: blackColor
            canHide: whitePrisonCanHide || horOrientation
            fields.model: 7
            sizeFactor: 0.7
        }

        PieceRBox {
            id: whiteVDrawer

            canHide: whiteDrawerCanHide || horOrientation
            fields.model: 9
            sizeFactor: 0.8
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

                width: parent.width * 0.88
                height: parent.height * 0.88
                color: "#000000"
                anchors.centerIn: parent
            }

            Rectangle {
                id: bg1

                width: parent.width * 0.85
                height: parent.height * 0.85
                color: "#ffffff"
                anchors.centerIn: parent
            }

            Rectangle {
                id: middle

                width: parent.width * 0.59
                height: parent.height * 0.59
                color: "#000000"
                anchors.centerIn: parent
            }

            Rectangle {
                id: bg2

                width: parent.width * 0.56
                height: parent.height * 0.56
                color: "#ffffff"
                anchors.centerIn: parent
            }

            Rectangle {
                id: inner

                width: parent.width * 0.3
                height: parent.height * 0.3
                color: "#000000"
                anchors.centerIn: parent
            }

            Rectangle {
                id: verticalBar

                width: parent.width / 50 * 0.88
                height: parent.height * 0.88
                color: "#000000"
                anchors.centerIn: parent
            }

            Rectangle {
                id: horizontalBar

                width: parent.width * 0.88
                height: parent.height / 50 * 0.88
                color: "#000000"
                anchors.centerIn: parent
            }

            Rectangle {
                id: bgCenter

                width: parent.width * 0.27
                height: parent.height * 0.27
                color: "#ffffff"
                anchors.centerIn: parent
            }

            GridLayout {
                id: fieldLayout

                anchors.fill: parent
                columns: 7

                Repeater {
                    id: boardFields

                    model: 49

                    Item {
                        onZChanged: {
                            bg.z = z;
                            columnLayout.z = z;
                        }

                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                }
            }
        }

        PieceRBox {
            id: blackVDrawer

            canHide: blackDrawerCanHide || horOrientation
            fields.model: 9
            sizeFactor: 0.8
        }

        PieceRBox {
            id: blackVPrison

            color: whiteColor
            canHide: blackPrisonCanHide || horOrientation
            fields.model: 7
            sizeFactor: 0.7
        }

        Move_List_Root {
            id: vHistoryRect

            implicitHeight: (!horOrientation && showMoveList) ? Math.max(width / whiteVFields.model, 200) : 0
            Layout.fillWidth: true
            contentWidth: width

            Behavior on implicitHeight { NumberAnimation { duration: 100 } }
        }
    }

    PieceCBox {
        id: blackHDrawer

        canHide: blackDrawerCanHide || !horOrientation
        fields.model: 9
        sizeFactor: 0.8
    }

    PieceCBox {
        id: blackHPrison

        color: whiteColor
        canHide: blackPrisonCanHide || !horOrientation
        fields.model: 7
        sizeFactor: 0.7
    }

    Move_List_Root {
        id: hHistoryRect

        implicitWidth: (horOrientation && showMoveList) ? Math.max(height / whiteHFields.model, entries.maxChildWidth) : 0
        contentWidth: implicitWidth
        Layout.fillHeight: true

        Behavior on implicitWidth { NumberAnimation { duration: 100 } }
    }
}
