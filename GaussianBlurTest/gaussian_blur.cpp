#include "gaussian_blur.h"


Gaussian_blur::Gaussian_blur(const qint32 &_radius,const qreal &_sigma):radius(_radius),sigma(_sigma),image(nullptr)
{
    InitWeight();
}

void Gaussian_blur::InitWeight(){
    weightArray=new qreal[2*radius+1];
    qreal coefficient=1/sigma/qSqrt(2*M_PI);        //正态函数系数
    qreal m2Sigma2=2*sigma*sigma;
    qreal sum=0;
    for(qint32 i=0;i<2*radius+1;i++){
        qint32 x=i-radius;         //距中心点距离
        weightArray[i]=coefficient*qPow(M_E,-(x*x)/m2Sigma2);
        sum+=weightArray[i];
    }
    for(qint32 i=0;i<2*radius+1;i++){       //归一化
        weightArray[i]/=sum;
    }
    InitTable();
}

void Gaussian_blur::InitTable(){
    resTable=new qreal*[256];
    for(int i=0;i<256;i++){
        resTable[i]=new qreal[2*radius+1];
        for(int j=0;j<2*radius+1;j++){
            resTable[i][j]=i*weightArray[j];
        }
    }
}

void Gaussian_blur::setRadiusAndSigma(const qint32 &_radius,const qreal &_sigma){
    radius=_radius;
    sigma=_sigma;
    delete[] weightArray;
    for(int i=0;i<256;i++){
        delete[] resTable[i];
    }
    delete[] resTable;
    InitWeight();
}

void Gaussian_blur::edge(qint32 &_x, const qint32 &_w){
    if(_x<0) _x=_w+_x;
    if(_x>=_w) _x-=_w;
}

void Gaussian_blur::Start_Blur(){
    uchar *data=image->bits();
    uchar *tempData=new uchar[image->sizeInBytes()];  //横向模糊的数据
    uchar *finalData=new uchar[image->sizeInBytes()]; //纵向模糊的数据
    for(qint32 x=0;x<image->width();x++){   //横向模糊
        for(qint32 y=0;y<image->height();y++){
            qreal r=0,g=0,b=0;
            for(qint32 i=-radius;i<=radius;i++){
                qint32 _x=x+i;
                edge(_x,image->width());
                qint32 index=_x*4+y*image->width()*4;
                r+= resTable[data[index+2]][i+radius];
                g+= resTable[data[index+1]][i+radius];
                b+= resTable[data[index]][i+radius];
            }
            qint32 indx=x*4+y*image->width()*4;
            tempData[indx+2] = (uchar)r;
            tempData[indx+1] = (uchar)g;
            tempData[indx] = (uchar)b;
        }
    }
    for(qint32 x=0;x<image->width();x++){  //纵向模糊
        for(qint32 y=0;y<image->height();y++) {
            qreal r=0,g=0,b=0;
            for(qint32 i=-radius;i<=radius;i++) {
                qint32 _y=y+i;
                edge(_y,image->height());
                qint32 index=x*4+_y*image->width()*4;
                r += resTable[tempData[index+2]][i+radius];
                g += resTable[tempData[index+1]][i+radius];
                b += resTable[tempData[index]][i+radius];
            }
            qint32 indx=x*4+y*image->width()*4;
            finalData[indx+2]=(uchar)r;
            finalData[indx+1]=(uchar)g;
            finalData[indx]=(uchar)b;
        }
    }
    QImage *res=new QImage(finalData,image->width(),image->height(),image->format(),
                           [](void *arr){delete[] (uchar*)arr;},finalData);
    delete[] tempData;
    emit finished(res);
}

Gaussian_blur::~Gaussian_blur(){
    if(image){
        delete image;
        image=nullptr;
    }
    if(weightArray){
        delete[] weightArray;
        weightArray=nullptr;
    }
}

void Gaussian_blur::Load_Image(const QString &fileName){
    if(image){
        delete image;
    }
    image=new QImage(fileName);
}

