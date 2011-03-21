#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectdialog.h"
#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include "ui_connectdialog.h"
#include "client.h"
#include <QDialog>
#include <qmessagebox.h>
#include <QFile>
#include <QTextStream>



bool save;
QFile* saveFile;
int sd;
PCINFO info;
PCINFO* otherClients;

PMYSOCKET mySocket;
bool connected;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    info = (PCINFO)malloc(sizeof(PCINFO));
    otherClients = (PCINFO*) calloc (sizeof(PCINFO),MAXCLIENTS);

    mySocket = (PMYSOCKET)malloc(sizeof(PMYSOCKET));
    info->id = -1;
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
    cleanup(mySocket, info,NULL);// saveFile);
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
        if(connectToServer(mySocket, info)){
            connected = true;
            ClientThread *thread = new ClientThread(mySocket);
            thread->start();

        }
    }
}

void MainWindow::on_actionOptions_triggered()
{
    OptionsDialog dialog(this);
    QByteArray temp;
    char* saveName;
    //QFile saveFile_;


    if(dialog.exec()){
        save = dialog.ui->save->isChecked();
        temp = dialog.ui->username->text().toAscii();
        strcpy(info->username, temp.data());
        temp = dialog.ui->filename->text().toLatin1();
        saveName = temp.data();
        if(save){
            saveFile=new QFile(saveName); // open(saveName, O_CREAT|O_WRONLY);
            saveFile->open(QIODevice::WriteOnly | QIODevice::Text);
            //perror("open");
        }
    }

}

void MainWindow::on_sendButton_clicked()
{
    char buffer[BUFLEN];
    int i;
    //if(connected){
        strcpy(buffer, "Me: ");
        strcat(buffer, ui->send->text().toLatin1());
        
        //ui->display->append(buffer);
        addToDisplay(buffer);
        if(save){
            QTextStream out(saveFile);

           out << buffer;
            //i = write(saveFile, buffer, strlen(buffer));
           //perror("write");
        }
        sendPacket(ui->send->text().toLatin1().data(), mySocket, info);
    //}
    ui->send->clear();
}

void MainWindow::on_send_returnPressed()
{
    MainWindow::on_sendButton_clicked();
}

void MainWindow::addToDisplay(QString text){
    ui->display->append(text);
    if(save){
        QTextStream out(saveFile);

       out << text.toLatin1();
        //write(saveFile, text.toLatin1(), text.length());
    }
    
}

void MainWindow::addClient(int id, PCINFO data){
    otherClients[id] = data;
    
}
void MainWindow::remClient(int id){
    
    otherClients[id] = NULL;
    
}
QString MainWindow::getClientName(int id){
    if(otherClients[id] != NULL){
        return otherClients[id]->username;
    }
    else return NULL;
}
