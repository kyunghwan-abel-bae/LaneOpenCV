#ifndef ADAS_FILTER_H
#define ADAS_FILTER_H

#include <QAbstractVideoFilter>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <QMutex>

#include <QDebug>

#include <gsl/gsl_fit.h>

using namespace std;

class ADASFilter : public QAbstractVideoFilter
{
    Q_OBJECT
public:
    Q_PROPERTY (int numLR READ numLR WRITE set_numLR)
    Q_PROPERTY (int numLG READ numLG WRITE set_numLG)
    Q_PROPERTY (int numLB READ numLB WRITE set_numLB)
    Q_PROPERTY (int numUR READ numUR WRITE set_numUR)
    Q_PROPERTY (int numUG READ numUG WRITE set_numUG)
    Q_PROPERTY (int numUB READ numUB WRITE set_numUB)

    Q_INVOKABLE void set_is_camera_on(bool is_on);
    Q_INVOKABLE void set_map_filters_checked_status(QVariantMap map);
    Q_INVOKABLE void set_map_points_values(QVariantMap map);
    Q_INVOKABLE QVariantMap map_points_values();

    Q_INVOKABLE void SetBGRMaskValues(QVariantMap map);

    QVideoFilterRunnable* createFilterRunnable() Q_DECL_OVERRIDE;

    void init_map_points_values(cv::Mat &init_src);

    int numLR() { return m_numLR; }
    int numLG() { return m_numLG; }
    int numLB() { return m_numLB; }

    int numUR() { return m_numUR; }
    int numUG() { return m_numUG; }
    int numUB() { return m_numUB; }

    void set_numLR(const int& numLR) { m_numLR = numLR; }
    void set_numLG(const int& numLG) { m_numLG = numLG; }
    void set_numLB(const int& numLB) { m_numLB = numLB; }

    void set_numUR(const int& numUR) { m_numUR = numUR; }
    void set_numUG(const int& numUG) { m_numUG = numUG; }
    void set_numUB(const int& numUB) { m_numUB = numUB; }

signals:
    void finished(QObject *e);

private:
    void Init();

    QMutex mutex_;

    cv::Scalar lower_white_, upper_white_;
    cv::Scalar lower_yellow_, upper_yellow_;

    // qml interface values
    map<QString, bool*> map_filters_checked_status_;

    QVariantMap map_points_values_;

    bool isHorizontalFlipChecked;
    bool isVerticalFlipChecked;
    bool isHSVChecked;
    bool isMaskChecked;
    bool isMaskColorChecked;
    bool isMaskColorFinderChecked;
    bool isGrayChecked;
    bool isGaussianChecked;
    bool isCannyChecked;
    bool isROIChecked;
    bool isLineOnlyChecked;
    bool isSteeringChecked;
    bool isSteeringStabilizationChecked;
    bool isLineOnImageChecked;

    bool is_camera_on_;

    int m_numLB;
    int m_numLG;
    int m_numLR;

    int m_numUB;
    int m_numUG;
    int m_numUR;

    float trap_bottom_width_;
    float trap_top_width_;
    float trap_height_;

    friend class ADASFilterRunnable;
};

class ADASFoundLanesResult {
public:
    cv::Point get_right_point1() { return cv::Point(right_x1_, y1_); }
    cv::Point get_right_point2() { return cv::Point(right_x2_, y2_); }
    cv::Point get_left_point1() { return cv::Point(left_x1_, y1_); }
    cv::Point get_left_point2() { return cv::Point(left_x2_, y2_); }

    bool is_right_lane_existed_;
    bool is_left_lane_existed_;

    int right_x1_, right_x2_;
    int left_x1_, left_x2_;
    int y1_, y2_;

    int bgr_width_, bgr_height_;
};

class ADASFilterRunnable : public QVideoFilterRunnable {
public:
    ADASFilterRunnable(ADASFilter *filter, QMutex *mutex);
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);

private:
    cv::Mat ProcessADAS(cv::Mat &src);
    cv::Mat RegionOfInterest(cv::Mat img_edges, cv::Point *points);
    void ShowROIInformation(cv::Mat &src, cv::Point *points);
    void FilterMaskColorsFinder(cv::Mat _img_bgr, cv::Mat &img_hsv, cv::Mat &img_filtered);
    void FilterMaskColors(cv::Mat _img_bgr, cv::Mat &img_hsv, cv::Mat &img_filtered);

    void DrawLine(cv::Mat &img_line, vector<cv::Vec4i> lines);
    void ExtractLanes(ADASFoundLanesResult &result, cv::Mat &img_line, vector<cv::Vec4i> lines);
    int CalculateSteeringAngle(ADASFoundLanesResult &result, bool stabilization_option_checked);

    ADASFilter *filter_;

    QMutex *mutex_;

    cv::Mat mat_;

    cv::Scalar lower_finder_;
    cv::Scalar upper_finder_;

    bool is_yuv_;
    bool show_roi_info_;

    float rho_;
    float theta_;
    float hough_threshold_;
    float min_line_length_;
    float max_line_gap_;

    int max_angle_deviation_two_lines_;
    int max_angle_deviation_one_line_;

    int steering_angle_;
    int prev_steering_angle_;
};


#endif // ADAS_FILTER_H
