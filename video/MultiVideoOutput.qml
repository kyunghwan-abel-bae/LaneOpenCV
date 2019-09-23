import QtQuick 2.3
import QtMultimedia 5.5

// can play multi video types
// support camera, media
// support filter

// video output which supports both of Media and Camera
// arguments
// setMediaSource
// setVideoFilter
// play()
// stop()
// cameraON
// mediaON

Item {
    id: root

    readonly property int k_video_type_undefined: -1
    readonly property int k_video_type_camera: 1
    readonly property int k_video_type_media: 2

    property int videoType: k_video_type_undefined
    property int mediaLoops: MediaPlayer.Infinite

    property string mediaSource

    property var videoFilters: []

    onVideoTypeChanged: loaderOutput.updateSourceComponent()

    function startCamera() { videoType = k_video_type_camera }
    function startMedia() { videoType = k_video_type_media }

    function stopCamera() { videoType = k_video_type_undefined }
    function stopMedia() { videoType = k_video_type_undefined }

    // using Loader for QML's potential risks
    // when change the video type.
    Loader {
        id: loaderOutput

        sourceComponent: componentMock

        anchors.fill: parent

        onLoaded: { loaderOutput.item.start() }

        function updateSourceComponent() {
            if(loaderOutput.item !== null)
                loaderOutput.item.stop()

            if(videoType == k_video_type_camera)
                sourceComponent = componentCamera
            else if(videoType == k_video_type_media)
                sourceComponent = componentMedia
            else
                sourceComponent = componentMock
        }

        Component {
            id: componentMock

            MultiVideoOutputComponentBody { }
        }

        Component {
            id: componentCamera

            MultiVideoOutputComponentBody {
                Component.onCompleted: {
                    // camera starts without start().
                    stop.connect(camera.stop)
                }

                Camera {
                    id: camera
                }

                VideoOutput {
                    source: camera

                    filters: videoFilters

                    fillMode: VideoOutput.PreserveAspectFit

                    anchors.fill: parent
                }
            }
        }

        Component {
            id: componentMedia

            MultiVideoOutputComponentBody {
                Component.onCompleted:  {
                    start.connect(media.play)
                    stop.connect(media.stop)
                }

                MediaPlayer {
                    id: media

                    loops: mediaLoops

                    source: mediaSource
                }

                VideoOutput {
                    source: media

                    filters: videoFilters

                    fillMode: VideoOutput.PreserveAspectFit

                    anchors.fill: parent
                }
            }
        }
    }
}
