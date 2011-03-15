#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectdialog.h"
#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include "ui_connectdialog.h"
#include <QDialog>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


char* ip;
int port;
bool save;
QString username;
int saveFile;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}



void MainWindow::on_actionExit_triggered()
{
    exit(1);
}

void MainWindow::on_actionConnect_triggered()
{
    ConnectDialog dialog(this);
    QByteArray temp;

    if(dialog.exec()){
        temp = dialog.ui->lineEdit->text().toAscii();
        ip = temp.data();
        port = dialog.ui->lineEdit_2->text().toInt();
    }
}

void MainWindow::on_actionOptions_triggered()
{
    OptionsDialog dialog(this);
    QByteArray temp;
    char* saveName;


    if(dialog.exec()){
        save = dialog.ui->save->isChecked();
        username = dialog.ui->username->text();
        temp = dialog.ui->filename->text().toLatin1();
        saveName = temp.data();
        if(save){
            saveFile = open(saveName, O_CREAT);
        }
    }

}
