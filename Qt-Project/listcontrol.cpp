#include "listcontrol.h"
#include "ui_listcontrol.h"
#include <QHBoxLayout>
#include <QPixmap>
#include <QDebug>
#include <QEvent>
#include<QMouseEvent>

//delete pictures
ListControl::ListControl(int index, const QString &name, const QImage &img, int size, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::ListControl),
    _name(name),
    _img(img),
    _size(size)
{
    ui->setupUi(this);

    this->installEventFilter(this);
    foreach (QObject* child, this->children()) {
        if (QWidget* widget = qobject_cast<QWidget*>(child)) {
            widget->installEventFilter(this);
        }
    }

    qDebug() << ui->nameLabel;
    ui->nameLabel->setText(name);
    ui->indexLabel->setText(QString::number(index));
    ui->imgLabel->setPixmap(QPixmap::fromImage(img.scaled(ui->imgLabel->size())));
}

ListControl::~ListControl()
{
    delete ui;
}

// check click
bool ListControl::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            emit clicked(this);
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}

void ListControl::on_pushButton_clicked()
{
    emit deleteSignal(this);
}

