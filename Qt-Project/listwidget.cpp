#include "listwidget.h"
#include<QJsonObject>
#include<QJsonValue>
#include"listcontrol.h"
#include"ui_listwidget.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

void ListWidget::getData(std::vector<std::vector<cv::Point3f> > &worldPoints, std::vector<std::vector<cv::Point2f> > &imagePoints)
{
    for(auto&item:_controls)
    {
        worldPoints.push_back(item->realWorldPoints);
        imagePoints.push_back(item->corners);
    }
}

ListWidget::ListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListWidget)
{
    ui->setupUi(this);
    this->initValue();
    strech = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    m_vboxLayout->addItem(strech);
}

ListWidget::~ListWidget()
{
    clear();
    delete ui;
}

void ListWidget::clear()
{
    QLayoutItem*item=nullptr;
    while ((item = m_vboxLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}


void ListWidget::addImage(const QImage&unimg,const QImage&img, const QString &name,int size,const std::vector<cv::Point2f>&corners,const std::vector<cv::Point3f>&realWorldPoints)
{
    m_vboxLayout->removeItem(strech);

    ListControl*listControl=new ListControl(this->_controls.size()+1,name,img,size,this);
    listControl->_undisImg=unimg;
    listControl->corners=corners;
    listControl->realWorldPoints=realWorldPoints;
    connect(listControl,&ListControl::clicked,this,[this](ListControl*item){
        emit itemClicked(item);
    });
    connect(listControl,&ListControl::deleteSignal,this,[this](ListControl*item){
        m_vboxLayout->removeWidget(item);
        _controls.removeAll(item);
        delete item;
    });
    _controls.push_back(listControl);
    m_vboxLayout->addWidget(listControl);
    m_vboxLayout->addItem(strech);
}

void ListWidget::initValue()
{
    m_vboxLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    m_vboxLayout->setSpacing(0);
}
