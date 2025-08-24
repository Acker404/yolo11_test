# Smoking Detection GUI

This is a desktop application built with C++ and Qt for detecting smoking-related activities in images, videos, and live camera feeds. It uses a YOLO object detection model running on the ONNX Runtime to identify objects such as cigarettes and hands with cigarettes, providing a user-friendly interface for visualization and data export.

## Features

### Multi-Source Media Processing
- **Single Image**: Analyze individual image files (`.png`, `.jpg`, `.bmp`).
- **Video File**: Process pre-recorded videos (`.mp4`, `.avi`).
- **Live Stream**: Connect to a local camera for real-time detection.
- **Batch Processing**: Analyze a folder of images and export the results for all of them.

### Detection & Visualization
- **YOLO-based Detection**: Powered by a YOLO ONNX model optimized for smoking-related object classes.
- **Real-Time Bounding Boxes**: Displays colored bounding boxes with class labels and confidence scores on detected objects.
- **Privacy Mosaic**: An optional feature to apply a mosaic effect over detected areas.

### Comprehensive Exporting
The application features a flexible export system that adapts to the media source.
1.  **Set Export Path**: First, select a main folder for all exports.
2.  **Configure Options**: Choose what to save:
    - `Image`: The frame with detections drawn on it.
    - `Label`: A YOLO-format `.txt` file with coordinates for each detection.
    - `CSV`: A comma-separated value file summarizing the detections.
3.  **Export**:
    - **For Images/Folders**: Performs a one-time export of the selected data. A progress bar is shown for folder exports.
    - **For Videos/Streams**: Acts as a toggle. When turned on, it continuously saves detection results to a dedicated subfolder (named after the video or a timestamped camera session). Click the button again to stop exporting. The CSV file for this mode contains timestamps for each detection.

## How to Use

1.  **Load Media**: Use the buttons on the right panel to:
    - `選擇圖片` (Select Image)
    - `選擇影片` (Select Video)
    - `開啟資料夾` (Open Folder)
    - `即時串流` (Live Stream)
2.  **Start Detection**: Click `啟用偵測` (Enable Detection) to run the model. For videos and streams, this begins continuous analysis. Click `停止` (Stop) to pause.
3.  **Adjust View**: Use the checkboxes to toggle the `標框` (Mark Box) and `馬賽克` (Mosaic) effects on the display.
4.  **Export Results**:
    - Click `輸出路徑` (Export Path) to choose where to save the results.
    - Click `輸出設定` (Export Settings) to check the boxes for the data types you want.
    - Click `輸出` (Export).
        - If you are viewing a video or stream, this button will change to `停止輸出` (Stop Export). Click it again to end the export session.

## Technical Stack

-   **Core Logic**: C++
-   **GUI**: Qt Framework
-   **Computer Vision & Inference**: OpenCV, ONNX Runtime
-   **Model**: A custom-trained YOLO model for smoking detection.

## Building from Source

### Prerequisites
- A C++ compiler supporting C++17
- Qt (5 or 6)
- OpenCV 4 or later
- ONNX Runtime

Once the prerequisites are installed, you can open the `.pro` or `CMakeLists.txt` file in Qt Creator to build and run the project.