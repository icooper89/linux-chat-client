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
    packet = (PPACKET)malloc(sizeof(PPACKET));

    packet->type = 3;
    packet->owner = info->id;
    strcpy(packet->data, buffer);

    //send(socket->socket, packet, PACKETSIZE, NULL);
}
