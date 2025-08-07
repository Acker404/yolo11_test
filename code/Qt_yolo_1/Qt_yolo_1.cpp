#include "Qt_yolo_1.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QInputDialog>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QDir>
#include <QMessageBox>
#include <QSettings>
#include <QTextStream>

// Helper function to show the export options dialog
OutputOptions getOutputOptionsDialog(QWidget* parent, const OutputOptions& currentOpt) {
    QSettings settings("YourCompany", "YourApp");
    OutputOptions opt = currentOpt;

    QDialog dialog(parent);
    dialog.setWindowTitle("輸出選項");

    QVBoxLayout* layoutMain = new QVBoxLayout(&dialog);
    QHBoxLayout* layout = new QHBoxLayout;

    QCheckBox* out_labeling = new QCheckBox("標籤 (.txt)");
    QCheckBox* out_image = new QCheckBox("圖片 (with boxes)");
    QCheckBox* out_csv = new QCheckBox("CSV (for folders)");

    out_labeling->setChecked(settings.value("output/label", opt.saveLabel).toBool());
    out_image->setChecked(settings.value("output/image", opt.saveImage).toBool());
    out_csv->setChecked(settings.value("output/csv", opt.saveCSV).toBool());

    layout->addWidget(out_labeling);
    layout->addWidget(out_image);
    layout->addWidget(out_csv);
    layoutMain->addLayout(layout);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QObject::connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layoutMain->addWidget(buttonBox);

    if (dialog.exec() == QDialog::Accepted) {
        opt.saveLabel = out_labeling->isChecked();
        opt.saveImage = out_image->isChecked();
        opt.saveCSV = out_csv->isChecked();

        settings.setValue("output/label", opt.saveLabel);
        settings.setValue("output/image", opt.saveImage);
        settings.setValue("output/csv", opt.saveCSV);
    }
    return opt;
}


