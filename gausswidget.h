#ifndef GAUSSWIDGET_H
#define GAUSSWIDGET_H

#include "boxforgauss.h"

#include <QWidget>
#include <QScopedPointer>
#include <QPointer>
#include <QMouseEvent>

class GaussWidgetPrivate;
class GaussWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GaussWidget(QWidget *parent = nullptr);
    ~GaussWidget();  // 必须编写析构函数

protected:
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void changeEvent(QEvent *e);

private:
    QScopedPointer<GaussWidgetPrivate> dd_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(dd_ptr), GaussWidget)
    int index = 0;
};

#endif // GAUSSWIDGET_H
