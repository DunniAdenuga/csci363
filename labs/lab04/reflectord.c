/*
 * A simple TCP echo server
 */
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERV_PORT 14002
#define MAXLEN    2048
#define BACKLOG   5
#define MAXSIZE 65527

struct udp_header_t {
   uint16_t source;   /* source port */
   uint16_t dest;     /* destination port */
   uint16_t len;      /* length, including header and data */
   uint16_t check;    /* checksum */
};

struct udp_packet_t {
   struct udp_header_t header;  /* udp header */
   char   data[MAXSIZE];     /* data */
};

/*
create a server socket;
repeat forever
  wait for client connection request;
  accept a client request;
  read the packet size from the client;
  read a packet from the client;
  randomly alter (or not) a bit in the packet;
  send the packet back to the client;
  close the connection;
 */

int main(int argc,char *argv[])    {

  int s, t;
  //int msg_len, 
  int i;
  //char msg[MAXLEN];
  struct sockaddr_in  sa;  /* socket addr. structure           */
  struct sockaddr_in  cl;  /* socket addr. structure           */

   /*
    * Allocate an open socket.
    */
   if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
     perror("TCP socket error.");
     exit(1);
   }

   /*
    * Set up socket address data for the server
    */
   memset((char *)&sa, 0, sizeof(sa));
   sa.sin_family      = AF_INET;
   sa.sin_addr.s_addr = htonl(INADDR_ANY);
   sa.sin_port        = htons(SERV_PORT);

   /*
    * Bind the socket to the service
    */
   if (bind(s, (struct sockaddr *)(&sa), sizeof sa) < 0) {
     perror("TCP bind error.");
     exit(1);
   }

   /*
    * Set maximum connections we will fall behind
    */
   listen(s, BACKLOG);
   
   i = sizeof(cl);
   while (1)    {
     if (s > 0)
       t = accept(s,(struct sockaddr *)(&cl),(socklen_t*)&i);
     else  {
	perror("socket error\n");
	exit(1);
     }
     if (t > 0)  {
       
       int size;
       struct udp_packet_t pack;
       read(t, &size, 4);
       read(t, &pack, size);
  
       strcat(pack.data, ".");
       int size2 = sizeof(pack);
       write(t,&size2 , 4);
       write(t, &pack, size2);
     }  else {
       perror(" connection error\n");
       exit(2);
     }
     close(t);
    } /* while(1) */

  close(s);
  return 0;
}
