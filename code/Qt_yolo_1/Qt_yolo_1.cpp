#include "Qt_yolo_1.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QInputDialog>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QDir>

cv::Mat QimageToMat(const QImage& image)
{
    QImage convertedImage = image.convertToFormat(QImage::Format_RGB888);
    return cv::Mat(convertedImage.height(), convertedImage.width(), CV_8UC3, (void*)convertedImage.bits(), convertedImage.bytesPerLine()).clone();
}

Qt_yolo_1::Qt_yolo_1(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    bool runOnGPU = false; // Set to false if you want to run on CPU
    std::string modelPath = "C:/Users/mark9/Desktop/yolo11_test/model/best.onnx"; // Path to your ONNX model
    std::string classesPath = "C:/Users/mark9/Desktop/yolo11_test/classes.txt"; // Path to your classes file
    pIntf_ = new Inference(modelPath, cv::Size(960, 960), classesPath, runOnGPU);

    view = new ImageView(this);
    ui.layout_media->addWidget(view);

    videoTimer_ = new QTimer(this);
    connect(videoTimer_, &QTimer::timeout, this, &Qt_yolo_1::processVideoFrame);

    QObject::connect(ui.Button_openImage, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Images (*.png *.jpg *.bmp)"));
        if (!fileName.isEmpty()) {
            loadFile(fileName);
            prevButton_->hide();
            nextButton_->hide();
            ui.listWidget_fileList->clear();
        }
    });

    QObject::connect(ui.Button_openVideo, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Videos (*.mp4 *.avi)"));
        if (!fileName.isEmpty()) {
            loadFile(fileName);
            prevButton_->hide();
            nextButton_->hide();
            ui.listWidget_fileList->clear();
        }
    });

    QObject::connect(ui.Button_Detection, &QPushButton::clicked, this, &Qt_yolo_1::startDetection);
    QObject::connect(ui.Button_Detection_stop, &QPushButton::clicked, this, &Qt_yolo_1::stopDetection);
    QObject::connect(ui.Button_stream, &QPushButton::clicked, this, &Qt_yolo_1::openCameraStream);
    QObject::connect(ui.Button_openFolder, &QPushButton::clicked, this, &Qt_yolo_1::openFolder);
    connect(ui.listWidget_fileList, &QListWidget::itemClicked, this, &Qt_yolo_1::onFileListItemClicked);
    connect(ui.checkBox_markbox, &QCheckBox::checkStateChanged, this, &Qt_yolo_1::onDetectionSettingsChanged);
    connect(ui.checkBox_mosaic, &QCheckBox::checkStateChanged, this, &Qt_yolo_1::onDetectionSettingsChanged);


    setupFileNavigation();
}

Qt_yolo_1::~Qt_yolo_1()
{
    delete pIntf_;
    pIntf_ = nullptr;
    if (cap_) {
        cap_->release();
        delete cap_;
    }
}