Qt_yolo_1::Qt_yolo_1(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    bool runOnGPU = false; 
    std::string modelPath = "C:/Users/mark9/Desktop/yolo11_test/model/best.onnx";
    std::string classesPath = "C:/Users/mark9/Desktop/yolo11_test/classes.txt";
    pIntf_ = new Inference(modelPath, cv::Size(960, 960), classesPath, runOnGPU);

    view = new ImageView(this);
    ui.layout_media->addWidget(view);

    videoTimer_ = new QTimer(this);
    connect(videoTimer_, &QTimer::timeout, this, &Qt_yolo_1::processVideoFrame);
    ui.progressBar_batch->setVisible(false);
    QObject::connect(ui.Button_openImage, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Images (*.png *.jpg *.bmp)"));
        if (!fileName.isEmpty()) {
            loadFile(fileName);
            prevButton_->hide();
            nextButton_->hide();
            ui.listWidget_fileList->clear();
            currentFolderPath_.clear(); // Ensure folder mode is off
        }
    });

    QObject::connect(ui.Button_openVideo, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Videos (*.mp4 *.avi)"));
        if (!fileName.isEmpty()) {
            loadFile(fileName);
            prevButton_->hide();
            nextButton_->hide();
            ui.listWidget_fileList->clear();
            currentFolderPath_.clear();
        }
    });

    QObject::connect(ui.Button_Detection, &QPushButton::clicked, this, &Qt_yolo_1::startDetection);
    QObject::connect(ui.Button_Detection_stop, &QPushButton::clicked, this, &Qt_yolo_1::stopDetection);
    QObject::connect(ui.Button_stream, &QPushButton::clicked, this, &Qt_yolo_1::openCameraStream);
    QObject::connect(ui.Button_openFolder, &QPushButton::clicked, this, &Qt_yolo_1::openFolder);
    connect(ui.listWidget_fileList, &QListWidget::itemClicked, this, &Qt_yolo_1::onFileListItemClicked);
    connect(ui.checkBox_markbox, &QCheckBox::checkStateChanged, this, &Qt_yolo_1::onDetectionSettingsChanged);
    connect(ui.checkBox_mosaic, &QCheckBox::checkStateChanged, this, &Qt_yolo_1::onDetectionSettingsChanged);
    QObject::connect(ui.Button_exportPath, &QPushButton::clicked, this, &Qt_yolo_1::handleExportPathClick);
    QObject::connect(ui.Button_export, &QPushButton::clicked, this, &Qt_yolo_1::handleExportClick);
    QObject::connect(ui.Button_exportSet, &QPushButton::clicked, this, &Qt_yolo_1::handleExportset);
    
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

        if (box.width > 0 && box.height > 0) {
            cv::Mat roi = frame(box);
            cv::Mat small_roi;
            cv::resize(roi, small_roi, cv::Size(10, 10), 0, 0, cv::INTER_NEAREST);
            cv::resize(small_roi, roi, box.size(), 0, 0, cv::INTER_NEAREST);
        }
    }

    if (drawMarkbox) {
        int thickness = std::max(1, (int)(frame.cols / 720.0 * 1.5));
        cv::rectangle(frame, detection.box, detection.color, thickness);
        std::string label = detection.className + " " + std::to_string(detection.confidence).substr(0, 4);
        double fontScale = 0.7;
        int baseline = 0;
        cv::Size textSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, fontScale, thickness, &baseline);
        int y_baseline = detection.box.y - 10;
        if (y_baseline - textSize.height - baseline < 0) {
            y_baseline = detection.box.y + detection.box.height + textSize.height + 5;
            if (y_baseline + baseline > frame.rows) {
                y_baseline = detection.box.y + textSize.height + 5;
            }
        }
        cv::Rect labelBackground(detection.box.x, y_baseline - textSize.height - baseline, textSize.width, textSize.height + baseline + 5);
        if (labelBackground.x < 0) labelBackground.x = 0;
        if (labelBackground.x + labelBackground.width > frame.cols) {
            labelBackground.x = frame.cols - labelBackground.width;
        }
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
            currentFolderPath_.clear();

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
        filters << "*.png" << "*.jpg" << "*.bmp";
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

