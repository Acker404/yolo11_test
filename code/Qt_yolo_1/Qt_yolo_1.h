#pragma once

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include "ui_Qt_yolo_1.h"
#include "inference.h"
#include "imageview.h"

class Qt_yolo_1 : public QMainWindow
{
    Q_OBJECT

public:
    Qt_yolo_1(QWidget *parent = nullptr);
    ~Qt_yolo_1();

private slots:
    void startDetection();
    void stopDetection();
    void processVideoFrame();

private:
    Ui::Qt_yolo_1Class ui;
    ImageView* view = nullptr;
    Inference* pIntf_ {nullptr};

    cv::Mat currentImage_;
    QString currentVideoPath_;
    bool isDetectionRunning_ = false;
    QTimer* videoTimer_ = nullptr;
};