#ifndef FILTER_H
#define FILTER_H

#include <QAbstractVideoFilter>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <gsl/gsl_fit.h>

using namespace std;

class Filter : public QAbstractVideoFilter
{
    Q_OBJECT
public:
    QVideoFilterRunnable *createFilterRunnable() Q_DECL_OVERRIDE;

    Q_INVOKABLE void setMapBoolForProcess(QVariantMap map);
    Q_INVOKABLE void setMapBGRListForProcess(QVariantMap map);

    Q_INVOKABLE void set_map_points_values(QVariantMap map);
    Q_INVOKABLE QVariantMap map_points_values();

    Q_PROPERTY (int numLR
                READ numLR
                WRITE set_numLR)

    Q_PROPERTY (int numLG
                READ numLG
                WRITE set_numLG)

    Q_PROPERTY (int numLB
                READ numLB
                WRITE set_numLB)
    Q_PROPERTY (int numUR
                READ numUR
                WRITE set_numUR)

    Q_PROPERTY (int numUG
                READ numUG
                WRITE set_numUG)

     Q_PROPERTY (int numUB
                READ numUB
                WRITE set_numUB)

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

    int m_numLB;
    int m_numLG;
    int m_numLR;

    int m_numUB;
    int m_numUG;
    int m_numUR;

    int m_numLWB;
    int m_numLWG;
    int m_numLWR;

    int m_numUWB;
    int m_numUWG;
    int m_numUWR;

    int m_numLYB;
    int m_numLYG;
    int m_numLYR;

    int m_numUYB;
    int m_numUYG;
    int m_numUYR;

    QStringList arrBGRLowerWhite;
    QStringList arrBGRUpperWhite;
    QStringList arrBGRLowerYellow;
    QStringList arrBGRUpperYellow;

    QStringList arrBGRFirstMask;
    QStringList arrBGRSecondMask;

    friend class FilterRunnable;
};

class FilterRunnable : public QVideoFilterRunnable {
public:
    FilterRunnable(Filter *filter);
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags) Q_DECL_OVERRIDE;

private:
    Filter *filter_;

    cv::Mat mat_;

    cv::Scalar lower_white_;
    cv::Scalar upper_white_;
    cv::Scalar lower_yellow_;
    cv::Scalar upper_yellow_;
    cv::Scalar lower_finder_;
    cv::Scalar upper_finder_;

    bool is_yuv_;

    float rho_;
    float theta_;
    float hough_threshold_;
    float min_line_length_;
    float max_line_gap_;

    float trap_bottom_width_;
    float trap_top_width_;
    float trap_height_;

    int max_angle_deviation_two_lines_;
    int max_angle_deviation_one_line_;

    int prev_steering_angle_;

    int test_count_;

    int last_checked_filter_;
    enum {
            k_FILTER_HSV = 0,
            k_FILTER_MASK = 1,
            k_FILTER_MASK_COLOR = 2,
            k_FILTER_MASK_COLOR_FINDER = 3,
            k_FILTER_GRAY = 4,
            k_FILTER_GAUSSIAN = 5,
            k_FILTER_CANNY = 6,
            k_FILTER_ROI = 7,
            k_FILTER_LINE_ONLY = 8,
            k_FILTER_LINE_ON_IMAGE = 9,
            k_FILTER_STEERING = 10,
            k_FILTER_STEERING_STABILIZATION = 11,
    };

    cv::Mat RegionOfInterest(cv::Mat img_edges, cv::Point *points);
    void FilterColors(cv::Mat _img_bgr, cv::Mat &img_filtered);
    void DrawLine(cv::Mat &img_line, vector<cv::Vec4i> lines);
};

class FilterResult : public QObject {
    Q_OBJECT
public slots:
    QVariantList rects() const { return rects_; }

private:
    QVariantList rects_;
    friend class FilterRunnable;
};

#endif // FILTER_H
