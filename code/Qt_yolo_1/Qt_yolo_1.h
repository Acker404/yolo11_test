#pragma once

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include <QPushButton>
#include <QStringList>
#include <QListWidget>
#include "ui_Qt_yolo_1.h"
#include "inference.h"
#include "imageview.h"
#include <opencv2/videoio.hpp>
#include <fstream>


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
    void openCameraStream();
    void openFolder();
    void nextFile();
    void previousFile();
    void onFileListItemClicked(QListWidgetItem* item);

private:
    void updateMediaInfoLabel();
    void loadFile(const QString& filePath);
    void setupFileNavigation();

    Ui::Qt_yolo_1Class ui;
    ImageView* view = nullptr;
    Inference* pIntf_ {nullptr};

    cv::Mat currentImage_;
    QString currentVideoPath_;
    int currentCameraIndex_ = -1;
    QString currentMediaInfo_;
    bool isDetectionRunning_ = false;
    QTimer* videoTimer_ = nullptr;
    cv::VideoCapture* cap_ = nullptr;
    int frameCounter_ = 0;

    // Folder navigation
    QString currentFolderPath_;
    QStringList fileList_;
    int currentFileIndex_ = -1;
    QPushButton* prevButton_ = nullptr;
    QPushButton* nextButton_ = nullptr;
};