#ifndef GAUSSWIDGET_H
#define GAUSSWIDGET_H

#include "boxforgauss.h"

#include <QWidget>
#include <QMouseEvent>

class GaussWidgetPrivate;
class GaussWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GaussWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void changeEvent(QEvent *e);

private:
    Q_DECLARE_PRIVATE(GaussWidget)
    GaussWidgetPrivate *d_ptr;
    int index = 0;
};

#endif // GAUSSWIDGET_H
