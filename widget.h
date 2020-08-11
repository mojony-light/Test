#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    QVector<double> gaussVector(int radius);
    QImage gaussImage(const QImage &origin, int radius);

    ~Widget();

private:
    QString filepath = Q_NULLPTR;
};

#endif // WIDGET_H