void Qt_yolo_1::drawDetection(cv::Mat& frame, const Detection& detection, bool drawMarkbox, bool applyMosaic)
{
    if (applyMosaic) {
        cv::Rect box = detection.box;
        if (box.x < 0) box.x = 0;
        if (box.y < 0) box.y = 0;
        if (box.x + box.width > frame.cols) box.width = frame.cols - box.x;
        if (box.y + box.height > frame.rows) box.height = frame.rows - box.y;

        cv::Mat roi = frame(box);
        cv::Mat small_roi;
        cv::resize(roi, small_roi, cv::Size(10, 10), 0, 0, cv::INTER_NEAREST);
        cv::resize(small_roi, roi, box.size(), 0, 0, cv::INTER_NEAREST);
    }

    if (drawMarkbox) {
        // --- Auto-adjusting thickness based on image size ---
        int thickness = std::max(1, (int)(frame.cols / 720.0 * 1.5));

        // Draw the bounding box for the detection
        cv::rectangle(frame, detection.box, detection.color, thickness);

        // Create the label text (class name + confidence)
        std::string label = detection.className + " " + std::to_string(detection.confidence).substr(0, 4);

        // --- Use a fixed font size (pt) ---
        double fontScale = 0.7;

        int baseline = 0;
        cv::Size textSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, fontScale, thickness, &baseline);

        // --- Intelligent positioning for the label ---
        // 1. Default position: Above the box
        int y_baseline = detection.box.y - 10;
        
        // Check if the label background goes off the top of the image
        if (y_baseline - textSize.height - baseline < 0) {
            // 2. If so, try placing it below the box
            y_baseline = detection.box.y + detection.box.height + textSize.height + 5;

            // Check if this new position goes off the bottom
            if (y_baseline + baseline > frame.rows) {
                // 3. If it *still* goes off-screen, place it inside the top of the box
                y_baseline = detection.box.y + textSize.height + 5;
            }
        }

        // Now define the background rectangle using the final baseline 'y'
        cv::Rect labelBackground(detection.box.x, y_baseline - textSize.height - baseline, textSize.width, textSize.height + baseline + 5);

        // Constrain the background to stay within the image horizontally
        if (labelBackground.x < 0) labelBackground.x = 0;
        if (labelBackground.x + labelBackground.width > frame.cols) {
            labelBackground.x = frame.cols - labelBackground.width;
        }
        
        // Draw the background and the text
        cv::rectangle(frame, labelBackground, detection.color, cv::FILLED);
        cv::putText(frame, label, cv::Point(labelBackground.x, y_baseline), cv::FONT_HERSHEY_SIMPLEX, fontScale, cv::Scalar(0, 0, 0), thickness);
    }
}

void Qt_yolo_1::startDetection()
{
    if (isDetectionRunning_) {
        return;
    }

    if (!originalImage_.empty()) {
        currentImage_ = originalImage_.clone();
        std::vector<Detection> output = pIntf_->runInference(currentImage_);
        for (const auto& detection : output) {
            drawDetection(currentImage_, detection, ui.checkBox_markbox->isChecked(), ui.checkBox_mosaic->isChecked());
        }
        view->loadImage(currentImage_);
    } else if (!currentVideoPath_.isEmpty() || currentCameraIndex_ != -1) {
        isDetectionRunning_ = true;
        if (cap_ && !currentVideoPath_.isEmpty()) {
            cap_->set(cv::CAP_PROP_POS_FRAMES, 0);
        }
        //videoTimer_->start(1000 / ui.spinBox_detect_frame_set->value());
        videoTimer_->start(30);
    }
    updateMediaInfoLabel();
}

void Qt_yolo_1::stopDetection()
{
    if (!isDetectionRunning_) {
        return;
    }

    isDetectionRunning_ = false;
    videoTimer_->stop();
    updateMediaInfoLabel();
}

void Qt_yolo_1::processVideoFrame()
{
    cv::Mat frame;
    if (cap_ && cap_->read(frame)) {
        frameCounter_++;
        if (frameCounter_ % ui.spinBox_detect_frame_set->value() == 0) {
            std::vector<Detection> output = pIntf_->runInference(frame);
            for (const auto& detection : output) {
                drawDetection(frame, detection, ui.checkBox_markbox->isChecked(), ui.checkBox_mosaic->isChecked());
            }
            view->loadImage(frame);
        }
    } else {
        stopDetection();
    }
}

void Qt_yolo_1::updateMediaInfoLabel()
{
    QString statusText = isDetectionRunning_ ? "偵測中" : "未偵測";
    ui.label_media_info->setText(currentMediaInfo_ + "\n" + statusText);
}

void Qt_yolo_1::openCameraStream()
{
    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    QStringList cameraDescriptions;
    for (const QCameraDevice &cameraDevice : cameras) {
        cameraDescriptions << cameraDevice.description();
    }

    bool ok;
    QString item = QInputDialog::getItem(this, tr("Select Camera"),
                                         tr("Camera:"), cameraDescriptions, 0, false, &ok);
    if (ok && !item.isEmpty()) {
        int selectedCameraIndex = cameraDescriptions.indexOf(item);
        if (selectedCameraIndex != -1) {
            currentCameraIndex_ = selectedCameraIndex;
            currentVideoPath_.clear();
            currentImage_.release();
            originalImage_.release();

            if (cap_) {
                cap_->release();
                delete cap_;
            }
            cap_ = new cv::VideoCapture(currentCameraIndex_);
            cv::Mat frame;
            cap_->read(frame);
            view->loadImage(frame);

            currentMediaInfo_ = QString("相機: %1, 解析度: %2x%3")
                               .arg(item).arg(frame.cols).arg(frame.rows);
            updateMediaInfoLabel();
            prevButton_->hide();
            nextButton_->hide();
            ui.listWidget_fileList->clear();
        }
    }
}

