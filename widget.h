#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>

class BoxForGauss;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    double *gaussVector(int radius);
    bool gaussImage(uint32_t *origin, int width, int height, int radius);

    ~Widget();

private:
    QString filepath = Q_NULLPTR;
    BoxForGauss *gauss;
};

#endif // WIDGET_H
