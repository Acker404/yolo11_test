# YOLOv8 Smoking Detection GUI

A professional desktop application built with C++ and the Qt framework for real-time detection of smoking-related activities. This tool leverages the power of YOLOv8 models via the ONNX Runtime to analyze images, videos, and live camera feeds, providing a rich user interface for visualizing results and exporting data.

## Key Features

*   **Multi-Source Media Analysis**: Process media from various sources:
    *   **Single Image**: Analyze individual `.png`, `.jpg`, or `.bmp` files.
    *   **Video File**: Run detection on pre-recorded `.mp4` or `.avi` videos.
    *   **Live Stream**: Connect to local camera devices for real-time monitoring.
    *   **Batch Processing**: Efficiently analyze an entire folder of images at once.
*   **Advanced Detection & Visualization**:
    *   **High-Performance Inference**: Utilizes a YOLOv8 ONNX model for fast and accurate object detection.
    *   **Dynamic Bounding Boxes**: Overlays colored bounding boxes, class labels, and confidence scores on detected objects.
    *   **Privacy Mosaic**: An optional feature to apply a mosaic effect over detected regions, useful for privacy preservation.
    *   **Adjustable Thresholds**: Fine-tune `Confidence`, `Score`, and `NMS` thresholds via UI sliders to optimize detection results.
*   **Comprehensive Data Export**:
    *   **Flexible Export Options**: Save detection results as images with bounding boxes, YOLO-format `.txt` label files, or a summary `.csv` file.
    *   **Context-Aware Exporting**:
        *   **Single Media/Batch**: Performs a one-time export of the analysis results.
        *   **Video/Stream**: Toggles a continuous export mode, saving timestamped detection data until stopped.

## Installation & Environment Requirements

This is a C++ project with the following runtime and build requirements:

*   **Runtime Environment**: Windows
*   **Core Dependencies**:
    *   **Qt Framework**: Version 5 or 6 for the graphical user interface.
    *   **OpenCV**: Version 4 or later for computer vision tasks.
    *   **ONNX Runtime**: Used for running inference with the YOLO model. The project is configured to use `Microsoft.ML.OnnxRuntime v1.22.1`.

To build from source, you will need a C++ compiler supporting C++17 and the development libraries for the dependencies listed above.

## Usage

1.  **Build and Run**: Compile the project using a C++ compiler and Qt's build system (e.g., qmake or CMake). Launch the resulting executable.
2.  **Load Media**: Use the buttons in the side panel to select your media source:
    *   `選擇圖片` (Select Image)
    *   `選擇影片` (Select Video)
    *   `開啟資料夾` (Open Folder)
    *   `即時串流` (Live Stream)
3.  **Control Detection**:
    *   Click `啟用偵測` (Enable Detection) to start the analysis.
    *   Click `停止` (Stop) to pause or end the detection.
    *   Use the `標框` (Mark Box) and `馬賽克` (Mosaic) checkboxes to control visualization.
4.  **Export Results**:
    *   Click `輸出路徑` (Export Path) to select a destination folder.
    - Click `輸出設定` (Export Settings) to choose the desired output formats (Image, Label, CSV).
    - Click `輸出` (Export) to save the results. For videos and streams, this button acts as a toggle to start and stop continuous exporting.

## API Documentation

The project is primarily structured around a few key classes:

### `Qt_yolo_1`
The main window and core application logic. It handles user interactions, manages media loading, and orchestrates the detection and export processes.

### `Inference`
A wrapper class for the ONNX Runtime that handles the YOLOv8 model.

*   **`Inference(onnxModelPath, modelInputShape, classesTxtFile, runWithCuda)`**: Constructor. Loads the ONNX model and sets it up for either CPU or CUDA execution.
*   **`std::vector<Detection> runInference(const cv::Mat &input)`**: Takes a `cv::Mat` image as input, performs preprocessing, runs inference, and returns a vector of `Detection` structs.
*   **`setConfidenceThreshold(float conf)`**: Sets the confidence threshold for filtering detections.
*   **`setNMSThreshold(float nms)`**: Sets the Non-Maximum Suppression threshold.

**Example Usage:**
```cpp
// Initialize inference engine
Inference yolo("path/to/model.onnx", cv::Size(640, 640), "", true);
// Load an image
cv::Mat image = cv::imread("path/to/image.jpg");
// Run detection
std::vector<Detection> results = yolo.runInference(image);
```

### `ImageView`
A custom `QGraphicsView` widget designed to display `cv::Mat` images while handling scaling and aspect ratio correctly.

*   **`void loadImage(const cv::Mat &image)`**: Clears the view and displays a new image.

### `DetectionWorker`
A `QObject` designed to run the inference process in a separate thread to keep the UI responsive.

*   **`void processImage(const cv::Mat& image)`**: Processes a single image and emits `detectionFinished` when done.
*   **`void processFrame(const cv::Mat& frame)`**: Processes a single video frame.

## Developer Information

### Running Tests
This project does not currently include a dedicated test suite. To ensure quality, manual testing of the UI and export functionality is recommended after making changes.

### Contribution Guidelines
Contributions are welcome. Please follow these steps:
1.  Fork the repository.
2.  Create a new branch for your feature or bug fix.
3.  Commit your changes with clear and descriptive messages.
4.  Push your branch and submit a pull request.
5.  Ensure your code adheres to the existing style and conventions.

### License
This project is licensed under the **AGPL-3.0 License**. The license details can be found in the `inference.h` and `inference.cpp` files.
