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
#include <QtWidgets/QSpinBox>
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
    QLabel *label_media_info;
    QVBoxLayout *verticalLayout_tool;
    QPushButton *Button_exportPath;
    QVBoxLayout *verticalLayout_detect;
    QHBoxLayout *horizontalLayout_detect_fram_set;
    QLabel *label_detect_fram_info;
    QSpinBox *spinBox_detect_frame_set;
    QHBoxLayout *horizontalLayout_detect_button;
    QPushButton *Button_Detection;
    QPushButton *Button_Detection_stop;
    QVBoxLayout *verticalLayout_out;
    QHBoxLayout *horizontalLayout_out_select;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox;
    QPushButton *Button_export;
    QVBoxLayout *verticalLayout_open;
    QPushButton *Button_stream;
    QVBoxLayout *verticalLayout_open_local_file;
    QHBoxLayout *horizontalLayout_open_local_single;
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
        Qt_yolo_1Class->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Qt_yolo_1Class->sizePolicy().hasHeightForWidth());
        Qt_yolo_1Class->setSizePolicy(sizePolicy);
        Qt_yolo_1Class->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
        Qt_yolo_1Class->setDocumentMode(false);
        centralWidget = new QWidget(Qt_yolo_1Class);
        centralWidget->setObjectName("centralWidget");
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        layout_media = new QVBoxLayout();
        layout_media->setSpacing(6);
        layout_media->setObjectName("layout_media");
        label_media_info = new QLabel(centralWidget);
        label_media_info->setObjectName("label_media_info");
        label_media_info->setAutoFillBackground(false);
        label_media_info->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 170, 127);"));

        layout_media->addWidget(label_media_info);


        horizontalLayout_2->addLayout(layout_media);

        verticalLayout_tool = new QVBoxLayout();
        verticalLayout_tool->setSpacing(6);
        verticalLayout_tool->setObjectName("verticalLayout_tool");
        Button_exportPath = new QPushButton(centralWidget);
        Button_exportPath->setObjectName("Button_exportPath");

        verticalLayout_tool->addWidget(Button_exportPath);

        verticalLayout_detect = new QVBoxLayout();
        verticalLayout_detect->setSpacing(8);
        verticalLayout_detect->setObjectName("verticalLayout_detect");
        verticalLayout_detect->setSizeConstraint(QLayout::SizeConstraint::SetMaximumSize);
        horizontalLayout_detect_fram_set = new QHBoxLayout();
        horizontalLayout_detect_fram_set->setSpacing(6);
        horizontalLayout_detect_fram_set->setObjectName("horizontalLayout_detect_fram_set");
        label_detect_fram_info = new QLabel(centralWidget);
        label_detect_fram_info->setObjectName("label_detect_fram_info");
        label_detect_fram_info->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_detect_fram_set->addWidget(label_detect_fram_info);

        spinBox_detect_frame_set = new QSpinBox(centralWidget);
        spinBox_detect_frame_set->setObjectName("spinBox_detect_frame_set");
        spinBox_detect_frame_set->setInputMethodHints(Qt::InputMethodHint::ImhDigitsOnly);
        spinBox_detect_frame_set->setMinimum(5);
        spinBox_detect_frame_set->setMaximum(120);
        spinBox_detect_frame_set->setSingleStep(5);
        spinBox_detect_frame_set->setValue(5);

        horizontalLayout_detect_fram_set->addWidget(spinBox_detect_frame_set);


        verticalLayout_detect->addLayout(horizontalLayout_detect_fram_set);

        horizontalLayout_detect_button = new QHBoxLayout();
        horizontalLayout_detect_button->setSpacing(6);
        horizontalLayout_detect_button->setObjectName("horizontalLayout_detect_button");
        Button_Detection = new QPushButton(centralWidget);
        Button_Detection->setObjectName("Button_Detection");

        horizontalLayout_detect_button->addWidget(Button_Detection);

        Button_Detection_stop = new QPushButton(centralWidget);
        Button_Detection_stop->setObjectName("Button_Detection_stop");

        horizontalLayout_detect_button->addWidget(Button_Detection_stop);


        verticalLayout_detect->addLayout(horizontalLayout_detect_button);


        verticalLayout_tool->addLayout(verticalLayout_detect);

        verticalLayout_out = new QVBoxLayout();
        verticalLayout_out->setSpacing(6);
        verticalLayout_out->setObjectName("verticalLayout_out");
        horizontalLayout_out_select = new QHBoxLayout();
        horizontalLayout_out_select->setSpacing(6);
        horizontalLayout_out_select->setObjectName("horizontalLayout_out_select");
        checkBox_2 = new QCheckBox(centralWidget);
        checkBox_2->setObjectName("checkBox_2");
        checkBox_2->setChecked(true);

        horizontalLayout_out_select->addWidget(checkBox_2);

        checkBox = new QCheckBox(centralWidget);
        checkBox->setObjectName("checkBox");

        horizontalLayout_out_select->addWidget(checkBox);


        verticalLayout_out->addLayout(horizontalLayout_out_select);

        Button_export = new QPushButton(centralWidget);
        Button_export->setObjectName("Button_export");

        verticalLayout_out->addWidget(Button_export);


        verticalLayout_tool->addLayout(verticalLayout_out);

        verticalLayout_open = new QVBoxLayout();
        verticalLayout_open->setSpacing(6);
        verticalLayout_open->setObjectName("verticalLayout_open");
        verticalLayout_open->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        Button_stream = new QPushButton(centralWidget);
        Button_stream->setObjectName("Button_stream");

        verticalLayout_open->addWidget(Button_stream);

        verticalLayout_open_local_file = new QVBoxLayout();
        verticalLayout_open_local_file->setSpacing(6);
        verticalLayout_open_local_file->setObjectName("verticalLayout_open_local_file");
        horizontalLayout_open_local_single = new QHBoxLayout();
        horizontalLayout_open_local_single->setSpacing(6);
        horizontalLayout_open_local_single->setObjectName("horizontalLayout_open_local_single");
        horizontalLayout_open_local_single->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);
        Button_openImage = new QPushButton(centralWidget);
        Button_openImage->setObjectName("Button_openImage");

        horizontalLayout_open_local_single->addWidget(Button_openImage);

        Button_openVideo = new QPushButton(centralWidget);
        Button_openVideo->setObjectName("Button_openVideo");

        horizontalLayout_open_local_single->addWidget(Button_openVideo);


        verticalLayout_open_local_file->addLayout(horizontalLayout_open_local_single);

        Button_openFolder = new QPushButton(centralWidget);
        Button_openFolder->setObjectName("Button_openFolder");

        verticalLayout_open_local_file->addWidget(Button_openFolder);


        verticalLayout_open->addLayout(verticalLayout_open_local_file);


        verticalLayout_tool->addLayout(verticalLayout_open);


        horizontalLayout_2->addLayout(verticalLayout_tool);

        Qt_yolo_1Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Qt_yolo_1Class);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 800, 20));
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
        Qt_yolo_1Class->setWindowTitle(QCoreApplication::translate("Qt_yolo_1Class", "smokeing detect", nullptr));
        label_media_info->setText(QString());
        Button_exportPath->setText(QCoreApplication::translate("Qt_yolo_1Class", "\350\250\255\345\256\232\350\274\270\345\207\272\350\267\257\345\276\221", nullptr));
        label_detect_fram_info->setText(QCoreApplication::translate("Qt_yolo_1Class", "frame_set", nullptr));
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
