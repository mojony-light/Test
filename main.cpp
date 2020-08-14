#include "widget.h"
#include <QApplication>
#include "gausswidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Widget w;
    GaussWidget w;
    w.show();

    return a.exec();
}
