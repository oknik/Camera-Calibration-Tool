#ifndef LISTCONTROL_H
#define LISTCONTROL_H

#include <QWidget>
#include <QImage>
#include <QLabel>
#include"opencv2/opencv.hpp"
#include<vector>
namespace Ui {
class ListControl;
}

class ListControl : public QWidget
{
    Q_OBJECT

public:
    explicit ListControl(int index, const QString& name, const QImage& img, int size, QWidget* parent = nullptr);
    ~ListControl();
signals:
    void clicked(ListControl*item);
    void deleteSignal(ListControl*item);
protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    Ui::ListControl *ui;
public:
    std::vector<cv::Point3f> realWorldPoints;
    std::vector<cv::Point2f>corners;
    int _size;
    QString _name;
    QImage _img;
    QImage _undisImg;
private slots:
    void on_pushButton_clicked();
};

#endif // LISTCONTROL_H
