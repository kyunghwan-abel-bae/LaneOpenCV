#include "adas_filter.h"
#include "opencvhelper.h"
#include "rgbframehelper.h"

#include <QFile>

#include <cmath>

QVideoFilterRunnable* ADASFilter::createFilterRunnable() {
    Init();
    return new ADASFilterRunnable(this, &mutex_);
}

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

    trap_bottom_width_ = 0.85;
    trap_top_width_ = 0.07;
    trap_height_ = 0.4;
}

void ADASFilter::set_is_camera_on(bool is_on) {
    is_camera_on_ = is_on;
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

void ADASFilter::init_map_points_values(cv::Mat &init_src) {
    int width = init_src.cols;
    int height = init_src.rows;

    map_points_values_["roi_point0_x"] = (width * (1 - trap_bottom_width_)) / 2;
    map_points_values_["roi_point0_y"] = height;
    map_points_values_["roi_point1_x"] = (width * (1 - trap_top_width_)) / 2;
    map_points_values_["roi_point1_y"] = height - height * trap_height_;
    map_points_values_["roi_point2_x"] = width - (width * (1 - trap_top_width_)) / 2;
    map_points_values_["roi_point2_y"] = height - height * trap_height_;
    map_points_values_["roi_point3_x"] = width - (width * (1 - trap_bottom_width_)) / 2;
    map_points_values_["roi_point3_y"] = height;
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
      mutex_(mutex),
      show_roi_info_(false),
      rho_(2),
      theta_(1*CV_PI/180),
      hough_threshold_(15),
      min_line_length_(10),
      max_line_gap_(20),
      max_angle_deviation_two_lines_(2),
      max_angle_deviation_one_line_(1),
      prev_steering_angle_(-1)
{}

void ADASFilterRunnable::FilterMaskColorsFinder(cv::Mat _img_bgr, cv::Mat &img_hsv, cv::Mat &img_filtered) {
    cv::Mat img_empty, img_result;
    cv::Mat finder_mask;

    if((_img_bgr.size == img_empty.size)
            || (img_hsv.size == img_empty.size))
        return;

    lower_finder_ = cv::Scalar(filter_->m_numLB, filter_->m_numLG, filter_->m_numLR);
    upper_finder_ = cv::Scalar(filter_->m_numUB, filter_->m_numUG, filter_->m_numUR);

    inRange(img_hsv, lower_finder_, upper_finder_, finder_mask);
    bitwise_and(_img_bgr, _img_bgr, img_result, finder_mask);

    img_result.copyTo(img_filtered);
}

void ADASFilterRunnable::FilterMaskColors(cv::Mat _img_bgr, cv::Mat &img_hsv, cv::Mat &img_filtered) {
    cv::Mat img_empty, img_result;
    cv::Mat white_image, yellow_image;
    cv::Mat white_mask, yellow_mask;

    if((_img_bgr.size == img_empty.size)
            || (img_hsv.size == img_empty.size))
        return;

    inRange(_img_bgr, filter_->lower_white_, filter_->upper_white_, white_mask);
    bitwise_and(_img_bgr, _img_bgr, white_image, white_mask);

    inRange(img_hsv, filter_->lower_yellow_, filter_->upper_yellow_, yellow_mask);
    bitwise_and(_img_bgr, _img_bgr, yellow_image, yellow_mask);

    addWeighted(white_image, 1.0, yellow_image, 1.0, 0.0, img_result);

    img_result.copyTo(img_filtered);
}

void ADASFilterRunnable::ShowROIInformation(cv::Mat &src, cv::Point *points) {
    cv::Point label_points[4];
    float base_scale = src.rows * 0.02;

    string label_texts[4];

    int fontFace = cv::FONT_HERSHEY_PLAIN;
    double fontScale = src.rows * 0.0014;
    int thickness = 1;

    if(src.channels() == 1)
        cvtColor(src, src, cv::COLOR_GRAY2BGR);

    line(src, points[0], points[1], cv::Scalar(255, 0, 0), 3);
    line(src, points[1], points[2], cv::Scalar(255, 0, 0), 3);
    line(src, points[2], points[3], cv::Scalar(255, 0, 0), 3);
    line(src, points[3], points[0], cv::Scalar(255, 0, 0), 3);

    for(int i=0;i<4;i++) {
        circle(src, points[i], base_scale, cv::Scalar(255, 0, 0), -1);

        label_points[i].x = points[i].x;
        label_points[i].y = points[i].y - base_scale - 10;

        label_texts[i] = "(" + to_string(points[i].x) + ", " + to_string(points[i].y) + ")";

        cv::putText(
                    src,
                    label_texts[i],
                    label_points[i],
                    fontFace,
                    fontScale,
                    cv::Scalar::all(255),
                    thickness,
                    8
                    );
    }
}

cv::Mat ADASFilterRunnable::RegionOfInterest(cv::Mat img_edges, cv::Point *points) {
    cv::Mat img_mask;

    if(img_edges.channels() == 1) {
        img_mask = cv::Mat::zeros(img_edges.rows, img_edges.cols, CV_8UC1);
    }
    else if(img_edges.channels() == 3){
        img_mask = cv::Mat::zeros(img_edges.rows, img_edges.cols, CV_8UC3);
    }
    else
        return img_edges;

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

void ADASFilterRunnable::ExtractLanes(ADASFoundLanesResult &result, cv::Mat &img_line, vector<cv::Vec4i> lines) {
    if (lines.size() == 0) return;

    // In case of error, don't draw the line(s)
    bool is_right_lane_existed = true;
    bool is_left_lane_existed = true;
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

        is_right_lane_existed = false;
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

        is_left_lane_existed = false;
    }

    //Find 2 end points for right and left lines, used for drawing the line
    //y = m*x + b--> x = (y - b) / m
    int y1 = height;
    int y2 = height * (1 - filter_->trap_height_);

    float right_x1 = (y1 - right_b) / right_m;
    float right_x2 = (y2 - right_b) / right_m;

    float left_x1 = (y1 - left_b) / left_m;
    float left_x2 = (y2 - left_b) / left_m;

    //Convert calculated end points from float to int
    result.is_left_lane_existed_ = is_left_lane_existed;
    result.is_right_lane_existed_ = is_right_lane_existed;
    result.y1_ = int(y1);
    result.y2_ = int(y2);
    result.right_x1_ = int(right_x1);
    result.right_x2_ = int(right_x2);
    result.left_x1_ = int(left_x1);
    result.left_x2_ = int(left_x2);
    result.bgr_width_ = width;
    result.bgr_height_ = height;
}

int ADASFilterRunnable::CalculateSteeringAngle(ADASFoundLanesResult &result, bool stabilization_option_checked=false) {
    // two detected lane lines
    float avg_lines;
    float x_offset, y_offset;
    float angle_to_mid_radian;
    int angle_to_mid_deg;
    int steering_angle;
    int bgr_width = result.bgr_width_, bgr_height = result.bgr_height_;
    int max_angle_deviation = max_angle_deviation_one_line_;

    if(result.is_left_lane_existed_
            && result.is_right_lane_existed_) {
        // y1 is the base which can tell right_x1 and left_x1 are elements for steering.
        avg_lines = (result.right_x1_ + result.left_x1_) / 2;
        x_offset = avg_lines - (bgr_width/2);
        y_offset = bgr_height / 2;

        angle_to_mid_radian = atanf(x_offset/y_offset);
        angle_to_mid_deg = int(angle_to_mid_radian * 180.0/3.14159);
        steering_angle = angle_to_mid_deg + 90;

        max_angle_deviation = max_angle_deviation_two_lines_;
    }

    if(stabilization_option_checked) {
        int angle_deviation = steering_angle - prev_steering_angle_;

        if(abs(angle_deviation) > max_angle_deviation)
            steering_angle = int(steering_angle + max_angle_deviation * angle_deviation / abs(angle_deviation));
    }

    steering_angle_ = prev_steering_angle_ = steering_angle;

    return steering_angle_;
}


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

    if (filter_->map_points_values_.keys().length() == 0)
        filter_->init_map_points_values(mat_);

    if(filter_->isVerticalFlipChecked)
        cv::flip(mat_, mat_, 0);

    if(filter_->isHorizontalFlipChecked)
        cv::flip(mat_, mat_, +1);

    return QVideoFrame(mat8ToImage(ProcessADAS(mat_)));
}


cv::Mat ADASFilterRunnable::ProcessADAS(cv::Mat &src) {
    cv::Mat img_filtered, img_roi;
    bool is_flipped = false;

    src.copyTo(img_filtered);

    int pre_condition = 0;
    if(filter_->isHSVChecked) {
        cvtColor(img_filtered, img_filtered, cv::COLOR_BGR2HSV);

        pre_condition++;

        if(filter_->isMaskColorFinderChecked) {
            FilterMaskColorsFinder(src, img_filtered, img_filtered);

            pre_condition++;
        }
        else if(filter_->isMaskColorChecked) {
            FilterMaskColors(src, img_filtered, img_filtered);

            pre_condition++;
        }
    }

    if(filter_->isGrayChecked) {
        cvtColor(img_filtered, img_filtered, cv::COLOR_BGR2GRAY);

        pre_condition++;

        if(filter_->isCannyChecked) {
            Canny(img_filtered, img_filtered, 50, 150);

            pre_condition++;
        }
    }

    if(filter_->isGaussianChecked) {
        GaussianBlur(img_filtered, img_filtered, cv::Size(3,3), 0, 0);

        pre_condition++;
    }

    if(filter_->isROIChecked) {

        if(!filter_->is_camera_on_) {
            cv::flip(img_filtered, img_filtered, 0);
            is_flipped = !is_flipped;
        }

        QVariantMap *map_point_list = &(filter_->map_points_values_);

        cv::Point points[4];

        points[0] = cv::Point((*map_point_list)["roi_point0_x"].toInt(), (*map_point_list)["roi_point0_y"].toInt());
        points[1] = cv::Point((*map_point_list)["roi_point1_x"].toInt(), (*map_point_list)["roi_point1_y"].toInt());
        points[2] = cv::Point((*map_point_list)["roi_point2_x"].toInt(), (*map_point_list)["roi_point2_y"].toInt());
        points[3] = cv::Point((*map_point_list)["roi_point3_x"].toInt(), (*map_point_list)["roi_point3_y"].toInt());

        img_filtered = RegionOfInterest(img_filtered, points);
        img_filtered.copyTo(img_roi);

        if(show_roi_info_) { // ROI is lastly checked.
            ShowROIInformation(img_filtered, points);

            if(is_flipped) {
                cv::flip(img_filtered, img_filtered, 0);
                is_flipped = !is_flipped;
            }
        }

        pre_condition++;
    }

    show_roi_info_ = true;

    if(filter_->isLineOnlyChecked
            && pre_condition > 5) { // 6 : HSV, MASK(COLOR OR FINDER), GRAY, GAUSSIAN, CANNY, ROI

        show_roi_info_ = false;

        cv::Mat uImage_edges, img_line;
        img_roi.copyTo(uImage_edges);

        vector<cv::Vec4i> lines;
        HoughLinesP(uImage_edges, lines, rho_, theta_, hough_threshold_, min_line_length_, max_line_gap_);

        img_line = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);
        ADASFoundLanesResult afl_result;

        ExtractLanes(afl_result, img_line, lines);

        if(afl_result.is_left_lane_existed_)
            line(img_line, afl_result.get_left_point1(), afl_result.get_left_point2(), cv::Scalar(255, 0, 0), 10);
        if(afl_result.is_right_lane_existed_)
            line(img_line, afl_result.get_right_point1(), afl_result.get_right_point2(), cv::Scalar(255, 0, 0), 10);

        img_line.copyTo(img_filtered);

        if(filter_->isSteeringChecked) {

            float steering_angle_radian;
            int head_x1, head_y1, head_x2, head_y2;
            int steering_angle;

            steering_angle = CalculateSteeringAngle(afl_result, filter_->isSteeringStabilizationChecked);

            steering_angle_radian = float((steering_angle/180.0) * 3.14159);
            head_x1 = int(afl_result.bgr_width_/2);
            head_y1 = afl_result.bgr_height_;
            head_x2 = int(head_x1-(afl_result.bgr_height_/2/tan(steering_angle_radian)));
            head_y2 = int(afl_result.bgr_height_/2);

            line(img_filtered, cv::Point(head_x1, head_y1), cv::Point(head_x2, head_y2), cv::Scalar(0, 255, 0), 10);

            // show info
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
                        img_filtered,
                        str_text,
                        cv::Point(0, afl_result.bgr_height_-10),
                        fontFace,
                        fontScale,
                        cv::Scalar::all(255),
                        thickness,
                        8
                        );

            if(filter_->isLineOnImageChecked) {
                if(is_flipped) {
                    cv::flip(img_filtered, img_filtered, 0);
                    is_flipped = !is_flipped;
                }

                cv::Mat img_annotated;
                addWeighted(src, 0.8, img_filtered, 1.0, 0.0, img_annotated);
                img_annotated.copyTo(img_filtered);
            }
        }

        if(is_flipped)
            cv::flip(img_filtered, img_filtered, 0);
    }

    if(img_filtered.channels() == 1)
        cvtColor(img_filtered, img_filtered, cv::COLOR_GRAY2BGR);

    return img_filtered;
}
