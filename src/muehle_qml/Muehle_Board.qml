import "qrc:/src/boardgame_qml"

import QtQuick
import QtQuick.Layouts

RowLayout {
    id: root

    property color white_color: "white"
    property color black_color: "black"
    property bool hor_orientation: true
    property bool white_drawer_can_hide: true
    property bool black_drawer_can_hide: true
    property bool white_prison_can_hide: true
    property bool black_prison_can_hide: true
    property bool move_list_visible: false
    property alias board_fields: board_fields
    property alias white_h_drawer_fields: white_h_drawer.fields
    property alias white_v_drawer_fields: white_v_drawer.fields
    property alias white_h_prison_fields: white_h_prison.fields
    property alias white_v_prison_fields: white_v_prison.fields
    property alias black_h_drawer_fields: black_h_drawer.fields
    property alias black_v_drawer_fields: black_v_drawer.fields
    property alias black_h_prison_fields: black_h_prison.fields
    property alias black_v_prison_fields: black_v_prison.fields
    property alias h_move_list: h_move_list_root.entries
    property alias v_move_list: v_move_list_root.entries

    width: 640
    height: 480
    spacing: 1

    Piece_Column_Box {
        id: white_h_prison

        color: root.black_color // pieces for white prison are captured by black, so the prison has black's color
        can_hide: root.white_prison_can_hide || !root.hor_orientation
        fields.model: 7
        size_factor: 0.7
    }

    Piece_Column_Box {
        id: white_h_drawer

        can_hide: root.white_drawer_can_hide || !root.hor_orientation
        fields.model: 9
        size_factor: 0.8
    }

    ColumnLayout {
        id: column_layout

        spacing: 1

        Piece_Row_Box {
            id: white_v_prison

            color: root.black_color
            can_hide: root.white_prison_can_hide || root.hor_orientation
            fields.model: 7
            size_factor: 0.7
        }

        Piece_Row_Box {
            id: white_v_drawer

            can_hide: root.white_drawer_can_hide || root.hor_orientation
            fields.model: 9
            size_factor: 0.8
        }

        GridLayout {
            id: board_grid
            Layout.fillWidth: true
            Layout.fillHeight: true
            columnSpacing: 1
            rowSpacing: 1
            columns: 2

            Rectangle {
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width / 24

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    Repeater {
                        model: ["7", "6", "5", "4", "3", "2", "1"]

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            border.width: 1
                            border.color: "black"
                            radius: 4
                            Item {
                                anchors.centerIn: parent
                                width: parent.width
                                height: board_grid.height / 24
                                Text {
                                    anchors.fill: parent
                                    horizontalAlignment: Qt.AlignHCenter
                                    verticalAlignment: Qt.AlignVCenter
                                    fontSizeMode: Text.Fit
                                    minimumPixelSize: 1
                                    font.pixelSize: 72
                                    text: modelData
                                }
                            }
                        }
                    }
                }
            }

            Image {
                id: bg
                source: "qrc:/muehle_field_bright.svg"
                fillMode: Image.Stretch
                Layout.fillWidth: true
                Layout.fillHeight: true

                GridLayout {
                    anchors.fill: parent
                    columns: 7

                    Repeater {
                        id: board_fields

                        model: 49

                        Item {
                            onZChanged: {
                                bg.z = z;
                                board_grid.z = z;
                                column_layout.z = z;
                            }

                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }
                    }
                }
            }

            Item {
                Layout.preferredHeight: 1
                Layout.preferredWidth: 1
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: parent.height / 24

                RowLayout {
                    anchors.fill: parent
                    spacing: 0

                    Repeater {
                        model: ["A", "B", "C", "D", "E", "F", "G"]

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            border.width: 1
                            border.color: "black"
                            radius: 4
                            Text {
                                anchors.fill: parent
                                horizontalAlignment: Qt.AlignHCenter
                                verticalAlignment: Qt.AlignVCenter
                                fontSizeMode: Text.Fit
                                minimumPixelSize: 1
                                font.pixelSize: 72
                                text: modelData
                            }
                        }
                    }
                }
            }
        }

        Piece_Row_Box {
            id: black_v_drawer

            can_hide: root.black_drawer_can_hide || root.hor_orientation
            fields.model: 9
            size_factor: 0.8
        }

        Piece_Row_Box {
            id: black_v_prison

            color: root.white_color
            can_hide: root.black_prison_can_hide || root.hor_orientation
            fields.model: 7
            size_factor: 0.7
        }

        Move_List_Root {
            id: v_move_list_root

            implicitHeight: (!root.hor_orientation && root.move_list_visible) ? Math.max(width / root.white_v_drawer_fields.model, 200) : 0
            Layout.fillWidth: true
            contentWidth: width

            Behavior on implicitHeight { NumberAnimation { easing.type: Easing.OutBack; duration: 200 } }
        }
    }

    Piece_Column_Box {
        id: black_h_drawer

        can_hide: root.black_drawer_can_hide || !root.hor_orientation
        fields.model: 9
        size_factor: 0.8
    }

    Piece_Column_Box {
        id: black_h_prison

        color: root.white_color
        can_hide: root.black_prison_can_hide || !root.hor_orientation
        fields.model: 7
        size_factor: 0.7
    }

    Move_List_Root {
        id: h_move_list_root

        implicitWidth: (root.hor_orientation && root.move_list_visible) ? Math.max(height / root.white_h_drawer_fields.model, entries.maxChildWidth) : 0
        contentWidth: implicitWidth
        Layout.fillHeight: true

        Behavior on implicitWidth { NumberAnimation { easing.type: Easing.OutBack; duration: 200 } }
    }
}
