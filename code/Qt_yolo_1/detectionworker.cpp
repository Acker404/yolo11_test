#include "detectionworker.h"

DetectionWorker::DetectionWorker(Inference* inference, QObject *parent)
    : QObject(parent), pIntf_(inference)
{
}

DetectionWorker::~DetectionWorker()
{
}

void DetectionWorker::processImage(const cv::Mat& image)
{
    if (m_stop) return;
    cv::Mat processedImage = image.clone();
    std::vector<Detection> output = pIntf_->runInference(processedImage);
    for (const auto& detection : output) {
        drawDetection(processedImage, detection);
    }
    emit detectionFinished(processedImage);
    emit finished();
}

void DetectionWorker::processFrame(const cv::Mat& frame)
{
    if (m_stop) return;
    cv::Mat processedFrame = frame.clone();
    std::vector<Detection> output = pIntf_->runInference(processedFrame);
    for (const auto& detection : output) {
        drawDetection(processedFrame, detection);
    }
    emit detectionFinished(processedFrame);
}

void DetectionWorker::stop()
{
    m_stop = true;
}

void DetectionWorker::drawDetection(cv::Mat& frame, const Detection& detection)
{
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
