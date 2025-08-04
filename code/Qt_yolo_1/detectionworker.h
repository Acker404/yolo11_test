#ifndef DETECTIONWORKER_H
#define DETECTIONWORKER_H

#include <QObject>
#include <QThread>
#include <QImage>
#include "inference.h"
#include <opencv2/core/mat.hpp>

class DetectionWorker : public QObject
{
    Q_OBJECT

public:
    explicit DetectionWorker(Inference* inference, QObject *parent = nullptr);
    ~DetectionWorker();

public slots:
    void processImage(const cv::Mat& image);
    void processFrame(const cv::Mat& frame);
    void stop();

signals:
    void detectionFinished(const cv::Mat& result);
    void finished();

private:
    void drawDetection(cv::Mat& frame, const Detection& detection, bool drawMarkbox, bool applyMosaic);
    Inference* pIntf_;
    bool m_stop = false;
};

#endif // DETECTIONWORKER_H
