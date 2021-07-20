#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gaussian_blur.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();
public slots:
    void labelLoadImg(QImage *);
private slots:
    void on_pushButton_clicked();

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QImage *img;
    Gaussian_blur *g;
};

#endif // MAINWINDOW_H
