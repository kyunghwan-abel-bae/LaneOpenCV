import QtQuick 2.3

import QtQuick.Controls 2.0

GroupBox {
    id: root

    title: qsTr("Flip")

    font.pixelSize: fontSize11

    anchors.horizontalCenter: parent.horizontalCenter

    property alias isHorizontalFlipChecked: checkHorizontalFlip.checked
    property alias isVerticalFlipChecked: checkVerticalFlip.checked

    signal signalHorizontalFlipClicked(var isChecked)
    signal signalVerticalFlipClicked(var isChecked)

    Column {
        anchors.fill: parent

        Item {
            id: itemHorizontalFlip

            width: parent.width
            height: children[0].height

            CheckBox {
                id: checkHorizontalFlip

                text: qsTr("Horizontal Flip")
                font.pixelSize: fontSize11

                checked: false
            }
        }
        Item {
            id: itemVerticalFlip

            width: parent.width
            height: children[0].height

            CheckBox {
                id: checkVerticalFlip

                text: qsTr("Vertical Flip")
                font.pixelSize: fontSize11

                checked: false
            }
        }
    }
}
