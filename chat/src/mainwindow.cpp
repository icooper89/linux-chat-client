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
#include <QMetaType>



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

    qRegisterMetaType<PPACKET>("PPACKET");

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
            PPACKET tempPacket;
            tempPacket = (PPACKET)malloc(sizeof(PACKET));
            tempPacket->type = MSG_NEW;
            tempPacket->owner = info->id;
            memcpy(tempPacket->data, info, BUFLEN);
            ClientThread *thread = new ClientThread(mySocket);
            connect(thread,SIGNAL(parsePacketSig(PPACKET)),SLOT(parsePacket(PPACKET)));
            thread->start();
            if(strcmp(info->username, "") != 0){
                send(mySocket->socket, (char*) tempPacket, PACKETSIZE, NULL);
            }
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
        /*if(save){
            QTextStream out(saveFile);

           out << buffer;
            //i = write(saveFile, buffer, strlen(buffer));
           //perror("write");
        }*/
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

       out << text.toLatin1().append("\n");
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
        if(strcmp(otherClients[id]->username, "") == 0){
            return otherClients[id]->hostname;
        }
        return otherClients[id]->username;
    }
    else return NULL;
}



void MainWindow::parsePacket(PPACKET packet){
    switch(packet->type){
        case MSG_TEXT:{
            //send data to window/log
            QString buf;
            QString name;

            buf += getClientName(packet->owner);
            buf += ": ";
            buf += packet->data;
            addToDisplay(buf);
            break;
        }
    case MSG_NEW:{
            //add client info
            PCINFO temp = (PCINFO)packet->data;
            addClient(temp->id,temp);
            break;
        }
    case MSG_REM:{
            //remove client from list
            //PCINFO temp = (PCINFO)packet->data;
            //remClient(temp->id);
            break;
        }
    }
}
