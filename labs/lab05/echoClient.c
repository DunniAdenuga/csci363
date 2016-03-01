/*
 * client.c - main 
 */

/*
 * The echo client connects to a sever, sends a message to the server
 * and waits for the server sends the message back.
 *
 * Upon receivig the echo, the client prints the message to the screen.
 *
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>
#include "tcplib.h"
#define ECHOPORT     14002 /* a non-privilege port so all can use */
#define BUFFSIZE     1024

int main(int argc, char *argv[])
{
   int s;                   /* socket descriptor                */
   int len;                 /* length of received data          */
   char buf[BUFFSIZE+1];    /* buffer to read echo info         */
   char * prompt = "Type a message : ";

   if (argc != 2)   {
      fprintf(stderr,"usage %s hostname\n",argv[0]);
      exit(1);
   }
   s = socketClient(argv[1], ECHOPORT);

   /* Prompt */
   write(1, prompt, strlen(prompt));

   /* Read from keyboard */
   len = read(0, buf, BUFFSIZE);
   buf[len] = 0;

   /* Send the message  */
   if (write(s, buf, len) != len) {
      perror("write");
      exit(1);
   }

   /*
    * Read the reply and put to user's output
    */
   while ( (len = read(s,buf,BUFFSIZE)) > 0)
      write(1,buf,len);

   close(s);

   return 0;
}
