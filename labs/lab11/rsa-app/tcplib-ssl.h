#ifndef _TCPLIBH_
#define _TCPLIBH_
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "rsa.h"      /* for RSA functions */

#define BACKLOG          5          /* # of requests to queue              */
#define MAXHOSTNAME      32         /* maximum host name length            */

/* error codes */
#define NOHOST   -1   /* host does not exist */
#define SOCKFAIL -2   /* socket creation failed */
#define CONNFAIL -3   /* connection failed */
#define BINDFAIL -4   /* binding failed */
#define ACPTFAIL -5   /* accepting connection failed */

#ifndef ON_DEF
#define ON_DEF
extern int ON;    /* turn certain socket option on and off */
extern int OFF;
#endif

#define PRIVATE 0
#define PUBLIC  1

#define SERVER  0
#define CLIENT  1

int socketClient(char *host, int port);
int socketServer(int port);
int acceptConn(int s);
int writes(int sock, char * buf, int len);
int reads(int sock, char * buf, int len);
int load_private_key(char * fname);
int exchange_public_key(int sock, char * fname, int which);
int encrypt(unsigned char * plain_text, 
	    unsigned char * encrypted_text);
int decrypt(unsigned char * decrypted_text, 
	    unsigned char * encrypted_text, 
	    int encrypted_length) ;
#endif
