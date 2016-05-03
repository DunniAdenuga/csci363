#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include "crcmodel.h"

#define BUFSIZE 100000
#define MAX_UDP 65536

unsigned int generate_crc(char * buffer, int len);
int Socket(int domain, int type, int protocol);
ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
int send_data(char *host, int port, char *data, int len);

struct pkthdr {
   uint8_t  type;          // type of the packet
   uint8_t  id;            // an id number used to identify the data sent
   uint16_t seq;           // sequence number of the packet
   uint16_t data_length;   // length of data in this packet, excluding header
   uint16_t checksum;      // CRC checksum
  char *   data;          // starting location of the data
};
/*type:
 *1- data
 *2- ack
 *3-nack
 */
int main(int argc, char* argv[]) {
  int fd,len;
  		
  if (argc < 4) {
    printf("usage: source [ sink host name ] [ sink port number ] [fileName ]\n");
    exit(-1);
  }
  char *fileName = (char *)malloc(100);
  int port = atoi(argv[2]);
  strcpy(fileName, argv[3]);
  if ((fd = open(fileName, O_RDONLY)) < 0) {	
    // test for legal value		
    // print error message and exit	
    fprintf(stderr,"SOURCE: file name error! %s\n",argv[3]);
    exit(1);
  }
  struct stat bufStat;
  fstat(fd, &bufStat);
  len = bufStat.st_size;
  char *buffer =  (char *)malloc(len);
  read(fd, buffer, len);
  send_data(argv[1], port, buffer, len);
  return 0;
}

unsigned int generate_crc(char * buffer, int len){
char   ch;
cm_t   cm;
p_cm_t p_cm = &cm;
 p_cm->cm_width = 16;
  p_cm->cm_poly  = 0x8005L;       // CRC16
  //  p_cm->cm_poly  = 0x1021L;   // CRC-CCITT

  /*
   * parameter set-up ends
   */

  // one can initalize the proceeding bits wit 0s '0L' or 1s 'FFFFL'
  p_cm->cm_init  = 0L;            
  //  p_cm->cm_init  = 0xFFFFL;
  p_cm->cm_refin = TRUE;
  p_cm->cm_refot = TRUE;
  p_cm->cm_xorot = 0L;

  cm_ini(p_cm);

  for (int i = 0; i < len; i ++) {
    ch = buffer[i];
    cm_nxt(p_cm, ch);
  }
  ulong crc = cm_crc(p_cm);
  return crc;
}

