#include "client.h"
#include "mainwindow.h"
#include <QString>
#include <QByteArray>

/*----------------------------------------------------------------------------
connectToServer

does the connect to tcp server.
----------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------
sendPacket

sends buf to server.
----------------------------------------------------------------------------*/
void sendPacket(char buffer[BUFLEN], PMYSOCKET socket, PCINFO info){
    PPACKET packet;
    packet = (PPACKET)malloc(sizeof(PACKET));
    memset(packet->data, 0, sizeof(packet->data));
    packet->type = 3;
    packet->owner = info->id;
    strcpy(packet->data, buffer);
    send(socket->socket, (char*) packet, PACKETSIZE, NULL);
}
/*----------------------------------------------------------------------------
cleanup

closes sockets and files and structs.
----------------------------------------------------------------------------*/
void cleanup(PMYSOCKET socket, PCINFO info, int file){
    close(socket->socket);
    close(file);
    free(socket);
    free(info);
}
/*----------------------------------------------------------------------------
readloop

thread code.
----------------------------------------------------------------------------*/
void ClientThread::readLoop(PMYSOCKET mySocket){
    PPACKET packet;
    packet = (PPACKET)malloc(sizeof(PACKET));
    recv(mySocket->socket,(char*)packet,sizeof(PACKET),NULL);
    
    emit parsePacketSig(packet);
    readLoop(mySocket);
}

/*----------------------------------------------------------------------------
ClientThread

Constructor.
----------------------------------------------------------------------------*/
ClientThread::ClientThread(PMYSOCKET mySocket):mySocket_(mySocket){

}
/*----------------------------------------------------------------------------
run

base for thread.
----------------------------------------------------------------------------*/
void ClientThread::run(){
    readLoop(mySocket_);
}
