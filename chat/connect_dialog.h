#ifndef CONNECT_DIALOG_H
#define CONNECT_DIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
    class connect_dialog;
}

class connect_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit connect_dialog(QWidget *parent = 0);
    ~connect_dialog();
    
private:
    Ui::connect_dialog *ui;

private slots:
    void on_buttonBox_accepted();
};

#endif // CONNECT_DIALOG_H