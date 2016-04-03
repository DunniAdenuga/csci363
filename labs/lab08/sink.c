#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFSIZE     1000 // size of file read buffer 
#define MAX_UDP 65536

int Socket(int domain, int type, int protocol);
ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int main(int argc, char* argv[]) {
	struct sockaddr_in s_in;  // structure to hold server's address	
	struct sockaddr_in from;  // structure to hold client's address	
	int sd, sd2;	           // socket descriptors			
	int port;		             // protocol port number		
	socklen_t alen;          // length of address			
	
	int fsize;
	char *msg = (char *)malloc(BUFSIZE);

	 /*set all bytes in s_in to null*/
	bzero((char *) &s_in, sizeof(s_in));

	/* set the socket family to the internet*/
	s_in.sin_family = (short)AF_INET;

  /*  INADDR_ANY- makes sure an IPv4 wildcard address is used
  //converts the unsigned integer hostlong  from  host byte order to network byte order*/
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);


	// Check command-line argument for protocol port
	if (argc > 1) {			
		port = atoi(argv[1]);	        
	}
	else {
		printf("Usage: SINK [ port ]\n");
		exit(-1);
	}

	if (port > 0)	
		// test for illegal value	
		 s_in.sin_port = htons((unsigned short) port);
	else {				
		// print error message and exit	
		fprintf(stderr,"SINK: bad port number %s\n",argv[1]);
		exit(1);
	}

	// Create a socket 
	sd = Socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0) {
		fprintf(stderr, "SINK: socket creation failed\n");
		exit(1);
	}

	// Bind a local address to the socket 
	Bind(sd, (struct sockaddr *)&s_in, sizeof(s_in));  
	//while(1){
	
	
	fsize =sizeof(from);
	ssize_t cc;
	cc =Recvfrom(sd, msg, MAX_UDP, 0, (struct sockaddr *) &from, &fsize);
	
	//}
	
	printf("Message received from gateway in %d bytes\n", cc);
	printf("Message received from gateway is: %s \n", msg);
	close(sd);
	return 0;
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
