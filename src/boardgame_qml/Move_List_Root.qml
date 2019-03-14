import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.3

Flickable {
    id: root

    property alias entries: entries

    function adjContentY() {
        /*if (contentHeight > height) {
            contentY = contentHeight - height;
        }*/
    }

    onContentHeightChanged: {
        adjContentY();
    }

    contentHeight: entries.childrenRect.height
    clip: true
    boundsBehavior: Flickable.OvershootBounds
    ScrollBar.vertical: ScrollBar {}

    Move_List_Entry {
        id: entries

        property alias control: control
        property alias choose_move_list_file_existing: choose_move_list_file.selectExisting
        property alias choose_move_list_file_visible: choose_move_list_file.visible
        property alias default_suffix: choose_move_list_file.defaultSuffix
        property alias name_filters: choose_move_list_file.nameFilters

        signal chosen_move_list_path(var file_url)

        onAdding: {
            adjContentY();
        }

        leftPadding: 0
        maxChildWidth: control.width
        control_buttons_visible: false
        buttons.z: 100

        Move_List_Control {
            id: control
            parent: entries.buttons
            z: 100
        }

        FileDialog {
            id: choose_move_list_file;

            onAccepted: entries.chosen_move_list_path(choose_move_list_file.fileUrl)
        }
    }

    Behavior on contentY { NumberAnimation { easing.type: Easing.OutBack } }
}
