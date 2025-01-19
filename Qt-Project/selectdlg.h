#ifndef SELECTDLG_H
#define SELECTDLG_H

#include <QDialog>

namespace Ui {
class SelectDlg;
}

class SelectDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SelectDlg(QWidget *parent = nullptr);
    ~SelectDlg();
signals:
    void selectedSquare(int size,int type);
    void cancleSelected();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::SelectDlg *ui;
};

#endif // SELECTDLG_H
