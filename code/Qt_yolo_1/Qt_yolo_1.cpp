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
	std::string modelPath = "C:/Users/mark9/Desktop/yolo11_test/model/best.onnx"; // Path to your ONNX model
	std::string classesPath = "C:/Users/mark9/Desktop/yolo11_test/classes.txt"; // Path to your classes file
	pIntf_ = new Inference(modelPath, cv::Size(960, 960), classesPath, runOnGPU);

    QObject::connect(ui.open, &QPushButton::clicked, this, [=]() {  
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Images (*.png *.jpg *.bmp)"));  
        if (!fileName.isEmpty()) {  
			QImage image(fileName);
            //pIntf_->loadClassesFromFile();
			cv::Mat frame = QimageToMat(image);
            cv::Mat input_bgr;
            cv::cvtColor(frame, input_bgr, cv::COLOR_RGB2BGR);
            std::vector<Detection> output = pIntf_->runInference(input_bgr);

            int detections = output.size();
            std::cout << "Number of detections:" << detections << std::endl;

            for (int i = 0; i < detections; ++i) // Change 'detections.size()' to 'detections' since 'detections' is an integer
            {
                Detection detection = output[i];

                cv::Rect box = detection.box;
                cv::Scalar color = detection.color;

                // Detection box
                cv::rectangle(input_bgr, box, color, 2);

                // Format class string with confidence (2 decimal places)
                std::ostringstream ss;
                ss << std::fixed << std::setprecision(2) << detection.confidence;
                //std::string classString = detection.className + " " + ss.str();

                // Draw label background
                //cv::Size textSize = cv::getTextSize(classString, cv::FONT_HERSHEY_DUPLEX, 1, 2, 0);
                //cv::Rect textBox(box.x, box.y - 40, textSize.width + 10, textSize.height + 20);
                //cv::rectangle(input_bgr, textBox, color, cv::FILLED);
                std::string classString = detection.className + " " + std::to_string(detection.confidence).substr(0, 4);

                // 1. 根據圖片大小自動計算文字大小
                int imgHeight = input_bgr.rows;
                int imgWidth = input_bgr.cols;
                double scaleFactor = std::max(0.5, std::min(1.0, imgHeight / 720.0)); // 建議比例範圍 0.5 ~ 1.0
                int fontThickness = std::round(2 * scaleFactor);
                double fontScale = 0.6 * scaleFactor;

                // 2. 計算文字區塊大小
                cv::Size textSize = cv::getTextSize(classString, cv::FONT_HERSHEY_DUPLEX, fontScale, fontThickness, 0);

                // 3. 判斷放文字在上面還是下面，避免超出畫面
                int label_x = box.x;
                int label_y = (box.y - textSize.height - 10 > 0) ? box.y - 10 : box.y + box.height + textSize.height + 10;

                // 4. 定義文字背景框
                cv::Rect textBox(label_x, label_y - textSize.height, textSize.width + 10, textSize.height + 10);
                textBox &= cv::Rect(0, 0, input_bgr.cols, input_bgr.rows); // 裁切防止超出邊界

                // 5. 畫背景與文字
                cv::rectangle(input_bgr, textBox, detection.color, cv::FILLED);
                cv::putText(input_bgr, classString, cv::Point(label_x + 5, label_y - 2),
                    cv::FONT_HERSHEY_DUPLEX, fontScale, cv::Scalar(0, 0, 0), fontThickness);


                //cv::putText(input_bgr, classString, cv::Point(box.x + 5, label_y), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 2);

                // Draw label text
                //cv::putText(input_bgr, classString, cv::Point(box.x + 5, box.y - 10), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 2);
            }
            // Inference ends here...

            // This is only for preview purposes       
            
            float scale = 0.8;
            //cv::resize(input_bgr, input_bgr, cv::Size(input_bgr.cols * scale, input_bgr.rows * scale));
            cv::resize(input_bgr, input_bgr, cv::Size(1280, 960));
            cv::imshow("Inference", input_bgr);
        }  
    });  
}  

Qt_yolo_1::~Qt_yolo_1()  
{  
    delete pIntf_;  
	pIntf_ = nullptr;
}
    