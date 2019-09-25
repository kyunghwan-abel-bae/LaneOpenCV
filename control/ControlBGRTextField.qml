import QtQuick 2.3

import QtQuick.Controls 2.0

Row {
    id: root

    spacing: height * 0.1

    property int valueB
    property int valueG
    property int valueR

    property real fontPixelSize: 10

    property string labelValue

    Label {
        width: parent.width * 0.55
        height: parent.height

        text: labelValue

        font.pixelSize: fontPixelSize

        verticalAlignment: Text.AlignVCenter
    }

    TextField {
        id: fieldB

        width: parent.width * 0.2
        height: parent.height

        font.pixelSize: fontPixelSize
        maximumLength: 3
        validator: IntValidator { bottom: 0; top: 255 }
        leftPadding: width * 0.2
        horizontalAlignment: Text.AlignHCenter

        Component.onCompleted: text = valueB

        Keys.onPressed: {
            // enter pressed
            if(event.key === 16777220) {
                valueB = text * 1

                focus = false
            }
            // esc pressed
            else if(event.key === 16777216) {
                focus = false
            }
        }

        onFocusChanged: {
            if(!focus) {
                text = valueB
            }
            else {
                selectAll()
            }
        }
    }

    TextField {
        id: fieldG

        width: parent.width * 0.2
        height: parent.height

        font.pixelSize: fontPixelSize
        maximumLength: 3
        validator: IntValidator { bottom: 0; top: 255 }
        leftPadding: width * 0.2
        horizontalAlignment: Text.AlignHCenter

        Component.onCompleted: text = valueG

        Keys.onPressed: {
            // enter pressed
            if(event.key === 16777220) {
                valueG = text * 1

                focus = false
            }
            // esc pressed
            else if(event.key === 16777216) {
                focus = false
            }
        }

        onFocusChanged: {
            if(!focus) {
                text = valueG
            }
            else {
                selectAll()
            }
        }
    }

    TextField {
        id: fieldR

        width: parent.width * 0.2
        height: parent.height

        font.pixelSize: fontPixelSize
        maximumLength: 3
        validator: IntValidator { bottom: 0; top: 255 }
        leftPadding: width * 0.2
        horizontalAlignment: Text.AlignHCenter

        Component.onCompleted: text = valueR

        Keys.onPressed: {
            // enter pressed
            if(event.key === 16777220) {
                valueR = text * 1

                focus = false
            }
            // esc pressed
            else if(event.key === 16777216) {
                focus = false
            }
        }

        onFocusChanged: {
            if(!focus) {
                text = valueR
            }
            else {
                selectAll()
            }
        }
    }
}
