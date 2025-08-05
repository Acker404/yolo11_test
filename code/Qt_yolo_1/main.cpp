#include "Qt_yolo_1.h"
#include <QtWidgets/QApplication>
#include <windows.h>

int main(int argc, char *argv[])
{
    SetDllDirectory(NULL);  // 阻止從工作目錄搜尋 DLL
    QApplication app(argc, argv);
    Qt_yolo_1 window;
    window.show();
    return app.exec();
}
