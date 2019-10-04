import QtQuick 2.3

import QtQuick.Controls 2.0

Row {
    id: root

    spacing: height * 0.1

    property int valueX//: fieldX.text
    property int valueY//: fieldY.text

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

        font.pixelSize: fontPixelSize
        validator: IntValidator { bottom: 0; top: 9999 }

        text: valueX

        leftPadding: width * 0.2
        horizontalAlignment: Text.AlignHCenter

//        property int fieldValueX

//        Component.onCompleted: text = fieldValueX

        Keys.onPressed: {
            // enter pressed
            if(event.key === 16777220) {
//                fieldValueX = text * 1
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
//                text = fieldValueX
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

//        property int fieldValueY

//        Component.onCompleted: text = fieldValueY

        Keys.onPressed: {
            // enter pressed
            if(event.key === 16777220) {
//                fieldValueY = text * 1
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
                //text = fieldValueY
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
