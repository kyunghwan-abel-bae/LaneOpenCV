import QtQuick 2.3

import QtQuick.Controls 2.0

CheckBox {
    id: checkbox

    property real size: 1

    property real fontPixelSize_contentItem

    Component.onCompleted: {
        var indicator = checkbox.indicator
        var contentItem = checkbox.contentItem

        indicator.implicitWidth = indicator.implicitWidth * size
        indicator.implicitHeight = indicator.implicitWidth

        indicator.children[0].width = indicator.children[0].width * size
        indicator.children[0].height = indicator.children[0].width

        fontPixelSize_contentItem = contentItem.font.pixelSize * size
        contentItem.font.pixelSize = fontPixelSize_contentItem
    }
}
