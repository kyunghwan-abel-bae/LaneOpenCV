import QtQuick 2.3

// added by KH - should be removed
import QtQuick.Controls 1.4

import QtQuick.Controls 2.0

// added by KH - should be removed
import "../custom_control"

Rectangle {
    id: root

    color: "#ccc"

    property bool isRecordingON: true

    ///... copy codes from the old one whenever component is added.
    property var outputFilter
    property var videoOutput

    property var mapFilterBool

    Component.onCompleted: {
        var jsMap = {}
        mapFilterBool = jsMap
    }

    // added by KH for commentary
    // filter effect interval
    Timer {
        id: timerControl

        running: true
        interval: 3000; repeat: true

        onTriggered: {
            var map_bool = mapFilterBool
            var map_bool_keys = Object.keys(map_bool)

            var is_log_activated = false

            // for test
            if(map_bool_keys.length > 0) {
                console.log("map_bool : " + JSON.stringify(map_bool))
                mapFilterBool = {}
            }

            if(controlFilter.isMaskColorChecked) {
                var text = "- Upper White(B,G,R) : (" + controlFilter.valueUpperWhiteB + "," + controlFilter.valueUpperWhiteG + "," + controlFilter.valueUpperWhiteR + ")"
                console.log("text : " + text)
            }

            if(controlFilter.isROIChecked) {
                var map_roi_points = {}

                map_roi_points.roi_point0_x = controlFilter.valuePoint0_X
                map_roi_points.roi_point1_x = controlFilter.valuePoint1_X
                map_roi_points.roi_point2_x = controlFilter.valuePoint2_X
                map_roi_points.roi_point3_x = controlFilter.valuePoint3_X

                map_roi_points.roi_point0_y = controlFilter.valuePoint0_Y
                map_roi_points.roi_point1_y = controlFilter.valuePoint1_Y
                map_roi_points.roi_point2_y = controlFilter.valuePoint2_Y
                map_roi_points.roi_point3_y = controlFilter.valuePoint3_Y

                console.log("map_roi_points : " + JSON.stringify(map_roi_points))
            }

//            console.log("hello")
            var str_value_text = controlLog.valueText
            str_value_text.append("hi")
            controlLog.valueText = str_value_text

            /*
            if(map_bool_keys.length > 0) {
                filter.setMapBoolForProcess(map_bool)
                mapFilterBool = {}

                is_log_activated = true
            }
            */


        }
    }

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

                onIsHorizontalFlipCheckedChanged: mapFilterBool.isHorizontalFlipChecked = isHorizontalFlipChecked
                onIsVerticalFlipCheckedChanged: mapFilterBool.isVerticalFlipChecked = isVerticalFlipChecked
            }

            Item {
                width: 1; height: parent.height * 0.02
            }

            ControlFilter {
                id: controlFilter

                width: parent.width

                enabled: switchStart.checked

                targetFilter: outputFilter

                onIsHSVCheckedChanged: mapFilterBool.isHSVChecked = isHSVChecked
                onIsMaskCheckedChanged: mapFilterBool.isMaskChecked = isMaskChecked
                onIsMaskColorCheckedChanged: mapFilterBool.isMaskColorChecked = isMaskColorChecked
                onIsMaskColorFinderCheckedChanged: mapFilterBool.isMaskColorFinderChecked = isMaskColorFinderChecked
                onIsGrayCheckedChanged: mapFilterBool.isGrayChecked = isGrayChecked
                onIsGaussianCheckedChanged: mapFilterBool.isGaussianChecked = isGaussianChecked
                onIsCannyCheckedChanged: mapFilterBool.isCannyChecked = isCannyChecked
                onIsROICheckedChanged: mapFilterBool.isROIChecked = isROIChecked
                onIsLineOnlyCheckedChanged: mapFilterBool.isLineOnlyChecked = isLineOnlyChecked
                onIsSteeringCheckedChanged: mapFilterBool.isSteeringChecked = isSteeringChecked
                onIsSteeringStabilizationCheckedChanged: mapFilterBool.isSteeringStabilizationChecked = isSteeringStabilizationChecked
                onIsLineOnImageCheckedChanged: mapFilterBool.isLineOnImageChecked = isLineOnImageChecked
            }

            Item {
                width: 1; height: parent.height * 0.02
            }

            ControlLog {
                id: controlLog

                width: parent.width
                height: width

                enabled: switchStart.checked

                valueButtonText: isRecordingON ? qsTr("STOP RECORDING") : qsTr("START RECORDING")

                onSingnalButtonLogClicked: isRecordingON = !isRecordingON
            }

            Item {
                width: 1; height: parent.height * 0.02
            }
        }
    }
}
