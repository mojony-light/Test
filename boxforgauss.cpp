#include "boxforgauss.h"

#include <qmath.h>
#include <QVector>
#include <QImage>

BoxForGauss::BoxForGauss(QObject *parent) : QObject(parent)
{

}

void BoxForGauss::gaussImage(uint32_t *origin, int width, int height, int radius)
{
    double sigma = 1.0 * radius / 2.57;
    QVector<int> boxes = boxesForGauss(sigma, 3);
    uint32_t *dest = new uint32_t[width * height];

    boxBlur(origin, dest, width, height, (boxes[0] - 1) / 2);
    boxBlur(dest, origin, width, height, (boxes[1] - 1) / 2);
    boxBlur(origin, dest, width, height, (boxes[2] - 1) / 2);

    delete[] dest;
}

QVector<int> BoxForGauss::boxesForGauss(double sigma, int size)
{
    double wideal = qSqrt((12 * qPow(sigma, 2) / size) + 1);
    double wl = qFloor(wideal);

    if ((int(wl) % 2) == 0)
        wl--;

    double wu = wl + 2;
    double mideal = (12 * qPow(sigma, 2) - size * qPow(wl, 2)
                     - 4 * size * wl - 3 * size) / (-4 * wl - 4);

    double m = qRound(mideal);
    QVector<int> boxes(size);

    for (int i = 0; i < size; ++i)
        boxes[i] = (i < m ? int(wl) : int(wu));

    return boxes;
}

void BoxForGauss::boxBlur(uint32_t *origin, uint32_t * dest, int width, int height, int radius)
{
    if (radius < 0)
        return;

    boxBlurH(origin, dest, width, height, radius);
    boxBlurV(dest, origin, width, height, radius);
}

void BoxForGauss::boxBlurH(uint32_t *origin, uint32_t *dest, int width, int height, int radius)
{
#if 0
    double iarr = 1.0 / (2 * radius + 1);

    for (int i = 0; i < height; ++i) {
        int ti = i * width, li = ti, ri = ti + radius;
        uint32_t fv = origin[ti], lv = origin[ti + width - 1], val = (uint32_t(radius) + 1) * fv;

        for (int j = 0; j < radius; ++j)
            val += origin[ti + j];

        for (int j = 0; j <= radius; ++j) {
            val += origin[ri++] - fv;
            dest[ti++] = uint32_t(qRound(val * iarr));
        }
        for (int j = radius + 1; j < width - radius; ++j) {
            val += origin[ri++] - origin[li++];
            dest[ti++]  = uint32_t(qRound(val * iarr));
        }
        for (int j = width - radius; j < width; ++j) {
            val += lv - origin[li++];
            dest[ti++] = uint32_t(qRound(val * iarr));
        }
    }
#elif 1
    int index, r = 0, g = 0, b = 0;
    int tr, tg, tb;
    uint32_t color, preColor;
    int num;
    double iarr;

    for (int i = 0; i < height; ++i) {
        r = 0;
        g = 0;
        b = 0;
        index = i * width;
        num = radius;

        for (int j = 0; j < radius; j++) {
            color = origin[index + j];
            r += qRed(color);
            g += qGreen(color);
            b += qBlue(color);
        }

        for (int j = 0; j <= radius; ++j) {
            num++;
            iarr = 1.0 / (1.0 * num);
            color = origin[index + j + radius];

            r += qRed(color);
            g += qGreen(color);
            b += qBlue(color);
            tr = int(r * iarr);
            tg = int(g * iarr);
            tb = int(b * iarr);

            dest[index + j] = qRgb(tr, tg, tb);
        }

        iarr = 1.0 / (1.0 * num);
        for (int j = radius + 1; j < width - radius; ++j) {
            preColor = origin[index + j - 1 - radius];
            color = origin[index + j + radius];

            r += qRed(color) - qRed(preColor);
            g += qGreen(color) - qGreen(preColor);
            b += qBlue(color) - qBlue(preColor);

            tr = int(r * iarr);
            tg = int(g * iarr);
            tb = int(b * iarr);

            dest[index + j] = qRgb(tr, tg, tb);
        }

        for (int j = width - radius; j < width; ++j) {
            num--;
            iarr = 1.0 / (1.0 * num);
            preColor = origin[index + j - 1 - radius];

            r -= qRed(preColor);
            g -= qGreen(preColor);
            b -= qBlue(preColor);

            tr = int(r * iarr);
            tg = int(g * iarr);
            tb = int(b * iarr);

            dest[index + j] = qRgb(tr, tg, tb);
        }
    }
#endif
}

void BoxForGauss::boxBlurV(uint32_t *origin, uint32_t *dest, int width, int height, int radius)
{
#if 0
    double iarr = 1.0 / (2 * radius + 1);
    for (int i = 0; i < width; ++i) {
        int ti = i, li = ti, ri = ti + radius * width;
        uint32_t fv = origin[ti], lv = origin[ti + width * (height - 1)], val = uint32_t(radius + 1) * fv;

        for (int j = 0; j < radius; ++j)
            val += origin[ti + j * width];

        for (int j = 0; j <= radius; ++j) {
            val += origin[ri] - fv;
            dest[ti] = uint32_t(qRound(val * iarr));
            ri += width;
            ti += width;
        }

        for (int j = radius + 1; j < height - radius; ++j) {
            val += origin[ri] - origin[li];
            dest[ti] = uint32_t(qRound(val * iarr));
            li += width;
            ri += width;
            ti += width;
        }

        for (int j = height - radius; j < height; ++j) {
            val += lv - origin[li];
            dest[ti] = uint32_t(qRound(val * iarr));
            li += width;
            ti += width;
        }

    }
#elif 1
    int r = 0, g = 0, b = 0;
    int tr, tg, tb;
    uint32_t color, preColor;
    int num;
    double iarr;

    for (int i = 0; i < width; ++i) {
        r = 0;
        g = 0;
        b = 0;
        num = radius;

        for (int j = 0; j < radius; ++j) {
            color = origin[j * width + i];
            r += qRed(color);
            g += qGreen(color);
            b += qBlue(color);
        }

        for (int j = 0; j <= radius; ++j) {
            num++;
            iarr = 1.0 / (1.0 * num);
            color = origin[(j + radius) * width + i];

            r += qRed(color);
            g += qGreen(color);
            b += qBlue(color);

            tr = int(r * iarr);
            tg = int(g * iarr);
            tb = int(b * iarr);

            dest[j * width + i] = qRgb(tr, tg, tb);
        }

        iarr = 1.0 / (1.0 * num);
        for (int j = radius + 1; j < height - radius; ++j) {
            preColor = origin[(j - radius - 1) * width + i];
            color = origin[(j + radius) * width + i];

            r += qRed(color) - qRed(preColor);
            g += qGreen(color) - qGreen(preColor);
            b += qBlue(color) - qBlue(preColor);

            tr = int(r * iarr);
            tg = int(g * iarr);
            tb = int(b * iarr);

            dest[j * width + i] = qRgb(tr, tg, tb);
        }

        for (int j = height - radius; j < height; ++j)
        {
            num--;
            iarr = 1.0 / (1.0 * num);
            preColor = origin[(j - radius - 1) * width + i];

            r -= qRed(preColor);
            g -= qGreen(preColor);
            b -= qBlue(preColor);

            tr = int(r * iarr);
            tg = int(g * iarr);
            tb = int(b * iarr);

            dest[j * width + i] = qRgb(tr, tg, tb);
        }
    }
#endif
}
