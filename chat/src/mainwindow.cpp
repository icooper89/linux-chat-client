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


/*----------------------------------------------------------------------------
MainWindow

Constructor.
----------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------
~MainWindow

Destructor.
----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------
on_actionExit_triggered

slot for menu selection: Exit
----------------------------------------------------------------------------*/
void MainWindow::on_actionExit_triggered()
{
    cleanup(mySocket, info,NULL);// saveFile);
    exit(1);
}
/*----------------------------------------------------------------------------
on_actionConnect_triggered

slot for menu selection: Connect
opens dialog box
then connects to server
then starts read thread
----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------
on_actionOptions_triggered

slot for menu selection: Options

can specify username and whether or not want to save chat log.
----------------------------------------------------------------------------*/

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
/*----------------------------------------------------------------------------
on_sendButton_clicked

slot for send button being clicked
will get and clear text from text box
write text to display window and then send packet to server with text msg
----------------------------------------------------------------------------*/
void MainWindow::on_sendButton_clicked()
{
    char buffer[BUFLEN];

    if(connected){
        strcpy(buffer, "Me: ");
        strcat(buffer, ui->send->text().toLatin1());
        addToDisplay(buffer);

        sendPacket(ui->send->text().toLatin1().data(), mySocket, info);
    }
    ui->send->clear();
}
/*----------------------------------------------------------------------------
on_send_returnPressed

slot for pressing return button when send text box has focus.
----------------------------------------------------------------------------*/

void MainWindow::on_send_returnPressed()
{
    MainWindow::on_sendButton_clicked();
}

/*----------------------------------------------------------------------------
addToDisplay

adds text to display window and to log file if enabled.
----------------------------------------------------------------------------*/
void MainWindow::addToDisplay(QString text){
    ui->display->append(text);
    if(save){
        QTextStream out(saveFile);

       out << text.toLatin1().append("\n");
        //write(saveFile, text.toLatin1(), text.length());
    }
    
}
/*----------------------------------------------------------------------------
addClient

adds client data to array.
----------------------------------------------------------------------------*/

void MainWindow::addClient(int id, PCINFO data){
    otherClients[id] = data;
    
}

/*----------------------------------------------------------------------------
remClient

removes client data from array.
----------------------------------------------------------------------------*/
void MainWindow::remClient(int id){
    
    otherClients[id] = NULL;
    
}

/*----------------------------------------------------------------------------
getClientName

gets the username of the specified client.
----------------------------------------------------------------------------*/
QString MainWindow::getClientName(int id){
    if(otherClients[id] != NULL){
        if(strcmp(otherClients[id]->username, "") == 0){
            return otherClients[id]->hostname;
        }
        return otherClients[id]->username;
    }
    else return NULL;
}

/*----------------------------------------------------------------------------
parsePacket

deals with the packet received.  depends on the packet type:
MSG_TEXT:
    adds msg data to display window.
MSG_NEW:
    adds msg data as client info to array.
MSG_REM:
    unimplemented because not needed.
    removes client info from array.
----------------------------------------------------------------------------*/

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
