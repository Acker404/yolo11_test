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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Qt_yolo_1Class
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *layout_media;
    QLabel *labelpicture;
    QVBoxLayout *verticalLayout;
    QPushButton *Button_exportPath;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *Button_Detection;
    QPushButton *Button_Detection_stop;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox;
    QPushButton *Button_export;
    QVBoxLayout *verticalLayout_2;
    QPushButton *Button_stream;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *Button_openImage;
    QPushButton *Button_openVideo;
    QPushButton *Button_openFolder;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Qt_yolo_1Class)
    {
        if (Qt_yolo_1Class->objectName().isEmpty())
            Qt_yolo_1Class->setObjectName("Qt_yolo_1Class");
        Qt_yolo_1Class->resize(559, 400);
        centralWidget = new QWidget(Qt_yolo_1Class);
        centralWidget->setObjectName("centralWidget");
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        layout_media = new QVBoxLayout();
        layout_media->setSpacing(6);
        layout_media->setObjectName("layout_media");
        labelpicture = new QLabel(centralWidget);
        labelpicture->setObjectName("labelpicture");
        labelpicture->setAutoFillBackground(false);
        labelpicture->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 127);"));

        layout_media->addWidget(labelpicture);


        horizontalLayout_2->addLayout(layout_media);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName("verticalLayout");
        Button_exportPath = new QPushButton(centralWidget);
        Button_exportPath->setObjectName("Button_exportPath");

        verticalLayout->addWidget(Button_exportPath);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        Button_Detection = new QPushButton(centralWidget);
        Button_Detection->setObjectName("Button_Detection");

        horizontalLayout_3->addWidget(Button_Detection);

        Button_Detection_stop = new QPushButton(centralWidget);
        Button_Detection_stop->setObjectName("Button_Detection_stop");

        horizontalLayout_3->addWidget(Button_Detection_stop);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        checkBox_2 = new QCheckBox(centralWidget);
        checkBox_2->setObjectName("checkBox_2");
        checkBox_2->setChecked(true);

        horizontalLayout_4->addWidget(checkBox_2);

        checkBox = new QCheckBox(centralWidget);
        checkBox->setObjectName("checkBox");

        horizontalLayout_4->addWidget(checkBox);


        verticalLayout_3->addLayout(horizontalLayout_4);

        Button_export = new QPushButton(centralWidget);
        Button_export->setObjectName("Button_export");

        verticalLayout_3->addWidget(Button_export);


        verticalLayout->addLayout(verticalLayout_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        Button_stream = new QPushButton(centralWidget);
        Button_stream->setObjectName("Button_stream");

        verticalLayout_2->addWidget(Button_stream);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        Button_openImage = new QPushButton(centralWidget);
        Button_openImage->setObjectName("Button_openImage");

        horizontalLayout_5->addWidget(Button_openImage);

        Button_openVideo = new QPushButton(centralWidget);
        Button_openVideo->setObjectName("Button_openVideo");

        horizontalLayout_5->addWidget(Button_openVideo);


        verticalLayout_2->addLayout(horizontalLayout_5);

        Button_openFolder = new QPushButton(centralWidget);
        Button_openFolder->setObjectName("Button_openFolder");

        verticalLayout_2->addWidget(Button_openFolder);


        verticalLayout->addLayout(verticalLayout_2);


        horizontalLayout_2->addLayout(verticalLayout);

        Qt_yolo_1Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Qt_yolo_1Class);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 559, 20));
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
        labelpicture->setText(QString());
        Button_exportPath->setText(QCoreApplication::translate("Qt_yolo_1Class", "\350\250\255\345\256\232\350\274\270\345\207\272\350\267\257\345\276\221", nullptr));
        Button_Detection->setText(QCoreApplication::translate("Qt_yolo_1Class", "\345\225\237\347\224\250\345\201\265\346\270\254", nullptr));
        Button_Detection_stop->setText(QCoreApplication::translate("Qt_yolo_1Class", "\345\201\234\346\255\242", nullptr));
        checkBox_2->setText(QCoreApplication::translate("Qt_yolo_1Class", "\346\250\231\346\241\206", nullptr));
        checkBox->setText(QCoreApplication::translate("Qt_yolo_1Class", "\351\246\254\350\263\275\345\205\213", nullptr));
        Button_export->setText(QCoreApplication::translate("Qt_yolo_1Class", "\350\274\270\345\207\272", nullptr));
        Button_stream->setText(QCoreApplication::translate("Qt_yolo_1Class", "\345\215\263\346\231\202\344\270\262\346\265\201", nullptr));
        Button_openImage->setText(QCoreApplication::translate("Qt_yolo_1Class", "\351\201\270\346\223\207\345\234\226\347\211\207", nullptr));
        Button_openVideo->setText(QCoreApplication::translate("Qt_yolo_1Class", "\351\201\270\346\223\207\345\275\261\347\211\207", nullptr));
        Button_openFolder->setText(QCoreApplication::translate("Qt_yolo_1Class", "\351\226\213\345\225\237\350\263\207\346\226\231\345\244\276", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Qt_yolo_1Class: public Ui_Qt_yolo_1Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT_YOLO_1_H
