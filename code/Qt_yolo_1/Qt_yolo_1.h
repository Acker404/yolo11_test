#pragma once

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include <QPushButton>
#include <QStringList>
#include <QListWidget>
#include <QTextStream>
#include "ui_Qt_yolo_1.h"
#include "inference.h"
#include "imageview.h"
#include <opencv2/videoio.hpp>
#include <fstream>

// Defines the options for exporting processed data.
struct OutputOptions {
    bool saveLabel; // Save YOLO format label file.
    bool saveImage; // Save image with detections drawn on it.
    bool saveCSV;   // Save a CSV summary for batch processing.
};

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
    void onDetectionSettingsChanged();
    void handleExportPathClick();
    void handleExportClick();
    void handleExportset();

private:
    void updateMediaInfoLabel();
    void loadFile(const QString& filePath);
    void setupFileNavigation();
    void drawDetection(cv::Mat& frame, const Detection& detection, bool drawMarkbox, bool applyMosaic);

    // Helper functions for exporting
    void saveProcessedImage(const QString& path, const cv::Mat& image, const std::vector<Detection>& detections, bool mosaic, bool markbox);
    void saveYoloLabels(const QString& path, const std::vector<Detection>& detections, int imgWidth, int imgHeight);
    void appendCSVRow(QTextStream& stream, const QString& fileName, const std::vector<Detection>& detections);


    Ui::Qt_yolo_1Class ui;
    ImageView* view = nullptr;
    Inference* pIntf_ {nullptr};

    cv::Mat currentImage_;
    cv::Mat originalImage_;

    QString currentVideoPath_;
    QString exportPath;
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

    // Export settings
    OutputOptions exportOptions_ = { true, true, false }; // Default values
};
