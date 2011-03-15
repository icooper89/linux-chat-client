#include "client.h"

bool connectToServer(int socket, int port, char* host){
    struct hostent *hp;
    struct sockaddr_in server;

    bzero((char *)&server, sizeof(struct sockaddr_in));
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        if ((hp = gethostbyname(host)) == NULL)
        {
            fprintf(stderr, "Unknown server address\n");
            return false;
        }
        bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);

        // Connecting to the server
        if (connect (socket, (struct sockaddr *)&server, sizeof(server)) == -1)
        {
            fprintf(stderr, "Can't connect to server\n");
            perror("connect");
            return false;
        }
    return true;

}
