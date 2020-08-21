#include "widget.h"
#include <QApplication>
#include "gausswidget.h"

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<QApplication *>(QCoreApplication::instance()))

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Widget w;
    GaussWidget w;
    w.show();

    return a.exec();
}
