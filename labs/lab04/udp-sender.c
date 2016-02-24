/*
 * A simple TCP echo client
 */

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <netinet/udp.h>

#define SERV_PORT 14002
#define MAXLEN    2048
#define MAXUDPSIZE 65527
#define SERV_NAME "linuxremote1"
extern int inet_checksum(uint16_t *, int);

/*create a client socket;
establish a TCP connection with the server;
create a UDP packet with header and data;
compute and pack the checksum into the packet;
send the packet size to the server;
send the packet to the server;
read back the packet from the server;
check if the packet is corrupted or not;
print an appropriate message;*/

struct udp_header_t {
   uint16_t source;   /* source port */
   uint16_t dest;     /* destination port */
   uint16_t len;      /* length, including header and data */
   uint16_t check;    /* checksum */
};

struct udp_packet_t {
   struct udp_header_t header;  /* udp header */
   char   data[MAXUDPSIZE];     /* data */
};


void create_UDP_pkt(uint16_t src_port, uint16_t dst_port,
		    char * data, struct udp_packet_t *pkt){
  pkt->header.source = src_port;
  pkt->header.dest = dst_port;
  //printf("pre Data is: %s\n", data);
  strcpy(pkt->data, data);
  //printf("crpacket Data is: %s\n", pkt->data);
  pkt->header.len = strlen(pkt->data)+ sizeof(pkt->header);
  pkt->header.check = 0;
 
}

int main(int argc, char *argv[])   {

  int s;                   /* socket descriptor                */
  struct sockaddr_in  sa;  /* socket addr. structure, server   */
  struct addrinfo *ptrh;


   /*
    * Allocate an open socket.
    */
   if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
     perror("TCP socket error.");
     exit(1);
   }

   /*
    * Get server host information
    */
   getaddrinfo(SERV_NAME, NULL, NULL, &ptrh);

   /*
    * Set up socket address data for the server
    */
   memset((char *)&sa, 0, sizeof(sa));
   memcpy(&sa, (struct sockaddr_in *)(ptrh->ai_addr), ptrh->ai_addrlen);
   sa.sin_family      = AF_INET;
   sa.sin_port        = htons(SERV_PORT);

   /*
    * Connect to the remote server.
    */
   if (connect(s,(struct sockaddr *)(&sa),sizeof(struct sockaddr)) < 0) {
     perror("TCP connection error.");
     exit(1);
   }

  
   struct udp_packet_t packet;
   struct udp_packet_t packet2;
   create_UDP_pkt(sa.sin_port, SERV_PORT, "The Lady Morgana & Merlin", &packet);
   /*send the packet size to the server;
send the packet to the server;
read back the packet from the server;
check if the packet is corrupted or not;
print an appropriate message;*/
   //printf("crpacket Data2 is: %s\n", packet.data);
   packet.header.check = inet_checksum((uint16_t *) &packet, packet.header.len);
   
   int size = sizeof(packet);
   write(s, &size, 4);
   write(s, &packet, size);
   int size2; 
   read(s, &size2, 4);
   read(s, &packet2, size2);
   int check = packet2.header.check;
  
   packet2.header.check = 0;
   packet2.header.check = inet_checksum((uint16_t *) &packet2, packet2.header.len);
   

   if(check == packet2.header.check){
     printf("Data is: %s\n", packet2.data);
   }
   else{
     printf("Data is corrupted.\n");
   }
}


