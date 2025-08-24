// Ultralytics 🚀 AGPL-3.0 License - https://ultralytics.com/license

#include "inference.h"

Inference::Inference(const std::string &onnxModelPath, const cv::Size &modelInputShape, const std::string &classesTxtFile, const bool &runWithCuda)
{
    modelPath = onnxModelPath;
    modelShape = modelInputShape;
    classesPath = classesTxtFile;
    cudaEnabled = runWithCuda;

    loadOnnxNetwork();
    //loadClassesFromFile(); //The classes are hard-coded for this example
}

std::vector<Detection> Inference::runInference(const cv::Mat& input)
{
    cv::Mat modelInput = input;
    int pad_x, pad_y;
    float scale;
    if (letterBoxForSquare && modelShape.width == modelShape.height)
        modelInput = formatToSquare(modelInput, &pad_x, &pad_y, &scale);

    cv::Mat blob;
    cv::dnn::blobFromImage(modelInput, blob, 1.0 / 255.0, modelShape, cv::Scalar(), true, false);
    net.setInput(blob);

    std::vector<cv::Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());

    int rows = outputs[0].size[1];
    int dimensions = outputs[0].size[2];

    bool yolov8 = false;

    // 檢查是否為 YOLOv8 格式
    // 您的模型輸出: [1, 7, 18900] - 7個維度 (4個box座標 + 3個類別分數)
    if (dimensions > rows) // shape[2] > shape[1] 表示 YOLOv8 格式
    {
        yolov8 = true;
        rows = outputs[0].size[2];        // 18900 個預測框
        dimensions = outputs[0].size[1];   // 7 個維度

        // 重塑輸出張量從 [1, 7, 18900] 到 [18900, 7]
        outputs[0] = outputs[0].reshape(1, dimensions); // [7, 18900]
        cv::transpose(outputs[0], outputs[0]);          // [18900, 7]
    }

    float* data = (float*)outputs[0].data;

    std::vector<int> class_ids;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    for (int i = 0; i < rows; ++i)
    {
        if (yolov8)
        {
            // YOLOv8 抽煙檢測模型格式: [x, y, w, h, class1_score, class2_score, class3_score]
            // 其中: class1=cigarette, class2=hand_with_cigarette, class3=mouth_with_cigarette

            // 提取邊界框座標
            float x = data[0];
            float y = data[1];
            float w = data[2];
            float h = data[3];

            // 提取類別分數 (從索引4開始的3個類別)
            float* class_scores = data + 4;  // 指向 [class1_score, class2_score, class3_score]

            // 找到最高分數的類別
            cv::Mat scores(1, classes.size(), CV_32FC1, class_scores);
            cv::Point class_id;
            double max_class_score;
            minMaxLoc(scores, 0, &max_class_score, 0, &class_id);

            // 使用最高類別分數作為置信度 (YOLOv8沒有單獨的objectness score)
            float confidence = max_class_score;

            // 檢查置信度閾值
            if (confidence >= modelConfidenceThreshold)
            {
                confidences.push_back(confidence);
                class_ids.push_back(class_id.x);

                // 座標轉換: 中心點格式轉換為左上角格式
                int left = int((x - 0.5 * w - pad_x) / scale);
                int top = int((y - 0.5 * h - pad_y) / scale);
                int width = int(w / scale);
                int height = int(h / scale);

                // 確保邊界框在圖像範圍內
                left = std::max(0, left);
                top = std::max(0, top);
                width = std::min(width, input.cols - left);
                height = std::min(height, input.rows - top);

                boxes.push_back(cv::Rect(left, top, width, height));
            }
        }
        else // YOLOv5 格式 (保持原有邏輯)
        {
            float confidence = data[4];

            if (confidence >= modelConfidenceThreshold)
            {
                float* classes_scores = data + 5;

                cv::Mat scores(1, classes.size(), CV_32FC1, classes_scores);
                cv::Point class_id;
                double max_class_score;

                minMaxLoc(scores, 0, &max_class_score, 0, &class_id);

                if (max_class_score > modelScoreThreshold)
                {
                    confidences.push_back(confidence);
                    class_ids.push_back(class_id.x);

                    float x = data[0];
                    float y = data[1];
                    float w = data[2];
                    float h = data[3];

                    int left = int((x - 0.5 * w - pad_x) / scale);
                    int top = int((y - 0.5 * h - pad_y) / scale);
                    int width = int(w / scale);
                    int height = int(h / scale);

                    boxes.push_back(cv::Rect(left, top, width, height));
                }
            }
        }

        data += dimensions;
    }

    // 非最大值抑制 (NMS)
    std::vector<int> nms_result;
    cv::dnn::NMSBoxes(boxes, confidences, modelConfidenceThreshold, modelNMSThreshold, nms_result);

    std::vector<Detection> detections{};
    for (unsigned long i = 0; i < nms_result.size(); ++i)
    {
        int idx = nms_result[i];

        Detection result;
        result.class_id = class_ids[idx];
        result.confidence = confidences[idx];

        // 為不同類別設置不同顏色
        if (result.class_id == 0) // cigarette - 紅色
            result.color = cv::Scalar(0, 0, 255);
        else if (result.class_id == 1) // hand_with_cigarette - 綠色
            result.color = cv::Scalar(0, 255, 0);
        else if (result.class_id == 2) // mouth_with_cigarette - 藍色
            result.color = cv::Scalar(255, 0, 0);
        else // 其他類別 - 隨機顏色
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dis(100, 255);
            result.color = cv::Scalar(dis(gen), dis(gen), dis(gen));
        }

        result.className = classes[result.class_id];
        result.box = boxes[idx];

        detections.push_back(result);
    }

    return detections;
}

void Inference::loadClassesFromFile()
{
    std::ifstream inputFile(classesPath);
    if (inputFile.is_open())
    {
        std::string classLine;
        while (std::getline(inputFile, classLine))
            classes.push_back(classLine);
        inputFile.close();
    }
}

void Inference::loadOnnxNetwork()
{
    net = cv::dnn::readNetFromONNX(modelPath);
    if (cudaEnabled)
    {
        std::cout << "\nRunning on CUDA" << std::endl;
        net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
        net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
    }
    else
    {
        std::cout << "\nRunning on CPU" << std::endl;
        net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
        net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    }
}


cv::Mat Inference::formatToSquare(const cv::Mat &source, int *pad_x, int *pad_y, float *scale)
{
    int col = source.cols;
    int row = source.rows;
    int m_inputWidth = modelShape.width;
    int m_inputHeight = modelShape.height;

    *scale = std::min(m_inputWidth / (float)col, m_inputHeight / (float)row);
    int resized_w = col * *scale;
    int resized_h = row * *scale;
    *pad_x = (m_inputWidth - resized_w) / 2;
    *pad_y = (m_inputHeight - resized_h) / 2;

    cv::Mat resized;
    cv::resize(source, resized, cv::Size(resized_w, resized_h));
    cv::Mat result = cv::Mat::zeros(m_inputHeight, m_inputWidth, source.type());
    resized.copyTo(result(cv::Rect(*pad_x, *pad_y, resized_w, resized_h)));
    resized.release();
    return result;
}
