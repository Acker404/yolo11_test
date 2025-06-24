#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Qt_yolo_1.h"
#include "inference.h"

class Qt_yolo_1 : public QMainWindow
{
    Q_OBJECT

public:
    Qt_yolo_1(QWidget *parent = nullptr);
    ~Qt_yolo_1();

private:
    Ui::Qt_yolo_1Class ui;

    Inference* pIntf_ {nullptr}; // Pointer to the Inference class for YOLOv8
};

