#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<PointMapper.h>
#include"CameraCalibrate.h"
#include"listcontrol.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_radioButton_3_toggled(bool checked);

    void on_radioButton_4_clicked(bool checked);

    void on_pushButton_6_clicked();

    void on_checkBox_toggled(bool checked);

    void processCapturedImage(int id, const QImage &preview);


private:

    ListControl*_curListItem=nullptr;
    PointMapper*pointMapper;
    cv::Size imgSize;
    CameraCalibrate cc;
    CalculateCorner * calc ;
    bool _ifK2=true;//2 coefficients
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
