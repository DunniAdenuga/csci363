#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 1000
#define NAMELEN 257
#define MAX_UDP 65536

int Socket(int domain, int type, int protocol);
ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int main(int argc, char* argv[]) {
  struct sockaddr_in in;//me-gateway
  struct sockaddr_in dest, from;//dest-sink, from - source
  struct hostent *hostptr;
  int fsize;
  char *msg = (char *)malloc(MAX_UDP);
  int	sd, sd2;		                 // socket descriptor	       	
  int	gport, sport;		               // protocol port number		
  char	*host;		           // pointer to host name		
  	
  
  memset((void *)&dest,0,sizeof(dest)); // clear sockaddr structure
  bzero((char *) &in, sizeof(in));
  dest.sin_family = (short)AF_INET;	          // set family to Internet	
  in.sin_family = (short)AF_INET;
  in.sin_addr.s_addr = htonl(INADDR_ANY);
  // Check command-line arguments
  
  if (argc < 4) {
    printf("usage: gateway [gateway port number ][sink host] [sink port number] \n");
    exit(-1);
  }

  hostptr = gethostbyname(argv[2]);		
  gport = atoi(argv[1]);
  sport = atoi(argv[3]);
 
  //copy bytes for host to destination socket
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);

  if ((sport <= 0)||(gport <= 0)) {	
    // test for legal value		
    // print error message and exit	
    fprintf(stderr,"GATEWAY: one or more bad port numbers %s, %s\n",argv[3], argv[1]);
    exit(1);
  }
  else{
    dest.sin_port = htons((u_short) sport);
    in.sin_port = htons((unsigned short) gport);
  }
  
  // Create a socket to receive from source
	sd = Socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0) {
		fprintf(stderr, "GATEWAY-SOURCE: socket creation failed\n");
		exit(1);
	}

	// Bind a local address to the socket 
	Bind(sd, (struct sockaddr *)&in, sizeof(in));  
	//while(1){
	
	
	fsize =sizeof(from);
	ssize_t cc;
	cc =Recvfrom(sd, msg, MAX_UDP, 0, (struct sockaddr *) &from, &fsize);
	
	//}
	
	printf("Message received from source in %d bytes\n", cc);
	close(sd);
	
	long int random_num = random() % 10;
	if((random_num == 1) ||(random_num == 2)){
	  strcat(msg, "!");
}
  // Create a socket to send to sink 
  
  sd2 = Socket(AF_INET, SOCK_DGRAM, 0);
  if (sd < 0) {
    fprintf(stderr, "GATEWAY-SINK: socket creation failed\n");
    exit(1);
  }
  
 
  int nbytes = Sendto(sd2, msg, (size_t)cc, 0, (struct sockaddr *)&dest, sizeof(dest));
 
 printf("Message sent to sink in %d bytes\n", nbytes);
  close(sd2);
  
  // Terminate the client program gracefully 
  return 0;
  //exit(0);
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

ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen){
  ssize_t rfid;
  if((rfid = recvfrom(sockfd, buf, len, flags, src_addr, addrlen)) < 0){
    perror("Receive failed");
    exit(-1);
  }
  return rfid;
}

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen){
  int bid;
  bid = bind(sockfd, addr, addrlen);
  if(bid == -1){
    perror("Bind Failed");
      exit(-1);
    }
  return bid;
}
