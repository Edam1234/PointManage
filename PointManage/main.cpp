#include "PointManage.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PointManage w;
    w.show();
    return a.exec();
}
