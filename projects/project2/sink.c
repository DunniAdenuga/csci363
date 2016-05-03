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
 #include "crcmodel.h"

 #define BUFSIZE 1000000 // size of file read buffer 
 #define MAX_UDP 65536

 unsigned int generate_crc(char * buffer, int len);
 int Socket(int domain, int type, int protocol);
 ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
 ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
 int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
 int recv_data(char *host, int port, char *data, int len);

 struct pkthdr {
    uint8_t  type;          // type of the packet
    uint8_t  id;            // an id number used to identify the data sent
    uint16_t seq;           // sequence number of the packet
    uint16_t data_length;   // length of data in this packet, excluding header
    uint16_t checksum;      // CRC checksum
   char *   data;          // starting location of the data
 };
 int main(int argc, char* argv[]) {


	 int port;		             // protocol port number	       		       
	 // Check command-line argument for protocol port
	 if (argc > 2) {			
		 port = atoi(argv[1]);	        
	 }
	 else {
		 printf("Usage: SINK [ port ][ host ]\n");
		 exit(-1);
	 }
	 char* data;
	 int len;
	 char* host = (char*)malloc(sizeof(argv[2]));;
	 strcpy(host, argv[2]);
	 recv_data(host, port, data, len);

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
 ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen){
   ssize_t sid;
   if((sid = sendto(sockfd, buf, len, flags, dest_addr, addrlen)) < 0){
     perror("Send failed");
     exit(-1);
   }
   return sid;
 }

 int recv_data(char *host, int port, char *data, int len){
   struct sockaddr_in s_in;  // structure to hold server's address	
	 struct sockaddr_in from;  // structure to hold client's address	
	 int sd;	           // socket descriptors      	            		
	 socklen_t alen;          // length of address			

	 int fsize;
	 
	  /*set all bytes in s_in to null*/
	 bzero((char *) &s_in, sizeof(s_in));

	 /* set the socket family to the internet*/
	 s_in.sin_family = (short)AF_INET;

	 /*  INADDR_ANY- makes sure an IPv4 wildcard address is used*/
   s_in.sin_addr.s_addr = htonl(INADDR_ANY);
 if (port > 0)	
		 // test for illegal value	
		  s_in.sin_port = htons((unsigned short) port);
	 else {				
		 // print error message and exit	
		 fprintf(stderr,"SINK: bad port number %s\n",port);
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
	 data  = (char *)malloc(BUFSIZE);/**/
	 int count = 0;
	 while(1){
	   struct pkthdr* recvData =(struct pkthdr*)malloc(sizeof(struct pkthdr));
	   //struct pkthdr* recvData2 =(struct pkthdr*)malloc(sizeof(struct pkthdr));
	   //strcpy(recvData->data, "blo");
	   //recvData->data = (char*)malloc(65600);
	   //strcpy(recvData->data, "");
	   recvData->seq = 0;
	   struct pkthdr* sentData =(struct pkthdr*)malloc(sizeof(struct pkthdr));
	   int seq = 0;
	   
	  while (recvData->seq != 65534){
	    
	 fsize =sizeof(from);
	 ssize_t cc;
	 char* recv = (char*)malloc(BUFSIZE);
	 cc =Recvfrom(sd, recv, BUFSIZE, 0, (struct sockaddr *) &from, &fsize);
	 recvData = (struct pkthdr*)recv;
	 //msg[0]++;
	 //recvData = recvData2;
	 //recvData->data = &recv[struct ];
	 if(recvData->seq > seq){
	 seq = recvData->seq;
	 char * temp = (char*)malloc(MAX_UDP);
	 memcpy(temp, &recv[sizeof(struct pkthdr)],recvData->data_length) ;
	 printf("len - %d,\n type - %d, \n id - %d, \n seq - %d,\n checksum - 0x%x \n, Data: \n",recvData->data_length,recvData->type,recvData->id,recvData->seq,recvData->checksum/*,temp*/);
	 ulong crc = generate_crc(temp, recvData->data_length);
	 printf("crc - %x\n", crc);
	 if(crc == recvData->checksum){
	   //strcpy(&data[count], temp);
	   memcpy(&data[count], temp,recvData->data_length) ;
	   count = count + recvData->data_length;
	   printf("count - %d\n", count);
	   sentData->type = 1;
     sentData->id = 5;
     sentData->seq = 65534;
     //sentData->data = (char *)malloc(MAX_UDP);
     sentData->data = (char*)malloc(10);
     char* help = "ack";
     //strcpy(sentData->data, "nack") ;
     sentData->data = &help[0];
     sentData->data_length = strlen(help)+1;
     char * sent = (char*)malloc(sizeof(struct pkthdr) +strlen(help)+1);
     memcpy(sent,(char*)sentData, sizeof(struct pkthdr));
     memcpy(&sent[sizeof(struct pkthdr)],help, strlen(help)+1);
     unsigned int crc = generate_crc(help, sentData->data_length+1 );
     sentData->checksum = crc;

     //buffer = (char *)malloc(MAX_UDP);
     //strncpy(buffer, data, MAX_UDP-2);

     //memcpy(&(buffer[len]), &crc, 2);
     int size2 = sizeof(from);
     int nbytes = Sendto(sd, sent, sizeof(struct pkthdr)+strlen(help)+1, 0, (struct sockaddr *)&from, size2);
     if(recvData->seq == 65534){
       
       //printf("Contents:\n %s\n", data);
       //FILE * sub = fopen("file_copy", "a+");
        int fd = open("new_file", O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH);
       //fputs(data, sub);
       int written = write(fd, data, count);
       printf("total length- %d\n", count);
       printf("written- %d\n", written);
       //printf("made it here");
	     //data  = (char *)malloc(BUFSIZE);
	     //free(data);
       count = 0;
	     data  = (char *)malloc(BUFSIZE);
     }
	 }
	 else{
	   printf("An error exists in the transmitted message.\n");

     sentData->type = 1;
     sentData->id = 5;
     sentData->seq = 65534;
     //sentData->data = (char *)malloc(MAX_UDP);
     sentData->data = (char*)malloc(10);
     char* help = "nack";

     //strcpy(sentData->data, "nack") ;
     sentData->data = &help[0];
     sentData->data_length = strlen(help)+1;
     char * sent = (char*)malloc(sizeof(struct pkthdr) +strlen(help)+1);
     memcpy(sent,(char*)sentData, sizeof(struct pkthdr));
     memcpy(&sent[sizeof(struct pkthdr)],help, strlen(help)+1);
     unsigned int crc = generate_crc(help, sentData->data_length+1 );
     sentData->checksum = crc;

     //buffer = (char *)malloc(MAX_UDP);
     //strncpy(buffer, data, MAX_UDP-2);

     //memcpy(&(buffer[len]), &crc, 2);
     int size2 = sizeof(from);
     int nbytes = Sendto(sd, sent, sizeof(struct pkthdr)+strlen(help)+1, 0, (struct sockaddr *)&from, size2);
	 }
	  }
	 }
	 
	 }
	  close(sd);
	 return 0;
 }
