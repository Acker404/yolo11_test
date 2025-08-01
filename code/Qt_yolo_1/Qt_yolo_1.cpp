#include "Qt_yolo_1.h"
#include <QFileDialog>

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
            currentImage_ = cv::imread(fileName.toStdString());
            view->loadImage(currentImage_);
            currentVideoPath_.clear();
        }
    });

    QObject::connect(ui.Button_openVideo, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Videos (*.mp4 *.avi)"));
        if (!fileName.isEmpty()) {
            currentVideoPath_ = fileName;
            view->loadVideo(currentVideoPath_);
            currentImage_.release();
        }
    });

    QObject::connect(ui.Button_Detection, &QPushButton::clicked, this, &Qt_yolo_1::startDetection);
    QObject::connect(ui.Button_Detection_stop, &QPushButton::clicked, this, &Qt_yolo_1::stopDetection);
}

Qt_yolo_1::~Qt_yolo_1()
{
    delete pIntf_;
    pIntf_ = nullptr;
}

void Qt_yolo_1::startDetection()
{
    if (isDetectionRunning_) {
        return;
    }

    if (!currentImage_.empty()) {
        std::vector<Detection> output = pIntf_->runInference(currentImage_);
        int detections = output.size();
        std::cout << "Number of detections:" << detections << std::endl;

        for (int i = 0; i < detections; ++i)
        {
            Detection detection = output[i];
            cv::Rect box = detection.box;
            cv::Scalar color = detection.color;
            cv::rectangle(currentImage_, box, color, 2);
            std::string classString = detection.className + " " + std::to_string(detection.confidence).substr(0, 4);
            cv::putText(currentImage_, classString, cv::Point(box.x, box.y - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
        }
        view->loadImage(currentImage_);
    } else if (!currentVideoPath_.isEmpty()) {
        isDetectionRunning_ = true;
        videoTimer_->start(33); // ~30 FPS
    }
}

void Qt_yolo_1::stopDetection()
{
    if (!isDetectionRunning_) {
        return;
    }

    isDetectionRunning_ = false;
    videoTimer_->stop();
}

void Qt_yolo_1::processVideoFrame()
{
    static cv::VideoCapture cap(currentVideoPath_.toStdString());
    cv::Mat frame;
    if (cap.read(frame)) {
        std::vector<Detection> output = pIntf_->runInference(frame);
        int detections = output.size();

        for (int i = 0; i < detections; ++i)
        {
            Detection detection = output[i];
            cv::Rect box = detection.box;
            cv::Scalar color = detection.color;
            cv::rectangle(frame, box, color, 2);
            std::string classString = detection.className + " " + std::to_string(detection.confidence).substr(0, 4);
            cv::putText(frame, classString, cv::Point(box.x, box.y - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
        }
        view->loadImage(frame);
    } else {
        stopDetection();
    }
}