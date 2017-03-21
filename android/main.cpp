#include "set.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    set w;
    w.show();

    return a.exec();
}
