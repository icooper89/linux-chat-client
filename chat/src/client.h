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
#include <QThread>

#include "defines.h"

class ClientThread: public QThread{
    Q_OBJECT

signals:
    void addDisplaySig(QString);
public:
    ClientThread(PMYSOCKET mySocket);


    void parsePacket(PPACKET packet);
    void readLoop(PMYSOCKET mySocket);
protected:
    void run();

private:
    PMYSOCKET mySocket_;
};

bool connectToServer(PMYSOCKET socket, PCINFO info);
void sendPacket(char buffer[BUFLEN], PMYSOCKET socket, PCINFO info);
void cleanup(PMYSOCKET socket, PCINFO info, int file);


#endif // CLIENT_H
