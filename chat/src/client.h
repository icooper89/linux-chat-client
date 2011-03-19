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

#include "defines.h"

bool connectToServer(PMYSOCKET socket, PCINFO info);
void sendPacket(char buffer[BUFLEN], PMYSOCKET socket, PCINFO info);
void cleanup(PMYSOCKET socket, PCINFO info);
#endif // CLIENT_H
