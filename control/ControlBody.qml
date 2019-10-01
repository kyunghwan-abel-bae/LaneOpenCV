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
    property alias startOn: switchStart.checked

    property int filterTimeInterval: 3000

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
        interval: filterTimeInterval; repeat: true

        property int valueLowerWhiteB
        property int valueLowerWhiteG
        property int valueLowerWhiteR

        property int valueUpperWhiteB
        property int valueUpperWhiteG
        property int valueUpperWhiteR

        property int valueLowerYellowB
        property int valueLowerYellowG
        property int valueLowerYellowR

        property int valueUpperYellowB
        property int valueUpperYellowG
        property int valueUpperYellowR

        property var arrMaskValues : [
            valueLowerWhiteB, valueLowerWhiteG, valueLowerWhiteR,
            valueUpperWhiteB, valueUpperWhiteG, valueUpperWhiteR,
            valueLowerYellowB, valueLowerYellowG, valueLowerYellowR,
            valueUpperYellowB, valueUpperYellowG, valueUpperYellowR,
        ]

        Component.onCompleted: {
            /*
            for(var i=0;i<arrMaskValues.length;i++)
                arrMaskValues[i] = controlFilter.arrMaskValues[i]
            */
        }

        function check_mask_colors_changed() {
            var is_changed = false

            for(var i=0;i<arrMaskValues.length;i++) {
                console.log("arrMaskValues[i] : " + arrMaskValues[i] + ", cont.arr[i] : " + controlFilter.arrMaskValues[i])
                if(arrMaskValues[i] !== controlFilter.arrMaskValues[i]) {
                    arrMaskValues[i] = controlFilter.arrMaskValues[i]
                    is_changed = true
                }
            }

            return is_changed
        }

        onTriggered: {
            var map_bool = mapFilterBool
            var map_bool_keys = Object.keys(map_bool)

            var is_log_activated = false

            if(controlFilter.isMaskColorChecked) {
//            if(controlFilter.isMaskColorChecked
//                    && check_mask_colors_changed()) {
                console.log("valueLowerWhiteR : " + controlFilter.valueLowerWhiteR)
                console.log("valueUpperWhiteR : " + controlFilter.valueUpperWhiteR)

                /*
                outputFilter.numLWB = controlFilter.valueLowerWhiteB
                outputFilter.numLWG = controlFilter.valueLowerWhiteG
                outputFilter.numLWR = controlFilter.valueLowerWhiteR

                outputFilter.numUWB = controlFilter.valueUpperWhiteB
                outputFilter.numUWG = controlFilter.valueUpperWhiteG
                outputFilter.numUWR = controlFilter.valueUpperWhiteR
                */

//                var map_mask_values = {}

//                map_mask_values.valueLowerWhiteB = controlFilter.valueLowerWhiteB
//                map_mask_values.valueLowerWhiteG = controlFilter.valueLowerWhiteG
//                map_mask_values.valueLowerWhiteR = controlFilter.valueLowerWhiteR

//                map_mask_values.valueUpperWhiteB = controlFilter.valueUpperWhiteB
//                map_mask_values.valueUpperWhiteG = controlFilter.valueUpperWhiteG
//                map_mask_values.valueUpperWhiteR = controlFilter.valueUpperWhiteR

//                map_mask_values.valueLowerYellowB = controlFilter.valueLowerYellowB
//                map_mask_values.valueLowerYellowG = controlFilter.valueLowerYellowG
//                map_mask_values.valueLowerYellowR = controlFilter.valueLowerYellowR

//                map_mask_values.valueUpperYellowB = controlFilter.valueUpperYellowB
//                map_mask_values.valueUpperYellowG = controlFilter.valueUpperYellowG
//                map_mask_values.valueUpperYellowR = controlFilter.valueUpperYellowR

//                outputFilter.SetBGRMaskValues(map_mask_values)
            }


            if(map_bool_keys.length > 0) {
                console.log("map_bool_keys : " + map_bool_keys)




                // roi should be here

                outputFilter.setMapBoolForProcess(map_bool)
                mapFilterBool = {}

                is_log_activated = true
            }

            // roi
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

                outputFilter.set_map_points_values(map_roi_points);

                // test by KH -- Is it really required?
                is_log_activated = true
            }

            // Is it really required?
            // Because of map_bool_keys,
            // I guess that is_log_activated is already changed.
            if(controlFilter.isMaskColorChecked
                    || controlFilter.isMaskColorFinderChecked) {
                is_log_activated = true
            }




            // test by KH -- should be removed
            /*
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
            str_value_text += "hello22"
            controlLog.valueText = str_value_text

                    */


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

