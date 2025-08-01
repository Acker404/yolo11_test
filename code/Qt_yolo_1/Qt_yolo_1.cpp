#include "Qt_yolo_1.h"
#include <QFileDialog>
#include <QFileInfo>

// Helper function to draw detections with auto-sizing and positioning
void drawDetection(cv::Mat& frame, const Detection& detection)
{
    // --- Auto-adjusting thickness based on image size ---
    int thickness = std::max(1, (int)(frame.cols / 720.0 * 1.5));

    // Draw the bounding box for the detection
    cv::rectangle(frame, detection.box, detection.color, thickness);

    // Create the label text (class name + confidence)
    std::string label = detection.className + " " + std::to_string(detection.confidence).substr(0, 4);

    // --- Use a fixed font size (pt) ---
   // double fontScale = 0.7; // This value can be tuned to look like 20pt
    int targetHeight = 20;
    double fontScale = 0.1;
    int baseline = 0;
    cv::Size textSize;
    while (true) {
        cv::Size textSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, fontScale, 1, &baseline);
        if (textSize.height >= targetHeight)
            break;
        fontScale += 0.1;
    }

    //int baseline = 0;
    //cv::Size textSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, fontScale, thickness, &baseline);

    // --- Intelligent positioning for the label ---
    cv::Point labelOrigin = cv::Point(detection.box.x, detection.box.y - 10);
    if (labelOrigin.y - textSize.height < 0) {
        labelOrigin.y = detection.box.y + textSize.height + 5;
    }

    cv::Rect labelBackground(labelOrigin.x, labelOrigin.y - textSize.height - baseline, textSize.width, textSize.height + baseline + 5);
    // Constrain the background to stay within the image
    if (labelBackground.x < 0) labelBackground.x = 0;
    if (labelBackground.y < 0) labelBackground.y = 0;
    if (labelBackground.x + labelBackground.width > frame.cols) {
        labelBackground.x = frame.cols - labelBackground.width;
    }
    if (labelBackground.y + labelBackground.height > frame.rows) {
        labelBackground.y = frame.rows - labelBackground.height;
    }
    
    cv::rectangle(frame, labelBackground, detection.color, cv::FILLED);
    cv::putText(frame, label, cv::Point(labelBackground.x, labelBackground.y + textSize.height), cv::FONT_HERSHEY_SIMPLEX, fontScale, cv::Scalar(0, 0, 0), thickness);
}

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
            currentImage_ = cv::imread(fileName.toLocal8Bit().constData());
            view->loadImage(currentImage_);
            currentVideoPath_.clear();

            QFileInfo info(fileName);
            currentMediaInfo_ = QString("圖片: %1, 副檔名: %2, 解析度: %3x%4, 檔案大小: %5 KB")
                               .arg(info.fileName()).arg(info.suffix()).arg(currentImage_.cols).arg(currentImage_.rows).arg(info.size() / 1024);
            updateMediaInfoLabel();
        }
    });

    QObject::connect(ui.Button_openVideo, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Videos (*.mp4 *.avi)"));
        if (!fileName.isEmpty()) {
            currentVideoPath_ = fileName;
            if (cap_) {
                cap_->release();
                delete cap_;
            }
            cap_ = new cv::VideoCapture(currentVideoPath_.toLocal8Bit().constData());
            cv::Mat frame;
            cap_->read(frame);
            view->loadImage(frame);
            currentImage_.release();

            QFileInfo info(fileName);
            double fps = cap_->get(cv::CAP_PROP_FPS);
            currentMediaInfo_ = QString("影片: %1, 副檔名: %2, 解析度: %3x%4, FPS: %5")
                               .arg(info.fileName()).arg(info.suffix()).arg((int)cap_->get(cv::CAP_PROP_FRAME_WIDTH)).arg((int)cap_->get(cv::CAP_PROP_FRAME_HEIGHT)).arg(fps);
            updateMediaInfoLabel();
        }
    });

    QObject::connect(ui.Button_Detection, &QPushButton::clicked, this, &Qt_yolo_1::startDetection);
    QObject::connect(ui.Button_Detection_stop, &QPushButton::clicked, this, &Qt_yolo_1::stopDetection);
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

void Qt_yolo_1::startDetection()
{
    if (isDetectionRunning_) {
        return;
    }

    if (!currentImage_.empty()) {
        std::vector<Detection> output = pIntf_->runInference(currentImage_);
        for (const auto& detection : output) {
            drawDetection(currentImage_, detection);
        }
        view->loadImage(currentImage_);
    } else if (!currentVideoPath_.isEmpty()) {
        isDetectionRunning_ = true;
        if (cap_) {
            cap_->set(cv::CAP_PROP_POS_FRAMES, 0);
        }
        videoTimer_->start(1000 / ui.spinBox_detect_frame_set->value());
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
                drawDetection(frame, detection);
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