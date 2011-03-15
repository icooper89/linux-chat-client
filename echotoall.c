#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_TCP_PORT 7000	// Default port
#define BUFLEN	255		//Buffer length
#define TRUE	1
#define LISTENQ	5
#define MAXLINE 4096

void setupListenSocket (int port);
void serverLoop(int listen_sd);
void echoToAll(int origin,int client[], int maxi,char buf[]);
static void SystemFatal(const char* message);


int main(void){
	setupListenSocket(SERVER_TCP_PORT);
	return 0;
}


void setupListenSocket (int port){

	int arg;
	int listen_sd;
	struct sockaddr_in server;
	
   	

	// Create a stream socket
	if ((listen_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		SystemFatal("Cannot Create Socket!");
	
	// set SO_REUSEADDR so port can be resused imemediately after exit, i.e., after CTRL-c
        arg = 1;
        if (setsockopt (listen_sd, SOL_SOCKET, SO_REUSEADDR, &arg, sizeof(arg)) == -1)
                SystemFatal("setsockopt");

	// Bind an address to the socket
	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY); // Accept connections from any client

	if (bind(listen_sd, (struct sockaddr *)&server, sizeof(server)) == -1)
		SystemFatal("bind error");
	

	serverLoop(listen_sd);
}





void serverLoop(int listen_sd){


	int i, maxi, nready, bytes_to_read;
	int new_sd, sockfd,  maxfd, client[FD_SETSIZE];
	struct sockaddr_in client_addr;
	socklen_t client_len;
	char *bp, buf[BUFLEN];
   	size_t n;
   	fd_set rset, allset;


	listen(listen_sd, LISTENQ);

	maxfd	= listen_sd;	// initialize
   	maxi	= -1;		// index into client[] array

	for (i = 0; i < FD_SETSIZE; i++)
           	client[i] = -1;             // -1 indicates available entry
 	FD_ZERO(&allset);
   	FD_SET(listen_sd, &allset);



	while (TRUE)
	{
   		rset = allset;               // structure assignment
		nready = select(maxfd + 1, &rset, NULL, NULL, NULL);


      		if (FD_ISSET(listen_sd, &rset)) // new client connection
      		{
			client_len = sizeof(client_addr);
			if ((new_sd = accept(listen_sd, (struct sockaddr *) &client_addr, &client_len)) == -1)
				SystemFatal("accept error");
			
                        printf(" Remote Address:  %s\n", inet_ntoa(client_addr.sin_addr));

                        for (i = 0; i < FD_SETSIZE; i++)
			if (client[i] < 0)
            		{
				client[i] = new_sd;	// save descriptor
				break;
            		}
			if (i == FD_SETSIZE)
         		{
				printf ("Too many clients\n");
            			exit(1);
    			}

			FD_SET (new_sd, &allset);     // add new descriptor to set
			if (new_sd > maxfd)
				maxfd = new_sd;	// for select

			if (i > maxi)
				maxi = i;	// new max index in client[] array

			if (--nready <= 0)
				continue;	// no more readable descriptors
     		 }

		for (i = 0; i <= maxi; i++)	// check all clients for data
     		{
			if ((sockfd = client[i]) < 0)
				continue;

			if (FD_ISSET(sockfd, &rset))
         		{
         			bp = buf;
				bytes_to_read = BUFLEN;
				while ((n = read(sockfd, bp, bytes_to_read)) > 0)
				{
					bp += n;
					bytes_to_read -= n;
				}
				//write(sockfd, buf, BUFLEN);   // echo to client
				echoToAll(sockfd, client, maxi, buf);  //echo to all clients but original sender

				if (n == 0) // connection closed by client
            			{
					printf(" Remote Address:  %s closed connection\n", inet_ntoa(client_addr.sin_addr));
					close(sockfd);
					FD_CLR(sockfd, &allset);
               				client[i] = -1;
            			}
									            				
				if (--nready <= 0)
            				break;        // no more readable descriptors
			}
     		 }
   	}
}

void echoToAll(int origin,int client[], int maxi,char buf[]){
	int i, sockfd;

	for(i = 0; i <=maxi; i++){
		if((sockfd = client[i]) <0 || sockfd == origin){
			continue;
		}
		write(sockfd, buf, BUFLEN);
	}
}

static void SystemFatal(const char* message)
{
    perror (message);
    exit (EXIT_FAILURE);
}
