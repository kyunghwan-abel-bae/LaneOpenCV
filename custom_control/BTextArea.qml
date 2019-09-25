import QtQuick 2.3

import QtQuick.Controls 1.1
import QtQuick.Controls 2.0
Item {
    property string valueText

    ScrollView {
        //property alias valueText: textLog.text

        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff

        TextArea {
            id: textLog

            width: parent.width * 0.9
            height: width

            text: valueText

            readOnly: true
            selectByMouse: true
            wrapMode: Text.Wrap
            textFormat: Text.RichText
            font.pixelSize: 9

            background: Rectangle {
                border.color: "transparent"
                color: "transparent"
            }

            anchors {
                horizontalCenter: parent.horizontalCenter
            }

            onTextChanged: {
                if(textLog.length > 10000) {
                    textLog.text = textLog.text.substring(9000, textLog.text.length-1)
                }
            }
        }
    }
}
