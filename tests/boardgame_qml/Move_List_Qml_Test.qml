import QtQuick 2.9
import QtQuick.Layouts 1.3

//import "../boardgame_qml" // to make Move_List_Root known causes crash

ColumnLayout {
    id: root

    property alias move_list_1: move_list_root1.entries
    property alias move_list_2: move_list_root2.entries

    Move_List_Root {
        id: move_list_root1
    }

    Move_List_Root {
        id: move_list_root2
    }
}
