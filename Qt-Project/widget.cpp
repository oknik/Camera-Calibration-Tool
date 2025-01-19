#include "widget.h"
#include "ui_widget.h"

#include "CalculateCorner.h"
#include "PointMapper.h"
#include "CameraCalibrate.h"
#include "ImageUndistort.h"
#include "Tools.h"
#include <QDebug>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    CalculateCorner * calc = new CalculateCorner();
    calc->AddImagePath("");

    PointMapper pointMapper;
    pointMapper.SetSquareSize(5.0f);
    pointMapper.SetSquareSize(10.0f);
    pointMapper.SetBoardSize(cv::Size(8, 6));
    pointMapper.SetBoardType(CHESSBOARD);

    qDebug()<<pointMapper.GetSquareSize() ;

    Tools tool;
    cv::Size size = tool.GetImageSize("D:/图片/test1.png");
    qDebug()<<size.width<<'-'<<size.height;


    ui->lineEdit->setText("ddd");

}

Widget::~Widget()
{
    delete ui;
}

