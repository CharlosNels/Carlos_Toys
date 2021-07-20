#ifndef GAUSSIAN_BLUR_H
#define GAUSSIAN_BLUR_H

#include <QImage>
#include <QtMath>
#include <QWidget>
#include <QtGui>
#include <QtCore>
#include <QMessageBox>
#include <thread>

class Gaussian_blur: public QWidget
{
Q_OBJECT
private:
    qint32 radius;
    qreal sigma;
    QImage *image;
    qreal *weightArray;
    qreal **resTable;
    void InitWeight();
    void InitTable();
    void edge(qint32 &_x,const qint32 &_w);
public:
    ~Gaussian_blur();
    Gaussian_blur(const qint32 &_radius,const qreal &_sigma);
    void Load_Image(const QString &fileName);
    void setRadiusAndSigma(const qint32 &_radius,const qreal &_sigma);
    void Start_Blur();
signals:
    void finished(QImage *img);
};

#endif // GAUSSIAN_BLUR_H
