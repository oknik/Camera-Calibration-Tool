#ifndef LISTWIDGET_H
#define LISTWIDGET_H
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include <QWidget>
#include <QVBoxLayout>
#include<QJsonArray>
#include<QSpacerItem>
#include"listcontrol.h"
namespace Ui {
class ListWidget;
}

class ListWidget : public QWidget
{
    Q_OBJECT

public:
    void getData(std::vector<std::vector<cv::Point3f>>&worldPoints,std::vector<std::vector<cv::Point2f>>&imagePoints);
    explicit ListWidget(QWidget *parent = 0);
    ~ListWidget();
    void clear();
    void addImage(const QImage&unimg,const QImage&img,const QString&name,int size,const std::vector<cv::Point2f>&corners,const std::vector<cv::Point3f>&realWorldPoints);
signals:
    void itemClicked(ListControl*item);
private:
    void initValue();
private:
    QList<ListControl*>_controls;
    QSpacerItem*strech=nullptr;
    Ui::ListWidget *ui;
    QVBoxLayout *m_vboxLayout = nullptr;
};

#endif // LISTWIDGET_H
