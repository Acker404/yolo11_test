#include "Qt_yolo_1.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Qt_yolo_1 window;
    window.show();
    return app.exec();
}
