#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include "QFileDialog"

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::on_pushButton_clicked()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(0, "Chat Log Location");
    ui->filename->setText(filename);
}
