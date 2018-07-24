import QtQuick 2.9
import QtQuick.Controls 2.2

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

        onAdding: {
            adjContentY();
        }

        leftPadding: 0
        maxChildWidth: control.width
        control_buttons_visible: false

        Move_List_Control {
            id: control
            parent: entries.buttons
        }
    }

    Behavior on contentY { NumberAnimation { easing.type: Easing.OutCirc } }
}
