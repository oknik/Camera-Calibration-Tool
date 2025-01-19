#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<QMessageBox>
#include<QDebug>
#include<QLabel>
#include<QFileInfo>
#include"selectdlg.h"
#include"ImageUndistort.h"
#include<PointMapper.h>
#include"include/CalibrationOptions.h"
#include"include/CameraCalibrate.h"
#include"Tools.h"
#include<iostream>
#include <direct.h>
#include <filesystem>
#include <vector>


// change QImage to cv:Mat
cv::Mat qImageToCvMat(const QImage &image) {
    // 8-bit RGB image or Grayscale image
    switch (image.format()) {
    case QImage::Format_RGB888:
        return cv::Mat(image.height(), image.width(), CV_8UC3, const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();
    case QImage::Format_Grayscale8:
        return cv::Mat(image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();
    default:
        QImage converted = image.convertToFormat(QImage::Format_RGB888);
        return cv::Mat(converted.height(), converted.width(), CV_8UC3, const_cast<uchar*>(converted.bits()), converted.bytesPerLine()).clone();
    }
}

// change cv:Mat to QImage
QImage cvMatToQImage(const cv::Mat &mat) {
    if (mat.type() == CV_8UC1) {
        // 8-bit grayscale image
        QImage img(mat.cols, mat.rows, QImage::Format_Grayscale8);
        uchar *pImg = img.bits();
        memcpy(pImg, mat.data, static_cast<size_t>(img.byteCount()));
        return img;
    } else if (mat.type() == CV_8UC3) {
        // 8-bit color image in BGR format (OpenCV's default)
        QImage img(mat.cols, mat.rows, QImage::Format_RGB888);
        uchar *pImg = img.bits();
        for (int i = 0; i < mat.rows; ++i) {
            const uchar* pSrcRow = mat.ptr<uchar>(i);
            uchar* pDstRow = pImg + i * img.bytesPerLine();
            for (int j = 0; j < mat.cols; ++j) {
                // Convert from BGR to RGB
                pDstRow[j * 3]     = pSrcRow[j * 3 + 2]; // R
                pDstRow[j * 3 + 1] = pSrcRow[j * 3 + 1]; // G
                pDstRow[j * 3 + 2] = pSrcRow[j * 3];     // B
            }
        }
        return img;
    } else {
        // Unsupported format
        return QImage();
    }
}

QImage undistortImage(const QImage &inputImage, const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs, bool isFishEye) {
    // first change to cv:Mat
    cv::Mat distortedImage = qImageToCvMat(inputImage);

    // create ImageUndistort instance and undistort
    ImageUndistort undistorter(cameraMatrix, distCoeffs);
    undistorter.Undistort(distortedImage, isFishEye);
    cv::Mat undistortedImage = undistorter.GetUndistortImage();

    // change cv:Mat to QImage
    return cvMatToQImage(undistortedImage);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    ui->radioButton_3->setChecked(true);
    ui->radioButton->toggle();
    connect(ui->listWidget,&ListWidget::itemClicked,this,[this](ListControl*item){
        _curListItem=item;
        ui->tabWidget->clear();
        QLabel*img=new QLabel(this);
        img->setPixmap(QPixmap::fromImage(item->_img).scaled(ui->tabWidget->size()*0.9));
        ui->tabWidget->addTab(img,item->_name);
    });
    calc = new CalculateCorner();
    calc->SetBoardSize(cv::Size(8, 6));
    setWindowIcon(QIcon(":/img/icon.png"));
    pointMapper=new PointMapper();
    pointMapper->SetBoardSize(cv::Size(8, 6));
    pointMapper->SetBoardType(CHESSBOARD);
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete pointMapper;
    delete calc;
}

// upload images button
void MainWindow::on_pushButton_clicked()
{
    // image file types
    QString filter = "Image files (*.jpg *.jpeg *.png);;JPEG files (*.jpg *.jpeg);;PNG files (*.png)";

    // choose images
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
                                                          tr("Open Images"), //title
                                                          "",               //the default directory
                                                          filter,           //file filter
                                                          nullptr);         //the default filter is null

    if (!fileNames.isEmpty())
    {
        SelectDlg dlg;
        imgSize=Tools::GetImageSize(fileNames[0].toLocal8Bit().toStdString());
        //int index=ui->comboBox->currentIndex();

        bool isFishEye=ui->radioButton_4->isChecked();
        connect(&dlg, &SelectDlg::selectedSquare, this, [&](int size,int index) {
            switch(index){
            case 0:
                pointMapper->SetBoardType(BoardType::CHESSBOARD);
                break;
            case 1:
                pointMapper->SetBoardType(BoardType::ASYMMETRIC_CIRCLE_GRID);
                break;
            case 2:
                pointMapper->SetBoardType(BoardType::SYMMETRIC_CIRCLE_GRID);
                break;
            }
            pointMapper->SetSquareSize(size);
            foreach (const QString &fileName, fileNames) {
                // detect corner points
                std::string stdFileName = fileName.toLocal8Bit().toStdString();
                calc->CalculateSingleImageCorners(stdFileName, true, false);
                auto processedImages =  calc->GetProcessedImageByPath();
                std::vector<cv::Point2f> corners= calc->GetAllImageCorners().front();

                // map real world points
                auto realworldPoints=pointMapper->GenerateRealWorldPoints();

                // undistortion
                if (processedImages.find(stdFileName) != processedImages.end()) {
                    cv::Mat img = processedImages.at(stdFileName);

                    QImage qImg((const unsigned char*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
                    qImg = qImg.rgbSwapped();
                    // ImageUndistort undist(cc.GetCameraMatrix(),cc.GetDistCoeffs());
                    // QImage rawImg(fileName);

                    // undist.Undistort(qImageToCvMat(rawImg),isFishEye);

                    // add to listWidget
                    //ui->listWidget->addImage(cvMatToQImage(undist.GetUndistortImage()),qImg, QFileInfo(fileName).fileName(), size,corners,realworldPoints);
                    ui->listWidget->addImage(qImg,qImg, QFileInfo(fileName).fileName(), size,corners,realworldPoints);
                } else {
                    qDebug() << "Failed to process image:" << fileName;
                    QMessageBox::warning(this, tr("Error"), tr("无法处理图像: %1").arg(fileName));
                }

            }
        });

        connect(&dlg, &SelectDlg::cancleSelected, this, [&]() {
            qDebug() << "Selection cancelled.";
        });

        dlg.exec();
    }
}

// calibrate button
void MainWindow::on_pushButton_2_clicked()
{
    bool standard=ui->radioButton_3->isChecked();
    bool fisheye=ui->radioButton_4->isChecked();
    bool coe2=ui->radioButton->isChecked();
    bool coe3=ui->radioButton_2->isChecked();
    bool skew=ui->checkBox_2->isChecked();
    bool tagentialDistortion=ui->checkBox->isChecked();

    if(!(standard||fisheye)){
        QMessageBox::warning(this,"error","未选择Camera Model!");
    }
    if(!(coe2||coe3)){
        QMessageBox::warning(this,"error","未选择Radial Distortion!");
    }
    /*
    if(!(skew||tagentialDistortion)){
        QMessageBox::warning(this,"error","未选择Compute!");
    }
    */
    RadialDistortionType rdt=coe2?RadialDistortionType::TWO_COEFFICIENTS:RadialDistortionType::THREE_COEFFICIENTS;
    CalibrationOptions co;
    co.RadialDistortion=rdt;
    co.SkewEnabled=skew;
    co.UseTangentialDistortion=tagentialDistortion;
    //calibrate
    std::vector<std::vector<cv::Point3f> > worldPoints;
    std::vector<std::vector<cv::Point2f> > imagePoints;
    ui->listWidget->getData(worldPoints,imagePoints);
    if(standard){
        cc.CalibrateStandardCamera(worldPoints,imagePoints,imgSize,co);
    }
    else{
        cc.CalibrateFishEyeCamera(worldPoints,imagePoints,imgSize,co);
    }
}

// export button
void MainWindow::on_pushButton_3_clicked()
{
    // jump to parameter window
    ui->plainTextEdit->clear();
    ui->stackedWidget->setCurrentIndex(1);
    // show parameters
    QString path=QDir::currentPath();
    std::string tmpPath=path.toLocal8Bit().toStdString()+"/tmp.txt";
    cc.ExportCameraParameters(tmpPath, imgSize);
    QFile file(path+"/tmp.txt");
    if(file.open(QFile::ReadOnly)){
        QString data=file.readAll();
        ui->plainTextEdit->appendPlainText(data);
        file.close();
    }
    else{
        return;
    }
}

// change page button, return to images from parameters
void MainWindow::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

//control normal/fisheye camera options view
void MainWindow::on_radioButton_3_toggled(bool checked)
{
    if(checked)
        ui->stackedWidget_2->setCurrentIndex(0);
}

//control normal/fisheye camera options view
void MainWindow::on_radioButton_4_clicked(bool checked)
{
    if(checked)
        ui->stackedWidget_2->setCurrentIndex(1);
}

//take photos button
void MainWindow::on_pushButton_6_clicked()
{
    cv::VideoCapture Camera(0);
    if (!Camera.isOpened()) {
        std::cerr << "Error: unable to open camera" << std::endl;
        return;
    }

    cv::Mat CapturedImage;
    int imageCount = 0;
    std::vector<std::string> fileNames;
    std::string currentDirectory = std::filesystem::current_path().string();
    std::ostringstream folderName;
    folderName << currentDirectory << "/captured_images_" << std::time(nullptr);
    if (std::filesystem::create_directory(folderName.str())) {
        std::cout << "Directory created: " << folderName.str() << std::endl;
    } else {
        std::cerr << "Error: Unable to create directory" << std::endl;
    }

    while (true) {
        Camera >> CapturedImage;
        if (CapturedImage.empty()) {
            std::cerr << "Error: captured frame is empty" << std::endl;
            return;
            break;
        }
        cv::flip(CapturedImage, CapturedImage, 1);
        cv::imshow("Camera", CapturedImage);

        char key = static_cast<char>(cv::waitKey(10));

        if (key == 27) {
            std::cout << "Exiting capture image" << std::endl;
            Camera.release();
            cv::destroyAllWindows();
            break;
        }
        if (key == 32) {
            std::cout << "Space key pressed, saving image..." << std::endl;
            std::ostringstream fileName;
            fileName << folderName.str() << "/captured_image_" << imageCount++ << ".jpg";
            if (cv::imwrite(fileName.str(), CapturedImage)) {
                std::cout << "Image saved successfully as " << fileName.str() << std::endl;
                fileNames.push_back(fileName.str());
            } else {
                std::cerr << "Error: Unable to save image: " << std::endl;
                break;
            }
        }
    }

    if (!fileNames.empty()) {
        cv::Mat firstImage = cv::imread(fileNames[0]);
        if (firstImage.empty()) {
            std::cerr << "Error: Unable to load image from file " << fileNames[0] << std::endl;
            return;
        }

        cv::Size imgSize = firstImage.size();
        std::cout << "Image size (width x height): " << imgSize.width << " x " << imgSize.height << std::endl;
    }
    //the same to upload
    if (!fileNames.empty())
    {
        SelectDlg dlg;
        //imgSize=Tools::GetImageSize(fileNames[0].toLocal8Bit().toStdString());
        //int index=ui->comboBox->currentIndex();

        bool isFishEye=ui->radioButton_4->isChecked();
        connect(&dlg, &SelectDlg::selectedSquare, this, [&](int size,int index) {
            switch(index){
            case 0:
                pointMapper->SetBoardType(BoardType::CHESSBOARD);
                break;
            case 1:
                pointMapper->SetBoardType(BoardType::ASYMMETRIC_CIRCLE_GRID);
                break;
            case 2:
                pointMapper->SetBoardType(BoardType::SYMMETRIC_CIRCLE_GRID);
                break;
            }
            pointMapper->SetSquareSize(size);
            for (const std::string &fileName : fileNames) {
                std::string stdFileName = fileName;
                calc->CalculateSingleImageCorners(stdFileName, true, false);
                auto processedImages =  calc->GetProcessedImageByPath();
                std::vector<cv::Point2f> corners= calc->GetAllImageCorners().front();

                auto realworldPoints=pointMapper->GenerateRealWorldPoints();
                if (processedImages.find(stdFileName) != processedImages.end()) {
                    cv::Mat img = processedImages.at(stdFileName);

                    QImage qImg((const unsigned char*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
                    qImg = qImg.rgbSwapped();
                    // ImageUndistort undist(cc.GetCameraMatrix(),cc.GetDistCoeffs());
                    // QImage rawImg(fileName);

                    // undist.Undistort(qImageToCvMat(rawImg),isFishEye);


                    //ui->listWidget->addImage(cvMatToQImage(undist.GetUndistortImage()),qImg, QFileInfo(fileName).fileName(), size,corners,realworldPoints);
                    ui->listWidget->addImage(qImg,qImg, QFileInfo(QString::fromStdString(fileName)).fileName(), size, corners, realworldPoints);
                } else {
                    qDebug() << "Failed to process image:" << QString::fromStdString(fileName);
                    QMessageBox::warning(this, tr("Error"), tr("无法处理图像: %1").arg(QString::fromStdString(fileName)));
                }

            }
        });

        connect(&dlg, &SelectDlg::cancleSelected, this, [&]() {
            qDebug() << "Selection cancelled.";
        });

        dlg.exec();
    }
}

// show undistortion
void MainWindow::on_checkBox_toggled(bool checked)
{
    if(!checked){
        if(ui->tabWidget->count()>0){
            QLabel*label=(QLabel*)ui->tabWidget->widget(0);
            label->setPixmap(QPixmap::fromImage(_curListItem->_img));
        }
    }
    else{
        if(ui->tabWidget->count()>0){
            QLabel*label=(QLabel*)ui->tabWidget->widget(0);
            label->setPixmap(QPixmap::fromImage(_curListItem->_undisImg));
        }
    }
}

// show taken photos
void MainWindow::processCapturedImage(int id, const QImage &preview) {
    // show taken photos
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("Images (*.png *.xpm *.jpg)"));
    if (!filePath.isEmpty()) {
        preview.save(filePath);
        QMessageBox::information(this, tr("成功"), tr("照片已保存！"));
    }
}
