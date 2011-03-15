#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE 4096
#define MAXCLIENTS 256
#define MAXNAMELEN 256
#define BUFLEN	1024 - 2 * sizeof(int)		//Buffer length
#define PACKETSIZE 1024

typedef struct ClientInfo {
    int id;
        char hostname[MAXNAMELEN];
        char username[MAXNAMELEN];
        int admin;
} CINFO, *PCINFO;
typedef struct Packet{
    int type;
    int owner;
    char data[BUFLEN];
} PACKET, *PPACKET;

typedef struct MySocket{
    int socket;
    int port;
} MYSOCKET, *PMYSOCKET;

bool connectToServer(PMYSOCKET socket, PCINFO info);
void sendPacket(char buffer[BUFLEN], PMYSOCKET socket, PCINFO info);
#endif // CLIENT_H
