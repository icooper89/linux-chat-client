#include "client.h"
#include "mainwindow.h"
#include <QString>
#include <QByteArray>

bool connectToServer(PMYSOCKET socket, PCINFO info){
    struct hostent *hp;
    struct sockaddr_in server;

    bzero((char *)&server, sizeof(struct sockaddr_in));
        server.sin_family = AF_INET;
        server.sin_port = htons(socket->port);
        if ((hp = gethostbyname(info->hostname)) == NULL)
        {
            fprintf(stderr, "Unknown server address\n");
            return false;
        }
        bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);

        // Connecting to the server
        if (connect (socket->socket, (struct sockaddr *)&server, sizeof(server)) == -1)
        {
            fprintf(stderr, "Can't connect to server\n");
            perror("connect");
            return false;
        }
    return true;

}

void sendPacket(char buffer[BUFLEN], PMYSOCKET socket, PCINFO info){
    PPACKET packet;
    packet = (PPACKET)malloc(sizeof(PACKET));
    memset(packet->data, 0, sizeof(packet->data));
    packet->type = 3;
    packet->owner = info->id;
    strcpy(packet->data, buffer);
    send(socket->socket, (char*) packet, PACKETSIZE, NULL);
}

void cleanup(PMYSOCKET socket, PCINFO info, int file){
    close(socket->socket);
    close(file);
    free(socket);
    free(info);
}
//will need complete rehaul of this function:
void readLoop(PMYSOCKET mySocket){
    PPACKET packet;
    packet = (PPACKET)malloc(sizeof(PACKET));
    recv(mySocket->socket,(char*)packet,sizeof(PACKET),NULL);
    
    parsePacket(packet);//move to completion routine? cant remember if recv is blocking or not
    readLoop(mySocket);
}

void parsePacket(PPACKET packet){
    switch(packet->type){
        case MSG_TEXT:{
            //send data to window/log
            /*QString buf;
            buf += MainWindow::getClientName(packet->owner);
            buf += ": ";
            buf += packet->data;
            MainWindow::addToDisplay(buf);*/
            break;
        }
        case MSG_NEW:
            //add client info
            //PCINFO temp = (PCINFO)packet->data;
            //addClient(temp->id,temp);
            break;
        case MSG_REM:
            //remove client from list
            //PCINFO temp = (PCINFO)packet->data;
            //remClient(temp->id);
            break;
    }
}

ClientThread::ClientThread(PMYSOCKET mySocket):mySocket_(mySocket){

}

void ClientThread::run(){
    readLoop(mySocket_);
}
