import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3

RowLayout {
    id: root

    signal request_move_list_back_to_start()
    signal request_move_list_forward
    signal request_move_list_back
    signal request_move_list_import
    signal request_move_list_export

    spacing: 0

    ToolButton {
        onClicked: root.request_move_list_back_to_start()

        flat: true
        icon.source: "qrc:/home.svg"
    }

    ToolButton {
        onClicked: root.request_move_list_back();

        flat: true
        icon.source: "qrc:/back.svg"
    }

    ToolButton {
        onClicked: root.request_move_list_forward()

        flat: true
        icon.source: "qrc:/forward.svg"
    }

    ToolButton {
        onClicked: root.request_move_list_import()

        flat: true
        icon.source: "qrc:/import.svg"
    }

    ToolButton {
        onClicked: root.request_move_list_export()

        flat: true
        icon.source: "qrc:/export.svg"
    }
}
