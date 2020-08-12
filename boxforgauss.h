#ifndef BOXFORGAUSS_H
#define BOXFORGAUSS_H

#include <QObject>

class BoxForGauss : public QObject
{
    Q_OBJECT
public:
    explicit BoxForGauss(QObject *parent = nullptr);
    void gaussImage(uint32_t *origin, int width, int height, int radius);

protected:
    QVector<int> boxesForGauss(double sigma, int size);
    void boxBlur(uint32_t *origin, uint32_t *dest, int width, int height, int radius);
    void boxBlurH(uint32_t *origin, uint32_t *dest, int width, int height, int radius);
    void boxBlurV(uint32_t *origin, uint32_t *dest, int width, int height, int radius);
signals:

public slots:
};

#endif // BOXFORGAUSS_H
