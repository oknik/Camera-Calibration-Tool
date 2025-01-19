#include "selectdlg.h"
#include "ui_selectdlg.h"

SelectDlg::SelectDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SelectDlg)
{
    ui->setupUi(this);
}

SelectDlg::~SelectDlg()
{
    delete ui;
}


//confirm
void SelectDlg::on_pushButton_clicked()
{
    emit selectedSquare(ui->spinBox->value(),ui->comboBox->currentIndex());
    close();
}

//cancel
void SelectDlg::on_pushButton_2_clicked()
{
    emit cancleSelected();
    close();
}

