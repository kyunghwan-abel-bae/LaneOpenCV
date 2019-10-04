#include "adas_filter.h"
#include "opencvhelper.h"
#include "rgbframehelper.h"
//#include "b_filter.h"

#include <QFile>

#include <cmath>

/*
QVideoFilterRunnable* ADASFilter::createFilterRunnable() {
    Init();
//    return new FilterRunnable(this);
}
*/

void ADASFilter::Init() {
    isHorizontalFlipChecked = false;
    isVerticalFlipChecked = false;
    isHSVChecked = false;
    isMaskChecked = false;
    isMaskColorChecked = false;
    isMaskColorFinderChecked = false;
    isGrayChecked = false;
    isGaussianChecked = false;
    isCannyChecked = false;
    isROIChecked = false;
    isLineOnlyChecked = false;
    isSteeringChecked = false;
    isSteeringStabilizationChecked = false;
    isLineOnImageChecked = false;

    map_filters_checked_status_["isHorizontalFlipChecked"] = &isHorizontalFlipChecked;
    map_filters_checked_status_["isVerticalFlipChecked"] = &isVerticalFlipChecked;
    map_filters_checked_status_["isHSVChecked"] = &isHSVChecked;
    map_filters_checked_status_["isMaskChecked"] = &isMaskChecked;
    map_filters_checked_status_["isMaskColorChecked"] = &isMaskColorChecked;
    map_filters_checked_status_["isMaskColorFinderChecked"] = &isMaskColorFinderChecked;
    map_filters_checked_status_["isGrayChecked"] = &isGrayChecked;
    map_filters_checked_status_["isGaussianChecked"] = &isGaussianChecked;
    map_filters_checked_status_["isCannyChecked"] = &isCannyChecked;
    map_filters_checked_status_["isROIChecked"] = &isROIChecked;
    map_filters_checked_status_["isLineOnlyChecked"] = &isLineOnlyChecked;
    map_filters_checked_status_["isSteeringChecked"] = &isSteeringChecked;
    map_filters_checked_status_["isSteeringStabilizationChecked"] = &isSteeringStabilizationChecked;
    map_filters_checked_status_["isLineOnImageChecked"] = &isLineOnImageChecked;
}

void ADASFilter::set_map_filters_checked_status(QVariantMap map) {
    QMutexLocker locker(&mutex_);

    for(int i=0;i<map.keys().length();i++) {
        *(map_filters_checked_status_[map.keys()[i]]) =  map.values()[i].toBool();
    }

    //    /*
    qDebug() << "isHorizontalFlipChecked : " << isHorizontalFlipChecked;
    qDebug() << "isVerticalFlipChecked : " << isVerticalFlipChecked;
    qDebug() << "isHSVChecked : " << isHSVChecked;
    qDebug() << "isMaskChecked : " << isMaskChecked;
    qDebug() << "isMaskColorChecked : " << isMaskColorChecked;
    qDebug() << "isMaskColorFinderChecked : " << isMaskColorFinderChecked;
    qDebug() << "isGaussianChecked : " << isGaussianChecked;
    qDebug() << "isGrayChecked : " << isGrayChecked;
    qDebug() << "isCannyChecked : " << isCannyChecked;
    qDebug() << "isROIChecked : " << isROIChecked;
    qDebug() << "isLineOnlyChecked : " << isLineOnlyChecked;
    qDebug() << "isSteeringChecked : " << isSteeringChecked;
    qDebug() << "isSteeringStabilizationChecked : " << isSteeringStabilizationChecked;
    //    */
}

void ADASFilter::set_map_points_values(QVariantMap map) {
    QMutexLocker locker(&mutex_);

    map_points_values_ = map;
}

QVariantMap ADASFilter::map_points_values() {
    return map_points_values_;
}

void ADASFilter::SetBGRMaskValues(QVariantMap map) {
    QMutexLocker locker(&mutex_);

    lower_white_ = cv::Scalar(
                map.value("valueLowerWhiteB").toInt(),
                map.value("valueLowerWhiteG").toInt(),
                map.value("valueLowerWhiteR").toInt()
                );

    upper_white_ = cv::Scalar(
                map.value("valueUpperWhiteB").toInt(),
                map.value("valueUpperWhiteG").toInt(),
                map.value("valueUpperWhiteR").toInt()
                );

    lower_yellow_ = cv::Scalar(
                map.value("valueLowerYellowB").toInt(),
                map.value("valueLowerYellowG").toInt(),
                map.value("valueLowerYellowR").toInt()
                );

    upper_yellow_ = cv::Scalar(
                map.value("valueUpperYellowB").toInt(),
                map.value("valueUpperYellowG").toInt(),
                map.value("valueUpperYellowR").toInt()
                );
}

ADASFilterRunnable::ADASFilterRunnable(ADASFilter *filter, QMutex *mutex)
    : filter_(filter),
      mutex_(mutex)
{}

QVideoFrame ADASFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags) {
    Q_UNUSED(surfaceFormat);
    Q_UNUSED(flags);

    if (!input->isValid()
            || (input->handleType() != QAbstractVideoBuffer::NoHandle && input->handleType() != QAbstractVideoBuffer::GLTextureHandle)) {
        qWarning("Invalid input format");
        return QVideoFrame();
    }

    input->map(QAbstractVideoBuffer::ReadOnly);
    if ( input->pixelFormat() == QVideoFrame::Format_YUV420P || input->pixelFormat() == QVideoFrame::Format_NV12 ) {
        is_yuv_ = true;
        mat_ = yuvFrameToMat8(*input);
    } else {
        is_yuv_ = false;
        QImage wrapper = imageWrapper(*input);
        if (wrapper.isNull()) {
            if (input->handleType() == QAbstractVideoBuffer::NoHandle)
                input->unmap();
            return *input;
        }
        mat_ = imageToMat8(wrapper);
    }
    ensureC3(&mat_);
    if (input->handleType() == QAbstractVideoBuffer::NoHandle)
        input->unmap();

}
