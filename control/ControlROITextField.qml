import QtQuick 2.3

import QtQuick.Controls 2.0

Row {
    id: root

    spacing: height * 0.1

    property int valueX
    property int valueY

    property real fontPixelSize

    property string valueLabel

    Label {
        width: parent.width * 0.35
        height: parent.height

        text: valueLabel

        font.pixelSize: fontPixelSize

        verticalAlignment: Text.AlignVCenter
    }

    Label {
        height: parent.height

        text: qsTr(" (")

        font.pixelSize: fontPixelSize

        verticalAlignment: Text.AlignVCenter
    }

    TextField {
        id: fieldX
        width: parent.width * 0.25
        height: parent.height

        text: valueX

        leftPadding: width * 0.2
        horizontalAlignment: Text.AlignHCenter

        font.pixelSize: fontPixelSize
        validator: IntValidator { bottom: 0; top: 9999 }

        inputMethodHints: Qt.ImhDigitsOnly

        Keys.onPressed: {
            // enter pressed
            if(event.key === 16777220) {
                valueX = text * 1

                focus = false
            }
            // esc pressed
            else if(event.key === 16777216) {
                focus = false
            }
        }

        onFocusChanged: {
            if(!focus) {
                text = valueX
            }
            else {
                selectAll()
            }
        }
    }

    Label {
        height: parent.height

        text: qsTr(",")

        font.pixelSize: fontPixelSize

        verticalAlignment: Text.AlignVCenter
    }

    TextField {
        id: fieldY

        width: parent.width * 0.25
        height: parent.height

        text: valueY

        font.pixelSize: fontPixelSize
        validator: IntValidator { bottom: 0; top: 9999 }

        leftPadding: width * 0.2
        horizontalAlignment: Text.AlignHCenter

        inputMethodHints: Qt.ImhDigitsOnly

        Keys.onPressed: {
            // enter pressed
            if(event.key === 16777220) {
                valueY = text * 1

                focus = false
            }
            // esc pressed
            else if(event.key === 16777216) {
                focus = false
            }
        }

        onFocusChanged: {
            if(!focus) {
                text = valueY
            }
            else {
                selectAll()
            }
        }
    }

    Label {
        height: parent.height

        text: qsTr(")")

        font.pixelSize: fontPixelSize

        verticalAlignment: Text.AlignVCenter
    }
}
