#include "window/mainwdo.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWdo w;
    w.show();
    return a.exec();
}