void Qt_yolo_1::handleExportPathClick()
{
    exportPath = QFileDialog::getExistingDirectory(this, tr("Select Export Folder"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}

void Qt_yolo_1::handleExportset()
{
    exportOptions_ = getOutputOptionsDialog(this, exportOptions_);
}

void Qt_yolo_1::handleExportClick()
{
    // Step 1: Basic Validation
    if (exportPath.isEmpty()) {
        QMessageBox::warning(this, tr("Export Error"), tr("Please select an export path first."));
        return;
    }

    bool isBatchMode = !currentFolderPath_.isEmpty();
    bool isSingleImageMode = !originalImage_.empty();

    // Step 4: Behavior for Multiple Images (Batch Mode)
    if (isBatchMode) {
        QDir exportDir(exportPath);
        QString imageExportPath = exportPath + "/images";
        QString labelExportPath = exportPath + "/labels";

        if (exportOptions_.saveImage) exportDir.mkpath(imageExportPath);
        if (exportOptions_.saveLabel) exportDir.mkpath(labelExportPath);

        QFile csvFile;
        QTextStream csvStream;
        if (exportOptions_.saveCSV) {
            csvFile.setFileName(exportPath + "/summary.csv");
            if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QMessageBox::warning(this, "CSV Error", "Could not create summary.csv");
            }
            else {
                csvStream.setDevice(&csvFile);
                csvStream << "filename,detected,labels\n";
            }
        }

        int totalFiles = fileList_.size();
        ui.progressBar_batch->setMinimum(0);
        ui.progressBar_batch->setMaximum(totalFiles);
        ui.progressBar_batch->setValue(0);
        ui.progressBar_batch->setVisible(true);

        int index = 0;
        for (const QString& fileName : fileList_) {
            QString fullPath = QDir(currentFolderPath_).filePath(fileName);
            cv::Mat image = cv::imread(fullPath.toLocal8Bit().constData());
            if (image.empty()) {
                index++;
                ui.progressBar_batch->setValue(index);
                continue;
            }

            std::vector<Detection> detections = pIntf_->runInference(image);
            QFileInfo fileInfo(fileName);
            QString baseName = fileInfo.baseName();

            if (exportOptions_.saveImage) {
                QString savePath = imageExportPath + "/" + baseName + "_detected.png";
                saveProcessedImage(savePath, image, detections, ui.checkBox_mosaic->isChecked(), ui.checkBox_markbox->isChecked());
            }
            if (exportOptions_.saveLabel) {
                QString savePath = labelExportPath + "/" + baseName + ".txt";
                saveYoloLabels(savePath, detections, image.cols, image.rows);
            }
            if (exportOptions_.saveCSV && csvFile.isOpen()) {
                appendCSVRow(csvStream, fileName, detections);
            }

            index++;
            ui.progressBar_batch->setValue(index);
        }

        if (csvFile.isOpen()) {
            csvFile.close();
        }

        ui.progressBar_batch->setValue(totalFiles);
        ui.progressBar_batch->setVisible(false);
        QMessageBox::information(this, tr("Export Success"), tr("Batch export completed successfully."));
    }

    // Step 3: Behavior for Single Image
    else if (isSingleImageMode) {
        QFileInfo fileInfo(fileList_.isEmpty() ? "single_image" : fileList_.at(currentFileIndex_));
        QString baseName = fileInfo.baseName();

        // Run inference to get the latest detections
        std::vector<Detection> detections = pIntf_->runInference(originalImage_);

        if (exportOptions_.saveImage) {
            QString savePath = exportPath + "/" + baseName + "_detected.png";
            saveProcessedImage(savePath, originalImage_, detections, ui.checkBox_mosaic->isChecked(), ui.checkBox_markbox->isChecked());
        }
        if (exportOptions_.saveLabel) {
            QString savePath = exportPath + "/" + baseName + ".txt";
            saveYoloLabels(savePath, detections, originalImage_.cols, originalImage_.rows);
        }
        QMessageBox::information(this, tr("Export Success"), tr("Single image exported successfully."));
    }
    else {
        QMessageBox::warning(this, tr("Export Error"), tr("No image or image folder loaded to export."));
    }
}

// --- Helper Function Implementations ---

void Qt_yolo_1::saveProcessedImage(const QString& path, const cv::Mat& image, const std::vector<Detection>& detections, bool mosaic, bool markbox) {
    cv::Mat processedImage = image.clone();
    for (const auto& detection : detections) {
        drawDetection(processedImage, detection, markbox, mosaic);
    }
    cv::imwrite(path.toLocal8Bit().constData(), processedImage);
}

void Qt_yolo_1::saveYoloLabels(const QString& path, const std::vector<Detection>& detections, int imgWidth, int imgHeight) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return; // Or show an error
    }
    QTextStream out(&file);
    for (const auto& det : detections) {
        float x_center = (det.box.x + det.box.width / 2.0f) / imgWidth;
        float y_center = (det.box.y + det.box.height / 2.0f) / imgHeight;
        float width = (float)det.box.width / imgWidth;
        float height = (float)det.box.height / imgHeight;
        out << det.class_id << " " << x_center << " " << y_center << " " << width << " " << height << "\n";
    }
    file.close();
}

void Qt_yolo_1::appendCSVRow(QTextStream& stream, const QString& fileName, const std::vector<Detection>& detections) {
    bool detected = !detections.empty();
    QStringList labelList;
    for (const auto& det : detections) {
        labelList << QString::fromStdString(det.className);
    }
    stream << fileName << "," << (detected ? "true" : "false") << "," << labelList.join(", ") << "\n";
}
