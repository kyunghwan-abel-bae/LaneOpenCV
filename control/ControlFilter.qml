import QtQuick 2.3

import QtQuick.Controls 2.0

import "../custom_control"

GroupBox {
    id: root

    title: qsTr("Filters")

    font.pixelSize: fontSize11

    anchors.horizontalCenter: parent.horizontalCenter

    property alias isHSVChecked: checkHSV.checked
    property alias isMaskChecked: checkMask.checked
    property alias isMaskColorChecked: checkMaskColor.checked
    property alias isMaskColorFinderChecked: checkMaskColorFinder.checked
    property alias isGrayChecked: checkGray.checked
    property alias isGaussianChecked: checkGaussian.checked
    property alias isCannyChecked: checkCanny.checked
    property alias isROIChecked: checkROI.checked
    property alias isLineOnlyChecked: checkLineOnly.checked
    property alias isSteeringChecked: checkSteering.checked
    property alias isSteeringStabilizationChecked: checkSteeringStabilzation.checked
    property alias isLineOnImageChecked: checkLineOnImage.checked
    property alias isFinalChecked: checkFinal.checked

    property var arrBGRLowerWhite: []
    property var arrBGRUpperWhite: []
    property var arrBGRLowerYellow: []
    property var arrBGRUpperYellow: []

    property var arrBGRFirstMask: []
    property var arrBGRSecondMask: []

    property bool areROIValuesInitialized: false

    property int valueLowerWhiteB: 200
    property int valueLowerWhiteG: 200
    property int valueLowerWhiteR: 200

    property int valueUpperWhiteB: 255
    property int valueUpperWhiteG: 255
    property int valueUpperWhiteR: 255

    property int valueLowerYellowB: 89
    property int valueLowerYellowG: 133
    property int valueLowerYellowR: 133

    property int valueUpperYellowB: 162
    property int valueUpperYellowG: 255
    property int valueUpperYellowR: 255

    property int valueLowerFinderB: 89
    property int valueLowerFinderG: 133
    property int valueLowerFinderR: 133

    property int valueUpperFinderB: 162
    property int valueUpperFinderG: 255
    property int valueUpperFinderR: 255

    property int valuePoint0_X: -1
    property int valuePoint0_Y: -1
    property int valuePoint1_X: -1
    property int valuePoint1_Y: -1
    property int valuePoint2_X: -1
    property int valuePoint2_Y: -1
    property int valuePoint3_X: -1
    property int valuePoint3_Y: -1

    property var arrMaskValues : [
        valueLowerWhiteB, valueLowerWhiteG, valueLowerWhiteR,
        valueUpperWhiteB, valueUpperWhiteG, valueUpperWhiteR,
        valueLowerYellowB, valueLowerYellowG, valueLowerYellowR,
        valueUpperYellowB, valueUpperYellowG, valueUpperYellowR,
    ]
    property var targetFilter

    onIsMaskColorCheckedChanged: {
        isHSVChecked = true
    }

    onIsROICheckedChanged: {
        if(!areROIValuesInitialized) {
            get_roi_values()
            areROIValuesInitialized = true
        }
    }

    onIsFinalCheckedChanged: {
        if(isFinalChecked) {
            isHSVChecked = true
            isMaskChecked = true
            isMaskColorChecked = true
            isMaskColorFinderChecked = false
            isGrayChecked = true
            isGaussianChecked = true
            isCannyChecked = true
            isROIChecked = true
            isLineOnlyChecked = true
            isSteeringChecked = true
            isSteeringStabilizationChecked = true
            isLineOnImageChecked = true
        }
        else {
            isLineOnImageChecked = false
            isLineOnlyChecked = false
            isSteeringStabilizationChecked = false
            isSteeringChecked = false
            isROIChecked = false
            isCannyChecked = false
            isGaussianChecked = false
            isGrayChecked = false
            isMaskColorFinderChecked = false
            isMaskColorChecked = false
            isMaskChecked = false
            isHSVChecked = false
        }
    }

    function get_roi_values() {
        var map = targetFilter.map_points_values();

        valuePoint0_X = Math.round(map.roi_point0_x)
        valuePoint1_X = Math.round(map.roi_point1_x)
        valuePoint2_X = Math.round(map.roi_point2_x)
        valuePoint3_X = Math.round(map.roi_point3_x)

        valuePoint0_Y = Math.round(map.roi_point0_y)
        valuePoint1_Y = Math.round(map.roi_point1_y)
        valuePoint2_Y = Math.round(map.roi_point2_y)
        valuePoint3_Y = Math.round(map.roi_point3_y)
    }

    Column {
        anchors.fill: parent

        Item {
            id: itemOrigianl

            width: parent.width
            height: children[0].height

            CheckBox {
                id: checkOriginal

                text: qsTr("Original")
                font.pixelSize: fontSize11

                checked: true
                checkable: false
            }
        }

        Item {
            id: itemMask

            width: parent.width
            height: children[0].height + children[1].height

            CheckBox {
                id: checkMask

                text: qsTr("Mask")
                font.pixelSize: fontSize11

                onCheckedChanged: {
                    if(checked) {
                        colMask.visible = true
                        colMask.height = checkMask.height * 3
                    }
                    else {
                        checkMaskColor.checked = false
                        checkMaskColorFinder.checked = false

                        colMask.height = 0
                        colMask.visible = false
                    }
                }
            }

            Column {
                id: colMask

                width: parent.width * 0.95
                height: 0//checkMask.height * 2

                visible: false

                anchors {
                    right: parent.right
                    bottom: parent.bottom
                }

                Item {
                    id: itemHSV

                    width: parent.width
                    height: children[0].height

                    BCheckBox {
                        id: checkHSV

                        text: qsTr("HSV")

                        size: 0.8
                    }
                }

                Item {
                    id: itemMaskColor

                    width: parent.width
                    height: checkMaskColor.height + colMaskColor.height

                    BCheckBox {
                        id: checkMaskColor

                        text: isMaskColorFinderChecked ? qsTr("(Not Active)Mask Color") : qsTr("Mask Color")

                        size: 0.8

                        onCheckedChanged: {
                            if(checked) {
                                colMask.height += checkMask.height * 4
                                colMaskColor.height = checkMask.height * 4
                                colMaskColor.visible = true
                            }
                            else {
                                colMaskColor.visible = false
                                colMaskColor.height = 0
                                colMask.height -= checkMask.height * 4
                            }
                        }
                    }

                    Column {
                        id: colMaskColor

                        width: parent.width * 0.85
                        height: 0

                        visible: false

                        anchors.top: checkMaskColor.bottom

                        spacing: height * 0.05

                        ControlBGRTextField {
                            id: textFieldLowerWhite

                            width: parent.width
                            height: checkMaskColor.height

                            valueB: valueLowerWhiteB
                            valueG: valueLowerWhiteG
                            valueR: valueLowerWhiteR

                            fontPixelSize: checkMaskColor.fontPixelSize_contentItem

                            labelValue: qsTr("Lower White (B,G,R)")

                            onValueBChanged: valueLowerWhiteB = valueB
                            onValueGChanged: valueLowerWhiteG = valueG
                            onValueRChanged: valueLowerWhiteR = valueR

                        }

                        ControlBGRTextField {
                            id: textFieldUpperWhite

                            width: parent.width
                            height: checkMaskColor.height

                            valueB: valueUpperWhiteB
                            valueG: valueUpperWhiteG
                            valueR: valueUpperWhiteR

                            fontPixelSize: checkMaskColor.fontPixelSize_contentItem

                            labelValue: qsTr("Upper White (B,G,R)")

                            onValueBChanged: valueUpperWhiteB = valueB
                            onValueGChanged: valueUpperWhiteG = valueG
                            onValueRChanged: valueUpperWhiteR = valueR
                        }

                        ControlBGRTextField {
                            id: textFieldLowerYellow

                            width: parent.width
                            height: checkMaskColor.height

                            valueB: valueLowerYellowB
                            valueG: valueLowerYellowG
                            valueR: valueLowerYellowR

                            fontPixelSize: checkMaskColor.fontPixelSize_contentItem

                            labelValue: qsTr("Lower Yellow (B,G,R)")

                            onValueBChanged: valueLowerYellowB = valueB
                            onValueGChanged: valueLowerYellowG = valueG
                            onValueRChanged: valueLowerYellowR = valueR
                        }

                        ControlBGRTextField {
                            id: textFieldUpperYellow

                            width: parent.width
                            height: checkMaskColor.height

                            valueB: valueUpperYellowB
                            valueG: valueUpperYellowG
                            valueR: valueUpperYellowR

                            fontPixelSize: checkMaskColor.fontPixelSize_contentItem

                            labelValue: qsTr("Upper Yellow (B,G,R)")

                            onValueBChanged: valueUpperYellowB = valueB
                            onValueGChanged: valueUpperYellowG = valueG
                            onValueRChanged: valueUpperYellowR = valueR
                        }
                    }
                }

                Item {
                    id: itemMaskColorFinder

                    width: parent.width
                    height: checkMaskColorFinder.height + itemMaskColorFinderController.height

                    BCheckBox {
                        id: checkMaskColorFinder

                        text: checked ? qsTr("(Active) Mask Color Finder") : qsTr("Mask Color Finder")

                        size: 0.8

                        onCheckedChanged: {
                            if(checked) {
                                colMask.height += checkMask.height * 7
                                itemMaskColorFinderController.visible = true
                            }
                            else {
                                itemMaskColorFinderController.visible = false
                                colMask.height -= checkMask.height * 7
                            }
                        }
                    }

                    Item {
                        id: itemMaskColorFinderController

                        width: parent.width * 0.85
                        height: checkMaskColorFinder.height * 4

                        visible: false

                        anchors {
                            top: checkMaskColorFinder.bottom
                            right: parent.right
                        }

                        Label {
                            id: lblFirstMask

                            height: parent.height * 0.1

                            text: qsTr("First Mask")

                            font.pixelSize: checkMaskColorFinder.fontPixelSize_contentItem

                            verticalAlignment: Text.AlignVCenter

                            anchors.topMargin: height
                        }

                        Rectangle {
                            id: rectFirstMaskColor

                            width: height
                            height: lblFirstMask.height

                            border.color: "black"

                            anchors {
                                left: lblFirstMask.right
                                leftMargin: width * 0.5
                                verticalCenter: lblFirstMask.verticalCenter
                            }

                            Component.onCompleted: set_color()

                            function set_color() {
                                var str_color = "#" + sliderFR.valueHex + "" + sliderFG.valueHex + "" + sliderFB.valueHex

                                color = str_color

                                targetFilter.numLB = sliderFB.valueDec
                                targetFilter.numLG = sliderFG.valueDec
                                targetFilter.numLR = sliderFR.valueDec
                            }
                        }

                        Column {
                            id: colFirstMaskColorController

                            width: parent.width
                            height: parent.height - lblFirstMask.height

                            spacing: height * 0.05

                            anchors.top: lblFirstMask.bottom

                            BSlider {
                                id: sliderFB

                                width: parent.width
                                height: checkMaskColorFinder.height

                                name: qsTr("B")

                                value: valueLowerFinderB

                                onValueHexChanged: rectFirstMaskColor.set_color()
                            }

                            BSlider {
                                id: sliderFG

                                width: parent.width
                                height: checkMaskColorFinder.height

                                name: qsTr("G")

                                value: valueLowerFinderG

                                onValueHexChanged: rectFirstMaskColor.set_color()
                            }

                            BSlider {
                                id: sliderFR

                                width: parent.width
                                height: checkMaskColorFinder.height

                                name: qsTr("R")

                                value: valueLowerFinderR

                                onValueHexChanged: rectFirstMaskColor.set_color()
                            }
                        }

                        Label {
                            id: lblSecondMask

                            height: lblFirstMask.height//parent.height * 0.1

                            text: qsTr("Second Mask")

                            font.pixelSize: checkMaskColorFinder.fontPixelSize_contentItem

                            verticalAlignment: Text.AlignVCenter

                            anchors {
                                top: colFirstMaskColorController.bottom
                                topMargin: height
                            }
                        }

                        Rectangle {
                            id: rectSecondMaskColor

                            width: height
                            height: lblFirstMask.height

                            border.color: "black"

                            anchors {
                                left: lblSecondMask.right
                                leftMargin: width * 0.5
                                verticalCenter: lblSecondMask.verticalCenter
                            }

                            Component.onCompleted: set_color()

                            function set_color() {
                                var str_color = "#" + sliderSR.valueHex + "" + sliderSG.valueHex + "" + sliderSB.valueHex

                                color = str_color

                                targetFilter.numUB = sliderSB.valueDec
                                targetFilter.numUG = sliderSG.valueDec
                                targetFilter.numUR = sliderSR.valueDec
                            }
                        }


                        Column {
                            id: colSecondMaskColorController

                            width: parent.width
                            height: parent.height - lblSecondMask.height

                            spacing: height * 0.05

                            anchors.top: lblSecondMask.bottom

                            BSlider {
                                id: sliderSB

                                width: parent.width
                                height: checkMaskColorFinder.height

                                name: qsTr("B")

                                value: valueUpperFinderB

                                onValueHexChanged: rectSecondMaskColor.set_color()
                            }

                            BSlider {
                                id: sliderSG

                                width: parent.width
                                height: checkMaskColorFinder.height

                                name: qsTr("G")

                                value: valueUpperFinderG

                                onValueHexChanged: rectSecondMaskColor.set_color()
                            }

                            BSlider {
                                id: sliderSR

                                width: parent.width
                                height: checkMaskColorFinder.height

                                name: qsTr("R")

                                value: valueUpperFinderR

                                onValueHexChanged: rectSecondMaskColor.set_color()
                            }
                        }
                    }
                }
            }
        }

        Item {
            id: itemGray

            width: parent.width
            height: children[0].height

            CheckBox {
                id: checkGray

                text: qsTr("Gray scale")
                font.pixelSize: fontSize11
            }
        }

        Item {
            width: parent.width
            height: children[0].height

            CheckBox {
                id: checkGaussian

                text: qsTr("Gaussian")
                font.pixelSize: fontSize11
            }
        }

        Item {
            width: parent.width
            height: children[0].height

            CheckBox {
                id: checkCanny

                text: qsTr("Canny")
                font.pixelSize: fontSize11
            }
        }

        Item {
            width: parent.width
            height: children[0].height + children[1].height

            CheckBox {
                id: checkROI

                text: qsTr("Region of Interest")
                font.pixelSize: fontSize11

                onCheckedChanged: {
                    if(checked) {
                        colROI.visible = true
                        colROI.height = checkROI.height * 5
                    }
                    else {
                        colROI.height = 0
                        colROI.visible = false
                    }
                }
            }

            Column {
                id: colROI

                width: parent.width * 0.95
                height: 0
                visible: false

                anchors {
                    right: parent.right
                    bottom: parent.bottom
                }

                spacing: width * 0.05

                ControlROITextField {
                    id: textFieldPoint0

                    width: parent.width
                    height: checkROI.height

                    valueX: valuePoint0_X
                    valueY: valuePoint0_Y

                    fontPixelSize: checkROI.font.pixelSize

                    valueLabel: qsTr("Point0(x,y)")

                    onValueXChanged: valuePoint0_X = valueX
                    onValueYChanged: valuePoint0_Y = valueY
                }

                ControlROITextField {
                    id: textFieldPoint1

                    width: parent.width
                    height: checkROI.height

                    valueX: valuePoint1_X
                    valueY: valuePoint1_Y

                    fontPixelSize: checkROI.font.pixelSize

                    valueLabel: qsTr("Point1(x,y)")

                    onValueXChanged: valuePoint1_X = valueX
                    onValueYChanged: valuePoint1_Y = valueY
                }

                ControlROITextField {
                    id: textFieldPoint2

                    width: parent.width
                    height: checkROI.height

                    valueX: valuePoint2_X
                    valueY: valuePoint2_Y

                    fontPixelSize: checkROI.font.pixelSize

                    valueLabel: qsTr("Point2(x,y)")

                    onValueXChanged: valuePoint2_X = valueX
                    onValueYChanged: valuePoint2_Y = valueY
                }

                ControlROITextField {
                    id: textFieldPoint3

                    width: parent.width
                    height: checkROI.height

                    valueX: valuePoint3_X
                    valueY: valuePoint3_Y

                    fontPixelSize: checkROI.font.pixelSize

                    valueLabel: qsTr("Point3(x,y)")

                    onValueXChanged: valuePoint3_X = valueX
                    onValueYChanged: valuePoint3_Y = valueY
                }
            }
        }

        Item {
            width: parent.width
            height: children[0].height

            CheckBox {
                id: checkLineOnly

                text: qsTr("Line Only")
                font.pixelSize: fontSize11
            }
        }

        Item {
            width: parent.width
            height: children[0].height

            CheckBox {
                id: checkSteering

                text: qsTr("Steering")
                font.pixelSize: fontSize11
            }
        }

        Item {
            width: parent.width
            height: children[0].height

            CheckBox {
                id: checkSteeringStabilzation

                text: qsTr("Steering Stabilization")
                font.pixelSize: fontSize11
            }
        }

        Item {
            width: parent.width
            height: children[0].height

            CheckBox {
                id: checkLineOnImage

                text: qsTr("Line On Image")
                font.pixelSize: fontSize11
            }
        }

        Item {
            width: parent.width
            height: children[0].height

            CheckBox {
                id: checkFinal

                text: qsTr("Final")
                font.pixelSize: fontSize11
            }
        }
    }
}
