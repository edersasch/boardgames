import QtQuick 2.9
import QtQuick.Controls 2.2

Column {
    id: col

    property bool control_buttons_visible: true
    property alias buttons: buttons
    property alias maxChildWidth: buttons.maxChildWidth

    signal adding(int ycoord)
    signal request_delete_branch(int move_id)
    signal move_buttons_visible(var is_visible)

    function trigger_move_buttons_visible(is_visible) {
        show_moves_button.checked = is_visible;
    }

    onParentChanged: {
        width = Qt.binding(function() { return parent ? parent.width : 0; });
    }

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

        width: parent.width

        add: Transition {
            NumberAnimation { properties: "opacity"; from: 0; to: 1 }
            onRunningChanged: {
                if (running === true) {
                    col.adding(ViewTransition.destination.y)
                }
            }
        }

        move: Transition {
            NumberAnimation { properties: "x,y"; easing.type: Easing.OutCirc }
            NumberAnimation { properties: "opacity"; to: 1 } // "add" probably interrupted
        }

        ToolButton {
            id: delete_branch_button
            flat: true
            icon.source: "qrc:/delete.svg"
            visible: col.control_buttons_visible
            onClicked: col.request_delete_branch(buttons.children[2].move_id);
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
