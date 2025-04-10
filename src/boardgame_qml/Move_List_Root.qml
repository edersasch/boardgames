import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

Flickable {
    id: root

    property alias entries: entries

    function scroll_to(y_offset) {
        if (y_offset >= 0) {
            if (contentHeight < height) {
                contentY = 0;
            } else if (y_offset > contentY + height - control.height) {
                y_offset -= list_overlay.height;
                if (y_offset > (contentHeight - height)) {
                    y_offset = contentHeight - height;
                }
                contentY = y_offset;
            } else {
                if (y_offset < contentY + list_overlay.height) {
                    contentY = y_offset < height - control.height ? 0 : y_offset - list_overlay.height;
                }
            }
        }
    }

    contentHeight: entries.childrenRect.height
    clip: true
    boundsBehavior: Flickable.OvershootBounds
    ScrollBar.vertical: ScrollBar {}

    Move_List_Entry {
        id: entries

        property alias control: control
        property alias default_suffix: choose_move_list_file.defaultSuffix
        property alias name_filters: choose_move_list_file.nameFilters

        property bool choose_move_list_file_existing
        onChoose_move_list_file_existingChanged :
            choose_move_list_file_existing === true ?
                choose_move_list_file.fileMode = FileDialog.OpenFile :
                choose_move_list_file.fileMode = FileDialog.SaveFile
        choose_move_list_file_existing: true

        property bool choose_move_list_file_visible
        onChoose_move_list_file_visibleChanged:
            choose_move_list_file_visible === true ?
                choose_move_list_file.open() :
                choose_move_list_file.reject()
        choose_move_list_file_visible: false

        signal chosen_move_list_path(var file_url)

        function scroll_to(y_offset) {
            root.scroll_to(y_offset);
        }

        leftPadding: 0
        maxChildWidth: control.width
        control_buttons_visible: false

        Move_List_Control {
            id: control
            parent: entries.buttons
            z: 1
            onRequest_move_list_back_to_start: {
                root.contentY = 0;
            }
        }

        FileDialog {
            id: choose_move_list_file;

            onAccepted: entries.chosen_move_list_path(choose_move_list_file.selectedFile)
        }
    }

    Rectangle {
        id: list_overlay
        height: control.height * 2
        width: root.width
        x: 0
        y: root.contentY
        opacity: 0
        visible: opacity > 0
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#ffffff"
            }
            GradientStop {
                position: 0.4
                color: "#ffffff"
            }
            GradientStop {
                position: 1
                color: "#00000000"
            }
        }
        Move_List_Control {
            id: fixed_control
            z: 1
            confirm: control.confirm
            onRequest_move_list_back_to_start: {
                control.request_move_list_back_to_start();
            }
            onRequest_move_list_forward: {
                control.request_move_list_forward();
            }
            onRequest_move_list_back: {
                control.request_move_list_back();
            }
            onRequest_move_list_import: {
                control.request_move_list_import();
            }
            onRequest_move_list_export: {
                control.request_move_list_export();
            }
        }
        states: [
            State {
                when: root.contentY !== 0
                PropertyChanges {
                    target: list_overlay
                    opacity: 1
                }
                PropertyChanges {
                    target: control
                    opacity: 0
                }
            }
        ]
        transitions: [
            Transition {
                NumberAnimation {
                    property: "opacity"
                    duration: 200
                    easing.type: Easing.InOutQuad
                }
            }
        ]
    }

    Behavior on contentY { SmoothedAnimation { duration: 500 } }
}
