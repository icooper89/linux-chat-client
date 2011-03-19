#define MAXLINE 4096
#define MAXCLIENTS 256
#define MAXNAMELEN 256
#define BUFLEN	1024 - 2 * sizeof(int)		//Buffer length
#define PACKETSIZE 1024


#define MSG_NEW 1
#define MSG_REM 2
#define MSG_TEXT 3


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