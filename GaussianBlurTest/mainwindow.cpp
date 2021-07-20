#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    img=new QImage("D:\\我的文档\\Images\\back1.jpeg");
    g=new Gaussian_blur(12,4);
    g->Load_Image("D:\\我的文档\\Images\\back1.jpeg");
    ui->label->setScaledContents(true);
    ui->label_2->setScaledContents(true);
    ui->label->setPixmap(QPixmap::fromImage(*img));
    connect(g,SIGNAL(finished(QImage*)),this,SLOT(labelLoadImg(QImage*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    bool ok;
    qint32 r=ui->textEdit->toPlainText().toInt(&ok,10);
    g->setRadiusAndSigma(r,r/3);
    std::thread([this]{this->g->Start_Blur();}).detach();
}
QImage *lastImage;
void MainWindow::labelLoadImg(QImage *_img){
    ui->label_2->setPixmap(QPixmap::fromImage(*_img));
    if(lastImage){
        delete lastImage;
    }
    lastImage=_img;
}
