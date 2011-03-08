#include "Options_dialog.h"
#include "ui_Options_dialog.h"
#include "mainwindow.h"

Options_dialog::Options_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options_dialog)
{
    ui->setupUi(this);

}

Options_dialog::~Options_dialog()
{
    delete ui;
}

void Options_dialog::on_buttonBox_accepted()
{

}