int send_data(char *host, int port, char *data, int len){
struct sockaddr_in in;
 int id = (int)random();
  //struct	sockaddr_in sad; // structure to hold an IP address
 struct sockaddr_in dest, from;
  struct hostent *hostptr;

  int	fd,sd, sd2;		                 // socket descriptor	       	  
  char* buffer;		
  
  memset((void *)&dest,0,sizeof(dest)); // clear sockaddr structure	
  dest.sin_family = (short)AF_INET;	          // set family to Internet	
  
  // Check command-line arguments
  hostptr = gethostbyname(host);		
  
  //copy bytes for host to destination socket
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);

  if (port <= 0) {	
    // test for legal value		
    // print error message and exit	
    fprintf(stderr,"SOURCE: bad port number %s\n",port);
    exit(1);
  }
  else{
    dest.sin_port = htons((u_short) port);
  }
  // Create a socket. 
  sd = Socket(AF_INET, SOCK_DGRAM, 0);
  if (sd < 0) {
    fprintf(stderr, "SOURCE: socket creation failed\n");
    exit(1);
  }

  if(len > (MAX_UDP-50)){
    printf("big len %d\n", len);
    struct pkthdr* sentData = (struct pkthdr *)malloc(sizeof(struct pkthdr));
    struct pkthdr* recvData = (struct pkthdr *)malloc(sizeof(struct pkthdr));
    int noOfSegments = (len/(MAX_UDP- 50));
    if(len%MAX_UDP != 0){
      noOfSegments = (len/(MAX_UDP - 50))+ 1;
    }
    char * temp = (char *)malloc (10);
    int i = 0;
    int sameId = id;
    while(i < noOfSegments){  
    sentData->type = 1;
    sentData->id = sameId;
    sentData->seq = i + 1;
    if(i == noOfSegments - 1){
      sentData->seq = 65534;
    }
    //sentData->data = (char *)malloc(MAX_UDP);
    sentData->data = &data[i*(MAX_UDP-50)];
    //strncpy(sentData->data, &data[i*(MAX_UDP-2)],MAX_UDP-2) ;
    char *tempBuffer = (char *)malloc(MAX_UDP);
    strncpy(tempBuffer, &data[i*(MAX_UDP-50)], MAX_UDP-50);
    int tempLen = MAX_UDP-50;
    if(i == noOfSegments - 1){
      tempLen = len - (i*(MAX_UDP-50));
    }
    sentData->data_length = tempLen;
    unsigned int crc = generate_crc(tempBuffer, sentData->data_length );
    sentData->checksum = crc;
  
    //memcpy(&(buffer[len]), &crc, 2);
    int size = sizeof(dest);
    int size2 = sizeof(from);
    char * sent = (char*)malloc(sizeof(struct pkthdr) + tempLen);
    memcpy(sent,(char*)sentData, sizeof(struct pkthdr));
    memcpy(&sent[sizeof(struct pkthdr)],tempBuffer,tempLen);
    printf("len - %d,\n type - %d, \n id - %d, \n seq - %d,\n checksum - 0x%x \n, Data:\n",sentData->data_length,sentData->type,sentData->id,sentData->seq,sentData->checksum /*,tempBuffer*/);
    //printf("total length %d\n", sizeof(struct pkthdr)+ tempLen);
    int nbytes = Sendto(sd, sent, sizeof(struct pkthdr)+ tempLen, 0, (struct sockaddr *)&dest, size);
  char * receive =  malloc(50);   
  int  cc =Recvfrom(sd, receive, BUFSIZE , 0, (struct sockaddr *) &from, &size2);
  recvData = (struct pkthdr*)receive;
  temp = &receive[sizeof(struct pkthdr)];
  printf("temp - %s\n", temp);
    if(strcmp(temp, "ack") == 0){
    i++;
      }
    }
  }
  else{
    struct pkthdr* sentData = (struct pkthdr *)malloc(sizeof(struct pkthdr));
    struct pkthdr* recvData = (struct pkthdr *)malloc(sizeof(struct pkthdr));
    char * temp = (char *)malloc (10);
    strcpy(temp, "nack");
    int sameId = id;
    while(strcmp(temp, "nack") == 0){
      //struct pkthdr * sentData = (struct pkthdr *)malloc(sizeof(struct pkthdr));
      //struct pkthdr * recvData = (struct pkthdr *)malloc(sizeof(struct pkthdr));
    sentData->type = 1;
    sentData->id = sameId;
    sentData->seq = 65534;
    //sentData->data = (char *)malloc(MAX_UDP);
    //strcpy(sentData->data, data);
    sentData->data = &data[0];
    sentData->data_length = len;
    unsigned int crc = generate_crc(data,sentData->data_length );
    sentData->checksum = crc;
    printf("len - %d \n ", sentData->data_length);
	   //strcpy(sentData->data, data);
    int size = sizeof(dest);
    int size2 = sizeof(from);
    //sentData->data_length = len + 1;
    char * sent = (char*)malloc(sizeof(struct pkthdr) +len);
    memcpy(sent,(char*)sentData, sizeof(struct pkthdr));
    //strcat(data, "!");
    memcpy(&sent[sizeof(struct pkthdr)],data,len+1);//len+1-when you update data
    //strncpy(&sent[8],data,len);
    //strcat(sent, data);
    //strcpy(&sent[sizeof(struct pkthdr)], data);
    int nbytes = Sendto(sd, sent, sizeof(struct pkthdr)+len, 0, (struct sockaddr *)&dest, size);
    char * receive = malloc(50);
    int  cc =Recvfrom(sd, receive,BUFSIZE , 0, (struct sockaddr *) &from, &size2);
     recvData = (struct pkthdr*)receive;
     temp = &receive[sizeof(struct pkthdr)];
     printf("temmp - %s\n", temp);
    }
  }
  
  //printf("Message sent to gateway in %d bytes\n", nbytes);
  close(fd);
  close(sd);
  return 0;
}

//int recv_data(char *host, int port, char *data, int len){
//}

int Socket(int domain, int type, int protocol){
  int sd;
  sd = socket(domain, type, protocol);
  if(sd == -1){
    perror("Socket Failed");
    exit(-1);
  }
  return sd;
}

ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen){
  ssize_t sid;
  if((sid = sendto(sockfd, buf, len, flags, dest_addr, addrlen)) < 0){
    perror("Send failed");
    exit(-1);
  }
  return sid;
}

ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen){
  ssize_t rfid;
  if((rfid = recvfrom(sockfd, buf, len, flags, src_addr, addrlen)) < 0){
    perror("Receive failed");
    exit(-1);
  }
  return rfid;
}
