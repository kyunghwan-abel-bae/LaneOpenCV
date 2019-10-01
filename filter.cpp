#include "filter.h"
#include "opencvhelper.h"
#include "rgbframehelper.h"

#include <QFile>

#include <QDebug>
#include <cmath>

QVideoFilterRunnable* Filter::createFilterRunnable() {
    Init();
//    return new FilterRunnable(this);
    return new FilterRunnable(this);
}

void Filter::Init() {
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

void Filter::setMapBoolForProcess(QVariantMap map) {

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

void Filter::setMapBGRListForProcess(QVariantMap map) {
//    lower_white = Scalar(map.value, map.value2. map.value3)
}

// void Filter::SetBGRMaskValues
void Filter::SetBGRMaskValues(QVariantMap map) {
//    qDebug() << "map : " << map.value("valueLowerWhiteB").toInt();
//    QVariant a;
//    a.toInt()

//    lower_white_ = cv::Scalar(200, 200, 200);
    /*
    lower_white_ = cv::Scalar(
                map.value("valueLowerWhiteB").toInt(),
                map.value("valueLowerWhiteG").toInt(),
                map.value("valueLowerWhiteR").toInt()
                );
                */

    /*
    upper_white_ = cv::Scalar(
                map.value("valueUpperWhiteB").toInt(),
                map.value("valueUpperWhiteG").toInt(),
                map.value("valueUpperWhiteR").toInt()
                );
                */

}

void Filter::set_map_points_values(QVariantMap map) {
    map_points_values_ = map;
}

QVariantMap Filter::map_points_values() {
    return map_points_values_;
}

FilterRunnable::FilterRunnable(Filter *filter)
    : filter_(filter),
      rho_(2),
      theta_(1*CV_PI/180),
      hough_threshold_(15),
      min_line_length_(10),
      max_line_gap_(20),
      trap_bottom_width_(0.85),
      trap_top_width_(0.07),
      trap_height_(0.4),
      max_angle_deviation_two_lines_(2),
      max_angle_deviation_one_line_(1),
      prev_steering_angle_(-1),
      lower_white_(cv::Scalar(255, 255, 255)),
      upper_white_(cv::Scalar(255, 255, 255)),
//      lower_white_(cv::Scalar(200, 200, 200)),
//      upper_white_(cv::Scalar(255, 255, 255)),
      lower_yellow_(cv::Scalar(89, 133, 133)),
      upper_yellow_(cv::Scalar(162, 255, 255)),
      lower_finder_(cv::Scalar(89, 133, 133)),
      upper_finder_(cv::Scalar(162, 255, 255))
{ }

cv::Mat FilterRunnable::RegionOfInterest(cv::Mat img_edges, cv::Point *points) {
    cv::Mat img_mask = cv::Mat::zeros(img_edges.rows, img_edges.cols, CV_8UC1);

    cv::Scalar ignore_mask_color = cv::Scalar(255, 255, 255);
    const cv::Point* ppt[1] = { points };
    int npt[] = { 4 };

    //filling pixels inside the polygon defined by "vertices" with the fill color
    fillPoly(img_mask, ppt, npt, 1, cv::Scalar(255, 255, 255), cv::LINE_8);

    //returning the image only where mask pixels are nonzero
    cv::Mat img_masked;
    bitwise_and(img_edges, img_mask, img_masked);

    return img_masked;
}


void FilterRunnable::FilterColors(cv::Mat _img_bgr, cv::Mat &img_filtered) {
    // Filter the image to include only yellow and white pixels
    cv::UMat img_bgr;
    cv::UMat img_empty;
    _img_bgr.copyTo(img_bgr);
    cv::UMat img_hsv, img_combine;
    cv::UMat white_mask, white_image;
    cv::UMat yellow_mask, yellow_image;
    cv::UMat finder_mask, finder_image;

    if(filter_->isHSVChecked) {
        // moved by KH
        //Filter yellow pixels( Hue 30 )
        cvtColor(img_bgr, img_hsv, cv::COLOR_BGR2HSV);

        img_hsv.copyTo(img_filtered);
    }

    if(filter_->isMaskColorFinderChecked) {
        if(img_hsv.size == img_empty.size) {
            img_bgr.copyTo(img_hsv);
        }

        lower_finder_ = cv::Scalar(filter_->m_numLB, filter_->m_numLG, filter_->m_numLR);
        upper_finder_ = cv::Scalar(filter_->m_numUB, filter_->m_numUG, filter_->m_numUR);

        inRange(img_hsv, lower_finder_, upper_finder_, finder_mask);
        bitwise_and(img_bgr, img_bgr, finder_image, finder_mask);

        finder_image.copyTo(img_filtered);
    }
    else if(filter_->isMaskColorChecked) {

        qDebug() << "filter->m_numLWR : " << filter_->m_numLWR;
        qDebug() << "filter->m_numUWR : " << filter_->m_numUWR;

        lower_white_ = cv::Scalar(filter_->m_numLWB, filter_->m_numLWG, filter_->m_numLWR);
        upper_white_ = cv::Scalar(filter_->m_numUWB, filter_->m_numUWG, filter_->m_numUWR);

//        lower_white_ = cv::Scalar(200, 200, 200);
//        upper_white_ = cv::Scalar(255, 255, 255);

        qDebug() << "lower_white_ : " << lower_white_.val[2];
        qDebug() << "upper_white_ : " << upper_white_.val[2];

        //Filter white pixels
        //inRange(img_bgr, filter->lower_white_, upper_white_, white_mask);
        inRange(img_bgr, lower_white_, upper_white_, white_mask);
        bitwise_and(img_bgr, img_bgr, white_image, white_mask);

        // basic location
        //Filter yellow pixels( Hue 30 )
        //    cvtColor(img_bgr, img_hsv, cv::COLOR_BGR2HSV);

        inRange(img_hsv, lower_yellow_, upper_yellow_, yellow_mask);
        bitwise_and(img_bgr, img_bgr, yellow_image, yellow_mask);

        //Combine the two above images
        addWeighted(white_image, 1.0, yellow_image, 1.0, 0.0, img_combine);

        img_combine.copyTo(img_filtered);
    }
}


void FilterRunnable::DrawLine(cv::Mat &img_line, vector<cv::Vec4i> lines) {
    if (lines.size() == 0) return;

    // In case of error, don't draw the line(s)
    bool draw_right = true;
    bool draw_left = true;
    int width = img_line.cols;
    int height = img_line.rows;

    //Find slopes of all lines
    //But only care about lines where abs(slope) > slope_threshold
    float slope_threshold = 0.5;
    vector<float> slopes;
    vector<cv::Vec4i> new_lines;

    for (int i = 0; i < lines.size(); i++)
    {
        cv::Vec4i line = lines[i];
        int x1 = line[0];
        int y1 = line[1];
        int x2 = line[2];
        int y2 = line[3];


        float slope;
        //Calculate slope
        if (x2 - x1 == 0) //corner case, avoiding division by 0
            slope = 999.0; //practically infinite slope
        else
            slope = (y2 - y1) / (float)(x2 - x1);


        //Filter lines based on slope
        if (abs(slope) > slope_threshold) {
            slopes.push_back(slope);
            new_lines.push_back(line);
        }
    }



    // Split lines into right_lines and left_lines, representing the right and left lane lines
    // Right / left lane lines must have positive / negative slope, and be on the right / left half of the image
    vector<cv::Vec4i> right_lines;
    vector<cv::Vec4i> left_lines;

    for (int i = 0; i < new_lines.size(); i++)
    {

        cv::Vec4i line = new_lines[i];
        float slope = slopes[i];

        int x1 = line[0];
        int y1 = line[1];
        int x2 = line[2];
        int y2 = line[3];


        float cx = width * 0.5; //x coordinate of center of image

        if (slope > 0 && x1 > cx && x2 > cx)
            right_lines.push_back(line);
        else if (slope < 0 && x1 < cx && x2 < cx)
            left_lines.push_back(line);
    }


    //Run linear regression to find best fit line for right and left lane lines
    //Right lane lines
    double right_lines_x[1000];
    double right_lines_y[1000];
    float right_m, right_b;


    int right_index = 0;
    for (int i = 0; i < right_lines.size(); i++) {

        cv::Vec4i line = right_lines[i];
        int x1 = line[0];
        int y1 = line[1];
        int x2 = line[2];
        int y2 = line[3];

        right_lines_x[right_index] = x1;
        right_lines_y[right_index] = y1;
        right_index++;
        right_lines_x[right_index] = x2;
        right_lines_y[right_index] = y2;
        right_index++;
    }


    if (right_index > 0) {

        double c0, c1, cov00, cov01, cov11, sumsq;
        gsl_fit_linear(right_lines_x, 1, right_lines_y, 1, right_index,
                       &c0, &c1, &cov00, &cov01, &cov11, &sumsq);

        //printf("# best fit: Y = %g + %g X\n", c0, c1);

        right_m = c1;
        right_b = c0;
    }
    else {
        right_m = right_b = 1;

        draw_right = false;
    }

    // Left lane lines
    double left_lines_x[1000];
    double left_lines_y[1000];
    float left_m, left_b;

    int left_index = 0;
    for (int i = 0; i < left_lines.size(); i++) {

        cv::Vec4i line = left_lines[i];
        int x1 = line[0];
        int y1 = line[1];
        int x2 = line[2];
        int y2 = line[3];

        left_lines_x[left_index] = x1;
        left_lines_y[left_index] = y1;
        left_index++;
        left_lines_x[left_index] = x2;
        left_lines_y[left_index] = y2;
        left_index++;
    }


    if (left_index > 0) {
        double c0, c1, cov00, cov01, cov11, sumsq;
        gsl_fit_linear(left_lines_x, 1, left_lines_y, 1, left_index,
                       &c0, &c1, &cov00, &cov01, &cov11, &sumsq);

        //printf("# best fit: Y = %g + %g X\n", c0, c1);

        left_m = c1;
        left_b = c0;
    }
    else {
        left_m = left_b = 1;

        draw_left = false;
    }



    //Find 2 end points for right and left lines, used for drawing the line
    //y = m*x + b--> x = (y - b) / m
    int y1 = height;
    int y2 = height * (1 - trap_height_);

    float right_x1 = (y1 - right_b) / right_m;
    float right_x2 = (y2 - right_b) / right_m;

    float left_x1 = (y1 - left_b) / left_m;
    float left_x2 = (y2 - left_b) / left_m;


    //Convert calculated end points from float to int
    y1 = int(y1);
    y2 = int(y2);
    right_x1 = int(right_x1);
    right_x2 = int(right_x2);
    left_x1 = int(left_x1);
    left_x2 = int(left_x2);


    //Draw the right and left lines on image
    if (draw_right)
        line(img_line, cv::Point(right_x1, y1), cv::Point(right_x2, y2), cv::Scalar(255, 0, 0), 10);
    if (draw_left)
        line(img_line, cv::Point(left_x1, y1), cv::Point(left_x2, y2), cv::Scalar(255, 0, 0), 10);

    if(filter_->isSteeringChecked) {
        // two detected lane lines
        float avg_lines;
        float x_offset, y_offset;
        float angle_to_mid_radian;
        float steering_angle_radian;
        int angle_to_mid_deg;
        int steering_angle;
        int head_x1, head_y1, head_x2, head_y2;
        int max_angle_deviation = max_angle_deviation_one_line_;


        if(draw_right && draw_left) {
            // y1 is the base which can tell right_x1 and left_x1 are elements for steering.
            avg_lines = (right_x1 + left_x1) / 2;
            x_offset = avg_lines - (width/2);
            y_offset = height / 2;

            angle_to_mid_radian = atanf(x_offset/y_offset);
            angle_to_mid_deg = int(angle_to_mid_radian * 180.0/3.14159);
            steering_angle = angle_to_mid_deg + 90;

            max_angle_deviation = max_angle_deviation_two_lines_;
        }

        if(filter_->isSteeringStabilizationChecked) {
            int angle_deviation = steering_angle - prev_steering_angle_;

            if(abs(angle_deviation) > max_angle_deviation)
                steering_angle = int(steering_angle + max_angle_deviation * angle_deviation / abs(angle_deviation));
        }

        steering_angle_radian = float((steering_angle/180.0) * 3.14159);
        head_x1 = int(width/2);
        head_y1 = height;
        head_x2 = int(head_x1-(height/2/tan(steering_angle_radian)));
        head_y2 = int(height/2);

        line(img_line, cv::Point(head_x1, head_y1), cv::Point(head_x2, head_y2), cv::Scalar(0, 255, 0), 10);

        int fontFace = cv::FONT_HERSHEY_PLAIN;
        double fontScale = 2;
        int thickness = 2;

        string str_text;
        if(steering_angle > 90)
            str_text = "Turn Right : " + to_string(steering_angle-90)  + " degree";
        else if(steering_angle < 90)
            str_text = "Turn Left : " + to_string(90-steering_angle)  + " degree";
        else
            str_text = "Go Straight";

        cv::putText(
                    img_line,
                    str_text,
                    cv::Point(0, height-10),
                    fontFace,
                    fontScale,
                    cv::Scalar::all(255),
                    thickness,
                    8
                    );

        prev_steering_angle_ = steering_angle;
    }
}

QVideoFrame FilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags) {
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

    last_checked_filter_ = filter_->isMaskChecked ? k_FILTER_MASK : last_checked_filter_;
    last_checked_filter_ = filter_->isHSVChecked ? k_FILTER_HSV : last_checked_filter_;
    last_checked_filter_ = filter_->isMaskColorChecked ? k_FILTER_MASK_COLOR : last_checked_filter_;
    last_checked_filter_ = filter_->isMaskColorFinderChecked ? k_FILTER_MASK_COLOR_FINDER : last_checked_filter_;
    last_checked_filter_ = filter_->isGrayChecked ? k_FILTER_GRAY : last_checked_filter_;
    last_checked_filter_ = filter_->isGaussianChecked ? k_FILTER_GAUSSIAN : last_checked_filter_;
    last_checked_filter_ = filter_->isCannyChecked ? k_FILTER_CANNY : last_checked_filter_;
    last_checked_filter_ = filter_->isROIChecked ? k_FILTER_ROI : last_checked_filter_;
    last_checked_filter_ = filter_->isLineOnlyChecked ? k_FILTER_LINE_ONLY : last_checked_filter_;
    last_checked_filter_ = filter_->isSteeringChecked ? k_FILTER_STEERING : last_checked_filter_;
    last_checked_filter_ = filter_->isSteeringStabilizationChecked ? k_FILTER_STEERING_STABILIZATION : last_checked_filter_;
    last_checked_filter_ = filter_->isLineOnImageChecked ? k_FILTER_LINE_ON_IMAGE : last_checked_filter_;

//    qDebug() << "lower_white : " << filter_->lower_white_.val[0];//lower_yellow_.val[0];
//    qDebug() << "lower_yellow : " << lower_yellow_.val[0];

    cv::Mat img_bgr, img_filtered, img_gray, img_edges, img_annotated, img_return, img_empty, img_line;
    QVariantMap *mapPointList = &(filter_->map_points_values_);

    mat_.copyTo(img_bgr);

    if(filter_->isVerticalFlipChecked)
        cv::flip(img_bgr, img_bgr, 0);

    if(filter_->isHorizontalFlipChecked)
        cv::flip(img_bgr, img_bgr, +1);

    img_bgr.copyTo(img_return);

    if((*mapPointList).keys().length() == 0) { // initialized value
        int width = img_bgr.cols;
        int height = img_bgr.rows;

        (*mapPointList)["roi_point0_x"] = (width * (1 - trap_bottom_width_)) / 2;
        (*mapPointList)["roi_point0_y"] = height;
        (*mapPointList)["roi_point1_x"] = (width * (1 - trap_top_width_)) / 2;
        (*mapPointList)["roi_point1_y"] = height - height * trap_height_;
        (*mapPointList)["roi_point2_x"] = width - (width * (1 - trap_top_width_)) / 2;
        (*mapPointList)["roi_point2_y"] = height - height * trap_height_;
        (*mapPointList)["roi_point3_x"] = width - (width * (1 - trap_bottom_width_)) / 2;
        (*mapPointList)["roi_point3_y"] = height;
    }

    if(filter_->isMaskChecked) {
        img_bgr.copyTo(img_filtered);
        FilterColors(img_bgr, img_filtered);
        img_filtered.copyTo(img_return);
    }

    if(filter_->isGrayChecked) {
        if(img_empty.size == img_filtered.size)
            img_bgr.copyTo(img_filtered);

        cv::Mat img_gray_return;

        cvtColor(img_filtered, img_gray, cv::COLOR_BGR2GRAY);

        if(last_checked_filter_ == k_FILTER_GRAY) {
            cvtColor(img_gray, img_gray_return, cv::COLOR_GRAY2BGR);
            img_gray_return.copyTo(img_return);
        }
    }

    if(filter_->isGaussianChecked) {
        cv::Mat img_gaussian_return;

        GaussianBlur(img_gray, img_gray, cv::Size(3, 3), 0, 0);

        if(last_checked_filter_ == k_FILTER_GAUSSIAN) {
            cvtColor(img_gray, img_gaussian_return, cv::COLOR_GRAY2BGR);
            img_gaussian_return.copyTo(img_return);
        }
    }

    if(filter_->isCannyChecked) {

        cv::Mat img_canny_return;

        Canny(img_gray, img_edges, 50, 150);

        if(last_checked_filter_ == k_FILTER_CANNY) {
            cvtColor(img_edges, img_canny_return, cv::COLOR_GRAY2BGR);
            img_canny_return.copyTo(img_return);
        }
    }

    if(filter_->isROIChecked) {

        if(img_empty.size == img_filtered.size) {
            cvtColor(img_bgr, img_filtered, cv::COLOR_BGR2GRAY);
        }

        if(img_empty.size == img_edges.size) {
            if(img_empty.size == img_gray.size)
                img_filtered.copyTo(img_edges);
            else
                img_gray.copyTo(img_edges);
        }

        // if media,
        cv::flip(img_edges, img_edges, 0);

        /*
        if(filter_->isVerticalFlipChecked)
            cv::flip(img_edges, img_edges, 0);

        if(filter_->isHorizontalFlipChecked)
            cv::flip(img_edges, img_edges, +1);
            */

        cv::Point points[4];

        points[0] = cv::Point((*mapPointList)["roi_point0_x"].toInt(), (*mapPointList)["roi_point0_y"].toInt());
        points[1] = cv::Point((*mapPointList)["roi_point1_x"].toInt(), (*mapPointList)["roi_point1_y"].toInt());
        points[2] = cv::Point((*mapPointList)["roi_point2_x"].toInt(), (*mapPointList)["roi_point2_y"].toInt());
        points[3] = cv::Point((*mapPointList)["roi_point3_x"].toInt(), (*mapPointList)["roi_point3_y"].toInt());

        /* codes to be removed -- checked by KH
        points[0] = cv::Point((width * (1 - trap_bottom_width_)) / 2, height);
        points[1] = cv::Point((width * (1 - trap_top_width_)) / 2, height - height * trap_height_);
        points[2] = cv::Point(width - (width * (1 - trap_top_width_)) / 2, height - height * trap_height_);
        points[3] = cv::Point(width - (width * (1 - trap_bottom_width_)) / 2, height);
        */

        img_edges = RegionOfInterest(img_edges, points);

        // belows for only return.
        if(last_checked_filter_ == k_FILTER_ROI) {
            cv::Mat img_roi_return;

            img_edges.copyTo(img_roi_return);

            cv::Point label_points[4];
            float base_scale = img_roi_return.rows * 0.02;

            string label_texts[4];

            int fontFace = cv::FONT_HERSHEY_PLAIN;
            double fontScale = 1;
            int thickness = 1;

            cvtColor(img_roi_return, img_roi_return, cv::COLOR_GRAY2BGR);

            line(img_roi_return, points[0], points[1], cv::Scalar(255, 0, 0), 3);
            line(img_roi_return, points[1], points[2], cv::Scalar(255, 0, 0), 3);
            line(img_roi_return, points[2], points[3], cv::Scalar(255, 0, 0), 3);
            line(img_roi_return, points[3], points[0], cv::Scalar(255, 0, 0), 3);

            for(int i=0;i<4;i++) {
                circle(img_roi_return, points[i], base_scale, cv::Scalar(255, 0, 0), -1);

                label_points[i].x = points[i].x;
                label_points[i].y = points[i].y - base_scale - 10;

                label_texts[i] = "(" + to_string(points[i].x) + ", " + to_string(points[i].y) + ")";

                cv::putText(
                            img_roi_return,
                            label_texts[i],
                            label_points[i],
                            fontFace,
                            fontScale,
                            cv::Scalar::all(255),
                            thickness,
                            8
                            );
            }


            cv::flip(img_roi_return, img_roi_return, 0);
            /*
            if(filter_->isVerticalFlipChecked)
                cv::flip(img_roi_return, img_roi_return, 0);

            if(filter_->isHorizontalFlipChecked)
                cv::flip(img_roi_return, img_roi_return, +1);
                */

            img_roi_return.copyTo(img_return);
        }
    }

    if(filter_->isLineOnlyChecked) {
        cv::UMat uImage_edges;
        img_edges.copyTo(uImage_edges);

        vector<cv::Vec4i> lines;
        HoughLinesP(uImage_edges, lines, rho_, theta_, hough_threshold_, min_line_length_, max_line_gap_);

        img_line = cv::Mat::zeros(img_bgr.rows, img_bgr.cols, CV_8UC3);
        DrawLine(img_line, lines);

        img_line.copyTo(img_return);
        //        cv::flip(img_return, img_return, 0);
        if(last_checked_filter_ == k_FILTER_LINE_ONLY
                || last_checked_filter_ == k_FILTER_STEERING
                || last_checked_filter_ == k_FILTER_STEERING_STABILIZATION) {
            cv::flip(img_return, img_return, 0);
        }
    }

    if(filter_->isLineOnImageChecked) {

        if(img_line.size != img_empty.size) {
            // because this is last one.
            cv::flip(img_line, img_line, 0);

            addWeighted(img_bgr, 0.8, img_line, 1.0, 0.0, img_annotated);
            img_annotated.copyTo(img_return);
        }
    }

    if(0) {
        //    if(test_count_++ == 100) {
        cv::imshow("POP", img_return);
        //        QLabel *label = new QLabel();//filter_->parent());
        //        label->setPixmap(QPixmap::fromImage(mat8ToImage(img_return)));
        //        label->show();
    }

    //    qDebug() << "is Gray : " << mat8ToImage(img_return).isGrayscale();


    return QVideoFrame(mat8ToImage(img_return));
}
