/********************************************************************************
** Form generated from reading UI file 'Qt_yolo_1.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QT_YOLO_1_H
#define UI_QT_YOLO_1_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Qt_yolo_1Class
{
public:
    QWidget *centralWidget;
    QLabel *dispResult;
    QPushButton *open;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Qt_yolo_1Class)
    {
        if (Qt_yolo_1Class->objectName().isEmpty())
            Qt_yolo_1Class->setObjectName("Qt_yolo_1Class");
        Qt_yolo_1Class->resize(600, 400);
        centralWidget = new QWidget(Qt_yolo_1Class);
        centralWidget->setObjectName("centralWidget");
        dispResult = new QLabel(centralWidget);
        dispResult->setObjectName("dispResult");
        dispResult->setGeometry(QRect(200, 40, 171, 231));
        dispResult->setFrameShape(QFrame::Shape::Box);
        open = new QPushButton(centralWidget);
        open->setObjectName("open");
        open->setGeometry(QRect(470, 240, 80, 23));
        Qt_yolo_1Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Qt_yolo_1Class);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 600, 20));
        Qt_yolo_1Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Qt_yolo_1Class);
        mainToolBar->setObjectName("mainToolBar");
        Qt_yolo_1Class->addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Qt_yolo_1Class);
        statusBar->setObjectName("statusBar");
        Qt_yolo_1Class->setStatusBar(statusBar);

        retranslateUi(Qt_yolo_1Class);

        QMetaObject::connectSlotsByName(Qt_yolo_1Class);
    } // setupUi

    void retranslateUi(QMainWindow *Qt_yolo_1Class)
    {
        Qt_yolo_1Class->setWindowTitle(QCoreApplication::translate("Qt_yolo_1Class", "Qt_yolo_1", nullptr));
        dispResult->setText(QCoreApplication::translate("Qt_yolo_1Class", "TextLabel", nullptr));
        open->setText(QCoreApplication::translate("Qt_yolo_1Class", "OPEN IMAGE", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Qt_yolo_1Class: public Ui_Qt_yolo_1Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT_YOLO_1_H
