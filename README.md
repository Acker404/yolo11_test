# YOLOv11 Real-Time Object Detection

This project is a real-time object detection application that uses the YOLOv11 model to identify objects in images and videos. The application is built with C++ and Qt, and it uses the ONNX runtime for inference.

## Features

*   **Real-time object detection**: Detects objects in images and video streams.
*   **Image and video support**: Supports a variety of image and video formats.
*   **Interactive UI**: Allows users to open files, start and stop detection, and view the results in real-time.

## Technologies Used

*   **C++**: The core application logic is written in C++.
*   **Qt**: The graphical user interface is built with the Qt framework.
*   **OpenCV**: Used for image and video processing.
*   **Ultralytics YOLOv11**: The object detection model is based on the YOLOv11 architecture.
*   **ONNX Runtime**: The model is run using the ONNX runtime for efficient inference.

## Getting Started

To run the application, you will need to have the following installed:

*   A C++ compiler that supports C++17
*   Qt 6 or later
*   OpenCV 4 or later

Once you have the prerequisites installed, you can open the project in Qt Creator and build and run the application.

## Model

The application uses a pretrained YOLOv11s model that has been exported to the ONNX format. The model is included in the `model` directory.

## References

*   **Ultralytics YOLO**: https://github.com/ultralytics/ultralytics
