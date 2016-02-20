/*
 * A simple TCP echo client
 */

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "list.h"

#define SERV_PORT 12000
#define MAXLEN    2048
#define SERV_NAME "linuxremote1"
extern void insert(struct list_t * list, struct node_t * node);
extern struct node_t* make_node(char * word, int len, double rate);
extern void print_list( struct list_t *thislist );
int main(int argc, char *argv[])   {

  int s;                   /* socket descriptor                */
  //int len;                 /* length of received data          */
  //char msg[MAXLEN+1];      /* buffer to read info              */
  //int  msg_len;
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

  /* 
   * Send a message
   */
   /*
    * Send the message to the server and receive the echo back
    */
   //Make a List
   struct list_t *nodeList = (struct list_t *)malloc(sizeof(struct list_t));
   char* word1 = "If you think in terms of a year, plant a seed; if in terms of ten years, plant trees; if in terms of 100 years, teach the people.";
   char* word2 = "No matter how busy you may think you are, you must find time for reading, or surrender yourself to self-chosen ignorance.";
   char* word3 =  "Do not impose on others what you yourself do not desire.";
   char* word4 = "Our greatest glory is not in never falling, but in rising every time we fall.";
   char* word5 = "Wisdom, compassion, and courage are the three universally recognized moral qualities of human beings.";
   //printf("word1length:%d\n", (int)strlen(word1));
   insert(nodeList, make_node(word1, (int)strlen(word1), 3.50));
   insert(nodeList, make_node(word2, (int)strlen(word2), 4.50));
   insert(nodeList, make_node(word3, (int)strlen(word3), 5.50));
   insert(nodeList, make_node(word4, (int)strlen(word4), 6.50));
   insert(nodeList, make_node(word5, (int)strlen(word5), 7.50));
   //print_list(nodeList);
   int len =  nodeList->len;
   //printf("%d\n",len);
   
   struct node_t *current = nodeList->head;
  
   write(s, &len, 4);
  while (current != NULL) {
    write(s, current, 288);
    current = current->next;
    
  }

  close(s);
  return 0;
}
