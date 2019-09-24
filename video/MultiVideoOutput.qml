import QtQuick 2.3
import QtMultimedia 5.5

// can play multi video types
// support camera, media
// support filter

// video output which supports both of Media and Camera
// arguments
// setvideoSource
// setVideoFilter
// play()
// stop()
// cameraON
// mediaON

Item {
    id: root

    readonly property int k_output_type_undefined: -1
    readonly property int k_output_type_camera: 1
    readonly property int k_output_type_video: 2

    property int outputType: k_output_type_undefined
    property int videoLoops: MediaPlayer.Infinite

    property string videoSource
    property string playingVideoSource: "init"

    property var outputFilters: []

    onOutputTypeChanged: loaderOutput.updateSourceComponent()

    function startCamera() { outputType = k_output_type_camera }
    function startVideo() {
        console.log("start video")
        if(videoSource !== playingVideoSource) {
            loaderOutput.item.start()
        }

        outputType = k_output_type_video
    }

    function stopCamera() { outputType = k_output_type_undefined }
    function stopVideo() { outputType = k_output_type_undefined }

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

            if(outputType == k_output_type_camera)
                sourceComponent = componentCamera
            else if(outputType == k_output_type_video)
                sourceComponent = componentVideo
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

                    filters: outputFilters

                    fillMode: VideoOutput.PreserveAspectFit

                    anchors.fill: parent
                }
            }
        }

        Component {
            id: componentVideo

            MultiVideoOutputComponentBody {
                Component.onCompleted:  {
                    start.connect(media.safeStart)
                    stop.connect(media.stop)
                }

                MediaPlayer {
                    id: media

                    loops: videoLoops

                    function safeStart() {
                        media.stop()
                        source = playingVideoSource = videoSource
                        media.play()
                    }
                }

                VideoOutput {
                    source: media

                    filters: outputFilters

                    fillMode: VideoOutput.PreserveAspectFit

                    anchors.fill: parent
                }
            }
        }
    }
}