void Qt_yolo_1::openFolder()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dirPath.isEmpty()) {
        currentFolderPath_ = dirPath;
        QDir dir(dirPath);
        QStringList filters;
        filters << "*.png" << "*.jpg" << "*.bmp" << "*.mp4" << "*.avi";
        fileList_ = dir.entryList(filters, QDir::Files);
        ui.listWidget_fileList->clear();
        ui.listWidget_fileList->addItems(fileList_);

        if (!fileList_.isEmpty()) {
            currentFileIndex_ = 0;
            loadFile(dir.filePath(fileList_.at(currentFileIndex_)));
            prevButton_->show();
            nextButton_->show();
        }
    }
}

void Qt_yolo_1::nextFile()
{
    if (currentFileIndex_ != -1 && currentFileIndex_ < fileList_.size() - 1) {
        currentFileIndex_++;
        loadFile(QDir(currentFolderPath_).filePath(fileList_.at(currentFileIndex_)));
    }
}

void Qt_yolo_1::previousFile()
{
    if (currentFileIndex_ > 0) {
        currentFileIndex_--;
        loadFile(QDir(currentFolderPath_).filePath(fileList_.at(currentFileIndex_)));
    }
}

void Qt_yolo_1::loadFile(const QString& filePath)
{
    QFileInfo fileInfo(filePath);
    QString extension = fileInfo.suffix().toLower();
    if (extension == "png" || extension == "jpg" || extension == "bmp") {
        originalImage_ = cv::imread(filePath.toLocal8Bit().constData());
        currentImage_ = originalImage_.clone();
        view->loadImage(currentImage_);
        currentVideoPath_.clear();
        currentCameraIndex_ = -1;
        currentMediaInfo_ = QString("圖片: %1, 副檔名: %2, 解析度: %3x%4, 檔案大小: %5 KB")
                           .arg(fileInfo.fileName()).arg(fileInfo.suffix()).arg(currentImage_.cols).arg(currentImage_.rows).arg(fileInfo.size() / 1024);
    } else if (extension == "mp4" || extension == "avi") {
        currentVideoPath_ = filePath;
        currentCameraIndex_ = -1;
        originalImage_.release();
        if (cap_) {
            cap_->release();
            delete cap_;
        }
        cap_ = new cv::VideoCapture(currentVideoPath_.toLocal8Bit().constData());
        cv::Mat frame;
        cap_->read(frame);
        view->loadImage(frame);
        currentImage_.release();
        double fps = cap_->get(cv::CAP_PROP_FPS);
        currentMediaInfo_ = QString("影片: %1, 副檔名: %2, 解析度: %3x%4, FPS: %5")
                           .arg(fileInfo.fileName()).arg(fileInfo.suffix()).arg((int)cap_->get(cv::CAP_PROP_FRAME_WIDTH)).arg((int)cap_->get(cv::CAP_PROP_FRAME_HEIGHT)).arg(fps);
    }
    updateMediaInfoLabel();
}

void Qt_yolo_1::setupFileNavigation()
{
    prevButton_ = new QPushButton("Previous", this);
    nextButton_ = new QPushButton("Next", this);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(prevButton_);
    layout->addWidget(nextButton_);

    ui.layout_media->addLayout(layout);

    connect(prevButton_, &QPushButton::clicked, this, &Qt_yolo_1::previousFile);
    connect(nextButton_, &QPushButton::clicked, this, &Qt_yolo_1::nextFile);

    prevButton_->hide();
    nextButton_->hide();
}
void Qt_yolo_1::onFileListItemClicked(QListWidgetItem* item)
{
    loadFile(QDir(currentFolderPath_).filePath(item->text()));
    currentFileIndex_ = fileList_.indexOf(item->text());
}

void Qt_yolo_1::onDetectionSettingsChanged()
{
    if (!originalImage_.empty())
    {
        startDetection();
    }
}
