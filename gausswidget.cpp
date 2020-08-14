#include "gausswidget.h"

#include <QDebug>
#include <QPainter>
#include <QApplication>
#include <QScreen>

class GaussWidgetPrivate
{
    GaussWidgetPrivate(GaussWidget *qq) :
        q_ptr(qq),
        gaussHandler(new BoxForGauss(qq)),
        radius(00),
        pPixmap(qGuiApp->primaryScreen()->grabWindow(qq->winId(), 0, 0, qGuiApp->primaryScreen()->size().width(), qGuiApp->primaryScreen()->size().height()))
    {
    }

private:
    Q_DECLARE_PUBLIC(GaussWidget)
    GaussWidget *q_ptr;
    BoxForGauss *gaussHandler;
    int radius;
    QPoint dragPosition;
    QPixmap pPixmap;
};

GaussWidget::GaussWidget(QWidget *parent)
    : QWidget(parent),
      d_ptr(new GaussWidgetPrivate(this))
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
}

void GaussWidget::paintEvent(QPaintEvent *e)
{
    Q_D(GaussWidget);
    QWidget::paintEvent(e);
    QPainter p(this);

    if (((parent() && (this->windowFlags() & Qt::Dialog)) || !parent())) {
        QImage pImage = d->pPixmap.toImage();
        pImage = pImage.copy(this->x(), this->y(), this->width(), this->height());
        d->gaussHandler->gaussImage(reinterpret_cast<uint32_t *>(pImage.bits()), pImage.width(), pImage.height(), d->radius);
        p.setRenderHints(QPainter::Antialiasing);
//        p.setOpacity(0.2);
        p.drawImage(0, 0, pImage);
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
    Q_D(GaussWidget);
    QWidget::changeEvent(e);

    switch (e->type()) {
    case QEvent::ActivationChange:
        d->pPixmap = qGuiApp->primaryScreen()->grabWindow(this->winId(), 0, 0, qGuiApp->primaryScreen()->size().width(), qGuiApp->primaryScreen()->size().height());
        break;
    default:
        break;
    }
}
