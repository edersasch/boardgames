import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3

RowLayout {
    id: mainLayout
    width: 640
    height: 480
    spacing: 0
    anchors.fill: parent ? parent : undefined
    property alias gameboard: gameboard
    readonly property bool horOrientation: width >= height
    signal horizontal;
    signal vertical;
    signal newGame
    signal showMoveList
    signal hideMoveList
    signal enterSetupMode
    signal leaveSetupMode

    onHorOrientationChanged: {
        horOrientation ? horizontal() : vertical()
    }

    ColumnLayout {
        id: controlColumn
        spacing: 0
        visible: horOrientation
        z: 100

        ToolButton {
            id: newgameVButton
            flat: true
            icon.source: "qrc:/refresh.svg"
            onClicked: {
                mainLayout.newGame();
                setupVButton.checked = false;
            }
        }

        ToolButton {
            id: moveListVButton
            flat: true
            checkable: true
            icon.source: "qrc:/list.svg"
            onCheckedChanged: checked ? mainLayout.showMoveList() : mainLayout.hideMoveList()
        }

        ToolButton {
            id: setupVButton
            flat: true
            checkable: true
            icon.source: "qrc:/setup.svg"
            onCheckedChanged: checked ? mainLayout.enterSetupMode() : mainLayout.leaveSetupMode()
        }

        Item {
            Layout.fillHeight: true
        }
    }

    ColumnLayout {
        id: boardColumn
        spacing: 0

        RowLayout {
            id: controlRow
            spacing: 0
            visible: !horOrientation
            z: 100

            ToolButton {
                id: newgameHButton
                flat: true
                icon.source: "qrc:/refresh.svg"
                onClicked: {
                    mainLayout.newGame();
                    setupVButton.checked = false;
                }
            }

            ToolButton {
                id: moveListHButton
                flat: true
                checkable: true
                icon.source: "qrc:/list.svg"
                checked: moveListVButton.checked
                onCheckedChanged: moveListVButton.checked = checked
            }

            ToolButton {
                id: setupHButton
                flat: true
                checkable: true
                icon.source: "qrc:/setup.svg"
                checked: setupVButton.checked
                onCheckedChanged: setupVButton.checked = checked
            }

            Item {
                Layout.fillWidth: true
            }
        }

        Item {
            id: gameboard
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
