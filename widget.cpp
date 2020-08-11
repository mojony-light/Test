#include "widget.h"

#include <QVector>
#include <qmath.h>
#include <QDebug>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QTime>

#define OFFSET 2.57

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *clayout = new QHBoxLayout;
    QHBoxLayout *pixLayout = new QHBoxLayout;
    QLabel *originpixmap = new QLabel(this);
    QLabel *destpixmap = new QLabel(this);
    QPushButton *fileButton = new QPushButton(this);

    fileButton->setText("选择文件");
    QSlider *slider = new QSlider(this);
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0, 100);

    mainLayout->addLayout(pixLayout, 1);
    mainLayout->addLayout(clayout, 0);
    clayout->addWidget(fileButton, 0);
    clayout->addWidget(slider, 1);
    pixLayout->addWidget(originpixmap);
    pixLayout->addWidget(destpixmap);

    connect(fileButton, &QPushButton::clicked, [=](){
        filepath = QFileDialog::getOpenFileName(this, "选择文件");
        originpixmap->setPixmap(QPixmap(filepath));
        destpixmap->setPixmap(filepath);
    });

    connect(slider, &QSlider::valueChanged, [=](int value){
        if (value) {
            QImage dest = gaussImage(QImage(filepath), value);
            destpixmap->setPixmap(QPixmap::fromImage(dest));
        }
    });
}

/**
    radius: 高斯矩阵的大小，长宽一直的矩阵
**/
QVector<double> Widget::gaussVector(int radius)
{
    double sigma = double(radius) / OFFSET;
    double deno = 1.0 / (sigma * qSqrt(2 * M_PI));
    double nume = -1.0 / (2 * qPow(sigma, 2));

    QVector<double> gaussVec;
    double gaussSum = 0;

    // 计算高斯矩阵
    for (int x = -radius; x <= radius; ++x) {
        // 计算目标点周围的正态分布值，形成高斯矩阵，利用此高斯矩阵计算目标值
        double value = deno * qExp(nume * qPow(x, 2));
        gaussVec.push_back(value);
        gaussSum += value;
    }  // end for

    // 高斯矩阵所有的值加起来并不等于1,计算进行归一
    for (int i = 0; i < gaussVec.length(); ++i) {
        gaussVec[i] /= gaussSum;
    }  // end for
    return gaussVec;
}

// 这里仅仅用到一维高斯函数
QImage Widget::gaussImage(const QImage &origin, int radius)
{
    // fix: 时间
    // y轴偏移
    if (origin.isNull() || !radius)
        return QImage();
    QTime time;
    QImage newImage(origin.size(), origin.format());
    QVector<double> gaussVec = gaussVector(radius);

    for (int y = 0; y < origin.height(); ++y) {
        for (int x = 0; x < origin.width(); ++x) {
            // 横向遍历整个image矩阵
            double red = 0, green = 0, blue = 0;  // 用于叠加的rgb值
            double gaussSum = 0;

            for (int l = -radius; l <= radius; ++l) {
                // 将周围n个点的rgb值重新通过权重进行计算，一维高斯函数只有水平的周围坐标
                int xp = x + l;  // 当前距离x范围的值

                if (0 <= xp && xp < origin.width()) {
                    // 有效区域，0-width的范围内的坐标，取出其rgb进行计算
                    QRgb rgb = origin.pixel(xp, y);
                    int cr = qRed(rgb);
                    int cg = qGreen(rgb);
                    int cb = qBlue(rgb);

                    double t = gaussVec[l + radius];
                    red += cr * t;  // i从-radius开始，所以这里计算的时候要+radius
                    green += cg * t;
                    blue += cb * t;
                    gaussSum += t;  // 这里的高斯矩阵如果radius范围内的所有坐标都是有效的才会都用到，因此高斯和需要重新计算，否则可能基数不为1
                }  // end if
            }  // end for

            newImage.setPixel(x, y, qRgba(int(red / gaussSum), int(green / gaussSum), int(blue/ gaussSum), origin.pixelColor(x, y).alpha()));
        }  // end j
    } // end i

    return newImage;
}

Widget::~Widget()
{

}
