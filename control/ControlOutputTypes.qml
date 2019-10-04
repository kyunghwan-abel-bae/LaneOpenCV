import QtQuick 2.3

import QtQuick.Controls 1.4
import QtQuick.Controls 2.0

import QtQuick.Dialogs 1.0

Item {
    id: root

    readonly property int k_radioSampleChecked: 1
    readonly property int k_radioCameraChecked: 1
    readonly property int k_radioVideoChecked: 2

    property alias videoFileName: radioVideo.strSelectedVideoName

    property int statusChecked: -1

    property string videoSource

    signal signalRadioSampleChecked()
    signal signalRadioCameraChecked()
    signal signalRadioVideoChecked()

    signal videoSourceUpdated(var path)

    onSignalRadioSampleChecked: {
        statusChecked = k_radioSampleChecked

        rowSelectVideo.visible = false
    }

    onSignalRadioCameraChecked: {
        statusChecked = k_radioCameraChecked

        rowSelectVideo.visible = false
    }

    onSignalRadioVideoChecked: {
        statusChecked = k_radioVideoChecked

        rowSelectVideo.visible = true
    }

    Text {
        id: textOutputTypes

        width: parent.width
        height: parent.height * 0.3

        text: qsTr("Output Types")

        font.bold: true
        font.pixelSize: fontSize15
    }

    Column {
        id: columnOutputTypes

        width: parent.width
        height: parent.height - textOutputTypes.height

        anchors.top: textOutputTypes.bottom

        RadioButton {
            id: radioSample

            height: parent.height * 0.3

            text: qsTr("SAMPLE")

            font.pixelSize: fontSize12

            onClicked: signalRadioSampleChecked()
        }

        RadioButton {
            id: radioCamera

            height: parent.height * 0.3

            text: qsTr("CAMERA")

            font.pixelSize: fontSize12

            onClicked: signalRadioCameraChecked()
        }

        RadioButton {
            id: radioVideo

            height: parent.height * 0.3

            text: strSelectedVideoPath.length === 0 ? qsTr("VIDEO") : qsTr("VIDEO (") + strSelectedVideoName + ")"

            font.pixelSize: fontSize12

            property alias strSelectedVideoPath: root.videoSource//"/Users/Abel/Shared/2019_2/Automotive/opencv/sample_videos/0.mp4"
            property string strSelectedVideoName: "0.mp4"

            onStrSelectedVideoPathChanged: {
                videoSourceUpdated(strSelectedVideoPath)

                var path = strSelectedVideoPath
                var split_path = path.split("/")
                strSelectedVideoName = split_path[split_path.length-1]
            }

            onClicked: signalRadioVideoChecked()
        }

        Row {
            id: rowSelectVideo

            width: parent.width * 0.9
            height: radioVideo.height * 0.8

            visible: false

            anchors.right: parent.right

            TextField {
                id: textSelectVideo

                width: parent.width * 0.7
                height: parent.height

                font.pixelSize: radioVideo.font.pixelSize * 0.8
                hoverEnabled: true

                Component.onCompleted: text = radioVideo.strSelectedVideoPath

                Keys.onPressed: {
                    // enter pressed
                    if(event.key === 16777220) {
                        radioVideo.strSelectedVideoPath = text
                        rowSelectVideo.visible = false

                        focus = false
                    }

                    // esc pressed
                    else if(event.key === 16777216) {
                        focus = false
                    }
                }

                onFocusChanged: {
                    if(!focus) {
                        text = radioVideo.strSelectedVideoPath
                    }
                    else {
                        selectAll()
                    }
                }
            }

            Button {
                id: btnSelectVideo

                width: parent.width - textSelectVideo.width
                height: parent.height

                text: qsTr("FIND")

                font.pixelSize: textSelectVideo.font.pixelSize

                onClicked: {
                    dialogVideoFile.open()
                }

                FileDialog {
                    id: dialogVideoFile
                    title: qsTr("Please select a video")
                    folder: radioVideo.strSelectedVideoPath //shortcuts.home

                    onAccepted: {
                        textSelectVideo.text = dialogVideoFile.fileUrl
                        textSelectVideo.focus = true
                    }
                }
            }
        }

    }
}
