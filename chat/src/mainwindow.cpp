#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectdialog.h"
#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include "ui_connectdialog.h"
#include "client.h"
#include <QDialog>
#include <qmessagebox.h>



char* ip;
int port;
bool save;

int saveFile;
int sd;
PCINFO info;

PMYSOCKET mySocket;
bool connected;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    info = (PCINFO)malloc(sizeof(PCINFO));

    mySocket = (PMYSOCKET)malloc(sizeof(PMYSOCKET));
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
        strcpy(info->hostname, temp.data());
        mySocket->port = dialog.ui->lineEdit_2->text().toInt();
        if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
            printf("error creating socket");
        } else {
            mySocket->socket = sd;
        }
        info->id = 0;
        connectToServer(mySocket, info);
    }
}

void MainWindow::on_actionOptions_triggered()
{
    OptionsDialog dialog(this);
    QByteArray temp;
    char* saveName;


    if(dialog.exec()){
        save = dialog.ui->save->isChecked();
        temp = dialog.ui->username->text().toAscii();
        strcpy(info->username, temp.data());
        temp = dialog.ui->filename->text().toLatin1();
        saveName = temp.data();
        if(save){
            saveFile = open(saveName, O_CREAT);
        }
    }

}

void MainWindow::on_sendButton_clicked()
{
    char buffer[BUFLEN];
    info->id = 0;
    strcpy(buffer, ui->send->text().toLatin1());
    ui->display->append(ui->send->text());
    sendPacket(buffer, mySocket, info);
    ui->send->clear();
}

void MainWindow::on_send_returnPressed()
{
    MainWindow::on_sendButton_clicked();
}
