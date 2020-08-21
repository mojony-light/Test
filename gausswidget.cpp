#include "gausswidget.h"

#include <QDebug>
#include <QPainter>
#include <QApplication>
#include <QScreen>
#include <qpa/qplatformbackingstore.h>
#include <QBackingStore>

class GaussWidgetPrivate
{
    GaussWidgetPrivate(GaussWidget *qq) :
        q_ptr(qq),
        gaussHandler(new BoxForGauss(qq)),
        radius(50)
    {
    }

    GaussWidget *q_ptr;
    BoxForGauss *gaussHandler;
    int radius;
    QPoint dragPosition;
    Q_DECLARE_PUBLIC(GaussWidget)
};

GaussWidget::GaussWidget(QWidget *parent)
    : QWidget(parent),
      dd_ptr(new GaussWidgetPrivate(this))
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
}

GaussWidget::~GaussWidget()
{

}

void GaussWidget::paintEvent(QPaintEvent *e)
{
    Q_D(GaussWidget);
    QWidget::paintEvent(e);
    QPainter p(this);
    QImage image = window()->backingStore()->handle()->toImage();
    image.save("/home/uos/Desktop/1.jpg");
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::NoBrush);
    p.setOpacity(0.7);

    if (((parent() && (this->windowFlags() & Qt::Dialog)) || !parent())) {
        d->gaussHandler->gaussImage(reinterpret_cast<uint32_t *>(image.bits()), image.width(), image.height(), d->radius);
        p.setRenderHints(QPainter::Antialiasing);
        p.drawImage(0, 0, image);
    }
}

void GaussWidget::mouseMoveEvent(QMouseEvent *e)
{
    Q_D(GaussWidget);
    QWidget::mouseMoveEvent(e);

    if (e->buttons() & Qt::LeftButton) {
        this->move(e->globalPos() - d->dragPosition);
        this->update();
    }
}

void GaussWidget::mousePressEvent(QMouseEvent *e)
{
    Q_D(GaussWidget);

    if(e->button() == Qt::LeftButton) {
       d->dragPosition = e->globalPos()-frameGeometry().topLeft();
       e->accept();
    }
}

void GaussWidget::mouseReleaseEvent(QMouseEvent *e)
{

}

void GaussWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);

}
