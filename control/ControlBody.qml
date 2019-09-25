import QtQuick 2.3

import QtQuick.Controls 2.0

Rectangle {
    id: root

    color: "#ccc"

    property bool isRecordingON: true

    ///... copy codes from the old one whenever component is added.
    property alias isHorizontalFlipChecked: controlFlip.isHorizontalFlipChecked
    property alias isVerticalFlipChecked: controlFlip.isVerticalFlipChecked

    property var outputFilter
    property var videoOutput

    Flickable {
        id: scrollView

        width: parent.width * 0.95
        height: parent.height

        contentHeight: columnContainer.height

        ScrollBar.vertical: ScrollBar {
            width: root.width - scrollView.width; height: root.height

            orientation: Qt.Vertical

            anchors.right: parent.right
        }

        Column {
            id: columnContainer

            width: parent.width * 0.8

            anchors.horizontalCenter: parent.horizontalCenter

            Item {
                width: 1
                height: root.height * 0.01
            }

            ControlOutputTypes {
                id: controlOutputTypes

                width: parent.width
                height: root.height * 0.2

                videoSource: defaultVideoSource

                property alias rootHeight: root.height

                onRootHeightChanged: {
                    if(rootHeight < 450)
                        height = rootHeight * 0.5
                    else if(rootHeight < 600)
                        height = rootHeight * 0.3
                    else
                        height = rootHeight * 0.2
                }

                onVideoSourceUpdated: {
                    videoOutput.videoSource = path

                    if(videoOutput.outputType === videoOutput.k_output_type_video) {
                        videoOutput.startVideo()
                    }
                }

                onSignalRadioSampleChecked: {
                    if(videoOutput.outputType === videoOutput.k_output_type_undefined)
                        videoOutput.startVideo()
                    else
                        videoSourceUpdated(defaultVideoSource)
                }

                onSignalRadioCameraChecked: videoOutput.startCamera()
                onSignalRadioVideoChecked: {
                    videoOutput.videoSource = videoSource

                    videoOutput.startVideo()
                }
            }

            Item {
                width: 1
                height: root.height * 0.01
            }


            Item {
                id: itemStartSwitch

                width: parent.width
                height: root.height * 0.08

                enabled: (controlOutputTypes.statusChecked !== -1)

                Item {
                    anchors.fill: parent

                    Label {
                        text: qsTr("START")

                        font.pixelSize: fontSize15

                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Switch {
                        id: switchStart
                        checked: false

                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                    }
                }
            }

            Item {
                width: 1; height: parent.height * 0.01

                visible: root.height < 450 ? true : false
            }

            Rectangle {
                width: parent.width; height: 1

                color: "white"
            }

            Item {
                width: 1; height: parent.height * 0.02
            }

            ControlFlip {
                id: controlFlip

                width: parent.width

                enabled: switchStart.checked
            }

            ControlFilter {
                id: controlFilter

                width: parent.width

                enabled: switchStart.checked

                targetFilter: outputFilter
            }
        }
    }
}
