#ifndef ADAS_FILTER_H
#define ADAS_FILTER_H

#include <QAbstractVideoFilter>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <QMutex>

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

    Q_INVOKABLE void set_map_filters_checked_status(QVariantMap map);
    Q_INVOKABLE void set_map_points_values(QVariantMap map);
    Q_INVOKABLE void map_points_values();

    Q_INVOKABLE void SetBGRMaskValues(QVariantMap map);

    QVideoFilterRunnable *createFilterRunnable() Q_DECL_OVERRIDE;

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

    int m_numLB;
    int m_numLG;
    int m_numLR;

    int m_numUB;
    int m_numUG;
    int m_numUR;

    friend class ADASFilterRunnable;
};

class ADASFilterRunnable : public QVideoFilterRunnable {
public:
    ADASFilterRunnable(ADASFilter *filter, QMutex *mutex);
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);

private:
    ADASFilter *filter_;

    QMutex *mutex_;

    cv::Mat mat_;
};

#endif // ADAS_FILTER_H
