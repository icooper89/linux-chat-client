#include "client.h"

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

    packet->type = 3;
    packet->owner = info->id;
    strcpy(packet->data, buffer);

    //send(socket->socket, packet, PACKETSIZE, NULL);
}

void cleanup(PMYSOCKET socket, PCINFO info){
    close(socket->socket);
    free(socket);
    free(info);
}
//will need complete rehaul of this function:
void readLoop(PMYSOCKET mySocket){
    PPACKET packet;
    packet = (PPACKET)malloc(sizeof(PACKET));
    recv(mySocket->socket,packet,PACKETSIZE,NULL);
    
    //parsePacket(packet);//move to completion routine? cant remember if recv is blocking or not
}

void parsePacket(PPACKET packet){
    switch(packet->type){
        case MSG_TEXT:
            //send data to window/log
            QString buf;
            buf += getClientName(packet->owner);
            buf += ": ";
            buf += packet->data;
            addToDisplay(buf);
            break;
        case MSG_NEW:
            //add client info
            //addClient((PCINFO)packet->data);
            break;
        case MSG_REM:
            //remove client from list
            //PCINFO temp = (PCINFO)packet->data;
            //remClient(temp->id);
            break;
    }
}

