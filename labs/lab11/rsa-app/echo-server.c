#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "tcplib-ssl.h"

#define data_len 200
char * private_key;
char * public_key;

int
main(int argc, char * argv[]) {

  if (argc != 4) {
    fprintf(stderr, "usage : %s [port-number] [private-key-file] [public-key-file]\n", argv[0]);
    exit(1);
  }

  int    PORT_NUMB  = atoi(argv[1]);
  char * priv_key_f = argv[2];
  char * pub_key_f  = argv[3];
  char*   message = malloc(9999);      // an arbitrary number large than one block data
  //int    i;   // loop index

  /* create server socket and wait for client request */
  int sock = socketServer(PORT_NUMB);
  
  /* load private key, then exchange public key */
  int    key_len = load_private_key(priv_key_f);
  printf("private key length %d\n", key_len);

  while (1) {
    int clis = acceptConn(sock);
    key_len = exchange_public_key(clis, pub_key_f, SERVER);
    printf("public key length %d\n", key_len);
  
    int bytes_comm = 0;
    int bytes_read = 0;
    int bytes_write = 0;
    /* read the message length */
    bytes_read = reads(clis, (char *)&bytes_comm, sizeof(int));
    printf("message length received %d\n", bytes_comm);

    
    int count = 0;
    
    if(bytes_comm > data_len){
      int i = 0;
      //char* wait = malloc(5);
      // strncpy(wait, "ack", 5);
      while(count < bytes_comm){
 	printf("here\n");
	char* tempBuf =malloc(data_len);
      bytes_read = reads(clis, tempBuf, data_len);
      printf("wtf\n");
      //if(bytes_read > 0){
      //writes(clis, wait, 5);
      strcat(message, tempBuf);
       free(tempBuf);
	    count = count + data_len;
	    printf("count - %d\n", count);
	    i++;
	    //}
	    /*else{
	 writes(clis, "nack", 5);
	 }*/
    }
    }
    else{
      bytes_read = reads(clis, message, bytes_comm);
    }
    if (bytes_read <= 0) {
      close(clis);
      continue;
    }
    /* Convert the string to upper case, or whatever app you'd use */
    int x = 0;
    while ( x < bytes_comm){
      message[x] = toupper(message[x]);
      x++;
    }
    printf("upper case %s\n", message);
    int count2 = 0;
     bytes_write = writes(clis, (char *)&bytes_comm, sizeof(int));
    /* send it back */
    if(bytes_comm > data_len){
      int i = 0;
      char* wait = malloc(5);
      while(count2 < bytes_comm){
	//bytes_write = writes(clis, &message[i*data_len], data_len);
	//printf("bytes_write %d\n", bytes_write);
	char* tempBuf = malloc(data_len);
		memcpy(tempBuf, &message[i*data_len], data_len);
    printf("message %s\n", tempBuf);	
    bytes_write = writes(sock, tempBuf, data_len);
    printf("bytes_write %d\n", bytes_write);
     free(tempBuf);
    count2 = count2 + data_len;
    i++;
    /*printf("here2\n");
		char* tempBuf = malloc(data_len);
		memcpy(tempBuf, &message[i*data_len], data_len);
		reads(clis, wait, 5);
		if(strcmp(wait, "ack")==0){
    printf("message %s\n", tempBuf);	
    bytes_write = writes(clis, tempBuf, data_len);
    printf("bytes_write %d\n", bytes_write);
    count2 = count2 + data_len;
    i++;
    }*/
    free(tempBuf);
    free(wait);
      }
    }
    else{
      bytes_write = writes(clis, message, bytes_comm);
      printf("bytes_write %d\n", bytes_write);
    }
    close(clis);
  }

  close(sock);

  return 0;
}

//char* convert(int byte_comm, char* message){
