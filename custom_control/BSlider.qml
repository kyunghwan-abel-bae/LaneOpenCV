import QtQuick 2.3
import QtQuick.Controls 2.0

Row {
    id: root

    property int defaultValue: 0

    property alias value: slider.value

    property int valueDec: value
    property string valueHex: "00"

    property string name

    onValueDecChanged: {
        textField.text = valueDec

        valueHex = valueDec === 0 ? "00" : valueDec.toString(16)
    }

    Label {
        text: name

        anchors.verticalCenter: parent.verticalCenter
    }

    Item {
        width: parent.width * 0.01
        height: parent.height
    }

    Slider {
        id: slider

        width: parent.width * 0.7
        height: parent.height

        from: 0
        to: 255
        stepSize: 1
    }

    Item {
        width: parent.width * 0.01
        height: parent.height
    }

    Item {
        width: parent.width * 0.2
        height: parent.height

        TextField {
            id: textField

            width: parent.width
            height: parent.height

            text: "0"

            maximumLength: 3
            validator: IntValidator { bottom: 0; top: 255 }
            inputMethodHints: Qt.ImhDigitsOnly

            background: Item {
                Rectangle {
                    width: parent.width * 0.7
                    height: 1

                    color: "#333"

                    anchors {
                        bottom: parent.bottom
                        bottomMargin: 5
                    }
                }
            }

            padding: 0

            Keys.onPressed: {
                // enter pressed
                if(event.key === 16777220) {
                    value = text * 1

                    focus = false
                }
                // esc pressed
                else if(event.key === 16777216) {
                    focus = false
                }
            }

            onFocusChanged: {
                if(!focus) {
                    text = value
                }
                else {
                    selectAll()
                }
            }
        }
    }
}
