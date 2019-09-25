import QtQuick 2.3
import QtQuick.Controls 1.4

import "video"
import "control"

ApplicationWindow {
    id: root

    width: 1280
    height: 680

    visible: true

    // Handling font-sizes in a batch
    property int fontSize11: 11
    property int fontSize12: 12
    property int fontSize15: 15
    property int fontSize30: width * 0.024

    // Handling default media source for test
    //property string defaultVideoSource: "https://hanpage-wordpress.s3.ap-northeast-2.amazonaws.com/public/sample_lane.mp4"//"https://pixabay.com/videos/download/video-14205_large.mp4"//"https://d1fav6wwj1iaz1.cloudfront.net/wp-content/uploads/2019/09/20130943/sample_lanes.mp4"//"https://felgo.com/web-assets/video.mp4"//"file://" + Qt.resolvedUrl(".") + "/sample_videos/video1.mp4"//"Users/Abel/Shared/2019_2/Automotive/opencv/sample_videos/0.mp4"
//    property string defaultVideoSource: "http://d1fav6wwj1iaz1.cloudfront.net/public/sample_lane.mp4"//"https://pixabay.com/videos/download/video-14205_large.mp4"//"https://d1fav6wwj1iaz1.cloudfront.net/wp-content/uploads/2019/09/20130943/sample_lanes.mp4"//"https://felgo.com/web-assets/video.mp4"//"file://" + Qt.resolvedUrl(".") + "/sample_videos/video1.mp4"//"Users/Abel/Shared/2019_2/Automotive/opencv/sample_videos/0.mp4"
    property string defaultVideoSource: "file:///Users/Abel/Shared/2019_2/Automotive/opencv/sample_videos/sample_lane.mp4"

    onWidthChanged: {
        if(width < 801)
            output.width = width * 0.5
        else if(width < 1000)
            output.width = width * 0.65
        else
            output.width = width * 0.75
    }


    MultiVideoOutput {
        id: output

        width: parent.width * 0.75
        height: parent.height

        videoSource: defaultVideoSource
//        outputFilters: filter
    }

    // CVFilter {

    ControlBody {
        id: controlBody

        width: parent.width - output.width
        height: parent.height

        anchors.right: parent.right

        videoOutput: output

    // targetFilter: filter
    }


    // will be erased
    Timer {
        id: timer

        running: true; interval: 500
        repeat: true;

        onTriggered: {
            /*
            if(count === 0) {
                output.startCamera()
                count = 1
            }
            else {
            */

                //output.startMedia()
//                output.startVideo()
            /*
            }
            */

            //            output.cameraON = true
            //            output.mediaON = true
        }
    }

}
