import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3

RowLayout {
    id: mainLayout
    width: 640
    height: 480
    anchors.fill: parent ? parent : undefined
    property alias gameboard: gameboard
    readonly property bool horOrientation: width >= height
    signal horizontal;
    signal vertical;
    signal enterSetupMode
    signal leaveSetupMode
    signal newGame

    onHorOrientationChanged: {
        horOrientation ? horizontal() : vertical()
    }

    ColumnLayout {
        id: controlColumn
        anchors.fill: parent
        visible: horOrientation
        z: 100

        ToolButton {
            id: newgameVButton
            flat: true
            icon.source: "qrc:/newgame.svg"
            onClicked: mainLayout.newGame()
        }

        ToolButton {
            id: setupVButton
            flat: true
            checkable: true
            icon.source: "qrc:/setup.svg"
            onCheckedChanged: checked ? mainLayout.enterSetupMode() : mainLayout.leaveSetupMode()
        }
    }

    ColumnLayout {
        id: boardColumn

        RowLayout {
            id: controlRow
            anchors.fill: parent
            visible: !horOrientation
            z: 100

            ToolButton {
                id: newgameHButton
                flat: true
                icon.source: "qrc:/newgame.svg"
                onClicked: mainLayout.newGame()
            }

            ToolButton {
                id: setupHButton
                flat: true
                checkable: true
                icon.source: "qrc:/setup.svg"
                checked: setupVButton.checked
                onCheckedChanged: setupVButton.checked = checked
            }
        }

        Item {
            id: gameboard
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
