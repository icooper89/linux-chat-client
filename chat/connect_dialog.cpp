#include "connect_dialog.h"
#include "ui_connect_dialog.h"
#include "mainwindow.h"

connect_dialog::connect_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::connect_dialog)
{
    ui->setupUi(this);

}

connect_dialog::~connect_dialog()
{
    delete ui;
}

void connect_dialog::on_buttonBox_accepted()
{

}