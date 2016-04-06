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
#include "crcmodel.h"

#define BUFSIZE 1000
#define MAX_UDP 65536

unsigned int generate_crc(char * buffer, int len);
int Socket(int domain, int type, int protocol);
ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);

int main(int argc, char* argv[]) {
  struct sockaddr_in in;

  //struct	sockaddr_in sad; // structure to hold an IP address
  struct sockaddr_in dest;
  struct hostent *hostptr;

  int	fd,sd, sd2;		                 // socket descriptor	       	
  int	port, len;		               // protocol port number		
  char	*host;		           // pointer to host name		
  char	*buf = (char *)malloc(BUFSIZE);	       // buffer for data to be sent	
  char *fileName = (char *)malloc(100);
  char *buffer;
  memset((void *)&dest,0,sizeof(dest)); // clear sockaddr structure	
  dest.sin_family = (short)AF_INET;	          // set family to Internet	
  
  // Check command-line arguments
  
  if (argc < 4) {
    printf("usage: source [ gateway host name ] [ gateway port number ] [fileName ]\n");
    exit(-1);
  }

  hostptr = gethostbyname(argv[1]);		
  port = atoi(argv[2]);	
  //len = atoi(argv[3]);
  strcpy(fileName, argv[3]);
  //copy bytes for host to destination socket
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);

  if (port <= 0) {	
    // test for legal value		
    // print error message and exit	
    fprintf(stderr,"SOURCE: bad port number %s\n",argv[2]);
    exit(1);
  }
  else{
    dest.sin_port = htons((u_short) port);
  }
  if ((fd = open(fileName, O_RDONLY)) < 0) {	
    // test for legal value		
    // print error message and exit	
    fprintf(stderr,"SOURCE: file name error! %s\n",argv[3]);
    exit(1);
  }

  struct stat bufStat;
  fstat(fd, &bufStat);
  len = bufStat.st_size; 

  if(len > MAX_UDP){
    printf("Message too large\n");
    exit(1);
  }
  else{
    buffer = (char *)malloc(len + 2);
    read(fd, buffer, len);
    unsigned int crc = generate_crc(buffer, len);
    //printf("crc - %d\n", crc);
    memcpy(&(buffer[len]), &crc, 2);
    //printf("buffer - %s", buffer);
  }
  // Create a socket. 
  sd = Socket(AF_INET, SOCK_DGRAM, 0);
  if (sd < 0) {
    fprintf(stderr, "SOURCE: socket creation failed\n");
    exit(1);
  }
  //strcat(buf, "Ile aye ese mefa");
  int nbytes = Sendto(sd, buffer, len+2, 0, (struct sockaddr *)&dest, sizeof(dest));
  
  printf("Message sent to gateway in %d bytes\n", nbytes);
  close(fd);
  close(sd);
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
