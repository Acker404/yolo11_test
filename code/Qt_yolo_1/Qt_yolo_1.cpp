#include "Qt_yolo_1.h"  
#include <QFileDialog>  
cv :: Mat QimageToMat(const QImage& image)  
{  
    QImage convertedImage = image.convertToFormat(QImage::Format_RGB888);  
    return cv::Mat(convertedImage.height(), convertedImage.width(), CV_8UC3, (void*)convertedImage.bits(), convertedImage.bytesPerLine()).clone();  
}
Qt_yolo_1::Qt_yolo_1(QWidget* parent)  
    : QMainWindow(parent)  
{  
    ui.setupUi(this);  
	bool runOnGPU = false; // Set to false if you want to run on CPU
	std::string modelPath = "C:/Users/mark9/Desktop/yolo11_test/model/yolo11s.onnx"; // Path to your ONNX model
	std::string classesPath = "C:/Users/mark9/Desktop/yolo11_test/classes.txt"; // Path to your classes file
	pIntf_ = new Inference(modelPath, cv::Size(640, 640), classesPath, runOnGPU);

    QObject::connect(ui.open, &QPushButton::clicked, this, [=]() {  
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Images (*.png *.jpg *.bmp)"));  
        if (!fileName.isEmpty()) {  
			QImage image(fileName);

			cv::Mat frame = QimageToMat(image);

            std::vector<Detection> output = pIntf_->runInference(frame);

            int detections = output.size();
            std::cout << "Number of detections:" << detections << std::endl;

            for (int i = 0; i < detections; ++i)
            {
                Detection detection = output[i];

                cv::Rect box = detection.box;
                cv::Scalar color = detection.color;

                // Detection box
                cv::rectangle(frame, box, color, 2);

                // Detection box text
                std::string classString = detection.className + ' ' + std::to_string(detection.confidence).substr(0, 4);
                cv::Size textSize = cv::getTextSize(classString, cv::FONT_HERSHEY_DUPLEX, 1, 2, 0);
                cv::Rect textBox(box.x, box.y - 40, textSize.width + 10, textSize.height + 20);

                cv::rectangle(frame, textBox, color, cv::FILLED);
                cv::putText(frame, classString, cv::Point(box.x + 5, box.y - 10), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 2, 0);
            }
            // Inference ends here...

            // This is only for preview purposes
            float scale = 0.8;
            cv::resize(frame, frame, cv::Size(frame.cols * scale, frame.rows * scale));
            cv::imshow("Inference", frame);
        }  
    });  
}  

Qt_yolo_1::~Qt_yolo_1()  
{  
    delete pIntf_;  
	pIntf_ = nullptr;
}
    