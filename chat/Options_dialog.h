#ifndef OPTIONS_DIALOG_H
#define OPTIONS_DIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
    class Options_dialog;
}

class Options_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Options_dialog(QWidget *parent = 0);
    ~Options_dialog();
    
private:
    Ui::Options_dialog *ui;

private slots:
    void on_buttonBox_accepted();
};

#endif // OPTIONS_DIALOG_H