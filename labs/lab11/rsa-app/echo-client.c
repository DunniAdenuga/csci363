#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "tcplib-ssl.h"

#define data_len 200
char * private_key = NULL;
char * public_key = NULL;

char * read_long_message();  // generate message longer than one block of data

int
main(int argc, char * argv[]) {

  if (argc != 5) {
    fprintf(stderr, "usage : %s [server-name] [port-number] [private-key-file] [public-key-file]\n", argv[0]);
    exit(1);
  }

  char * SERV_NAME  = argv[1];
  int    PORT_NUMB  = atoi(argv[2]);
  char * priv_key_f = argv[3];
  char * pub_key_f  = argv[4];

  /* create client socket and connect to the server */
  int    sock = socketClient(SERV_NAME, PORT_NUMB);

  /* load private key, then exchange public key */
  int    key_len = load_private_key(priv_key_f);
  printf("private key length %d\n", key_len);
  
  key_len = exchange_public_key(sock, pub_key_f, CLIENT);
  printf("public key length %d\n", key_len);
  
  //char * message = "Hello world";  // short message
  char * message = read_long_message();
  printf("message - %s\n", message);
  char ret_msg[9999];
  int  bytes_comm = strlen(message);
  int bytes_read = 0;
   int bytes_write = 0;
  /* test message, write the message to the server, then read it back  */
  /* we first write the length of the message */
  //int len = writes(sock, (char *)&bytes_comm, sizeof(int));
  /* then the message itself */
  //len = writes(sock, message, strlen(message));
    int count2 = 0;
     bytes_write = writes(sock, (char *)&bytes_comm, sizeof(int));
     printf("bytes_comm %d\n", bytes_comm);
    /* send it back */
    if(bytes_comm > data_len){
      //	printf("here\n");
      int i = 0;
      //char* wait = malloc(5);
      //strncpy(wait, "ack", 5);
      while(count2 < bytes_comm){
	//printf("here2\n");
		char* tempBuf = malloc(data_len);
		memcpy(tempBuf, &message[i*data_len], data_len);
		//reads(sock, wait, 5);
		
    printf("message %s\n", tempBuf);	
    bytes_write = writes(sock, tempBuf, data_len);
    printf("bytes_write %d\n", bytes_write);
    count2 = count2 + data_len;
    i++;
   
    free(tempBuf);
    //free(wait);
	    
      }
    }
    else{
      bytes_write = writes(sock, message, bytes_comm);
    }
  printf("message sent %s\n", message);

  /* read back from the server, first the length, then the message */
  //len = 
  reads(sock, (char *)&bytes_comm, sizeof(int));
  //len = reads(sock, ret_msg, bytes_comm);
  int count = 0;
  if(bytes_comm > data_len){
      int i = 0;
      //char* wait = malloc(5);
      //strncpy(wait, "ack", 5);
      while(count < bytes_comm){
	//bytes_read = reads(sock, &ret_msg[i*data_len], data_len);
	char* tempBuf =malloc(data_len);
      bytes_read = reads(sock, tempBuf, data_len);
      strcat(message, tempBuf);
       free(tempBuf);
	    count = count + data_len;
	    i++;
	/*printf("here\n");
	char* tempBuf =malloc(data_len);
      bytes_read = reads(sock, tempBuf, data_len);
      printf("wtf\n");
      if(bytes_read > 0){
      writes(sock, wait, 5);
      strcat(message, tempBuf);
       free(tempBuf);
	    count = count + data_len;
	    printf("count - %d\n", count);
	    i++;
      }
      else{
	 writes(sock, "nack", 5);
	 }*/
    }
    }
    else{
      bytes_read = reads(sock, ret_msg, bytes_comm);
    }

  if (bytes_read <= 0) {
    perror("read message error");
    exit(1);
  }

  ret_msg[bytes_read] = 0;
  printf("message received %s\n", ret_msg);
  printf("received message length %d\n", strlen(ret_msg));

   close(sock);

  return 0;
}

/*
 * char * read_long_message()
 * 
 * The message should be longer than the allowed text buffer of 
 * 256 so it has to be sent in multiple chunks.
 */
char * read_long_message() {

  int i;
  char * data = "0123456789012345678901234567890123456789"; // 40 bytes
  char * msg = (char*)malloc(1024); // some random number > 256

  strcpy(msg, data);
  for (i = 0; i < 10; i ++)
    strcat(msg, data);
  printf("test message length : %u\n", (unsigned int)strlen(msg));


  return msg;
}
