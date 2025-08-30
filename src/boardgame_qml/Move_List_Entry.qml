import QtQuick
import QtQuick.Controls

Column {
    id: col

    property bool control_buttons_visible: true
    property alias buttons: buttons
    property alias maxChildWidth: buttons.maxChildWidth

    signal request_delete_branch(int move_id)
    signal move_buttons_visible(var is_visible)

    function trigger_move_buttons_visible(is_visible) {
        show_moves_button.checked = is_visible;
    }

    function scroll_to(y_offset) {
        if (parent && (parent instanceof Move_List_Root)) {
            (parent as Move_List_Root).scroll_to(y_offset + y);
        }
    }

    z: -1  // below Move_List_Control

    width: parent ? parent.width : 0
    spacing: 3
    leftPadding: 5

    Flow {
        id: buttons

        property int maxChildWidth: 0

        onChildrenChanged: {
            var mcw = 0
            var last_child;
            if (children.length > 0) {
                last_child = children[children.length - 1];
                mcw = last_child.width;
                if (mcw > maxChildWidth) {
                    maxChildWidth = mcw;
                }
                last_child.visible = children.length > 3 ? Qt.binding(function() { return show_moves_button.checked; }) : true
            }
        }

        width: col.width - col.leftPadding

        add: Transition {
            NumberAnimation { properties: "opacity"; from: 0; to: 1 }
        }

        move: Transition {
            NumberAnimation { properties: "x,y"; easing.type: Easing.OutBack }
            NumberAnimation { properties: "opacity"; to: 1 } // "add" probably interrupted
        }

        Confirm_Button {
            id: delete_branch_button
            flat: true
            direction: delete_branch_button.direction_right
            icon.source: "qrc:/delete.svg"
            visible: col.control_buttons_visible
            z: 100
            onConfirmed: col.request_delete_branch(buttons.children[2].move_id);
        }

        ToolButton {
            id: show_moves_button
            flat: true
            checkable: true
            checked: true
            icon.source: checked ? "qrc:/show.svg" : "qrc:/hide.svg";
            visible: col.control_buttons_visible && buttons.children.length > 3
            onCheckedChanged: {
                col.move_buttons_visible(checked);
            }
        }
    }
}
