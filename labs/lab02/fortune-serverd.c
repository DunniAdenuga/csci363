/*
 * Copyright (c) 2012 Bucknell University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: L. Felipe Perrone (perrone@bucknell.edu)
 *
 * Revised by X. Meng
 * -- 2015-01-31: change from a file server to a fortune cookie server.
 */
 
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
#include <signal.h>
#include "readn.h"
#include "writen.h"

#define BUFFSIZE     1024   // maximum length of a fortune cookie
#define REQTSIZE       32   // maximum length of the request string
#define	QLEN		6   // size of request queue
#define MAX_REQ        10 // maximum number of requests
// The implementation file is in cookies.c, these are functions needed
extern void init_cookies(char *);
extern char * get_cookie(int); 
extern int  count_cookies(void);
extern ssize_t				
readn(int, void *, size_t n);
extern ssize_t						
writen(int fd, const void *vptr, size_t n);

int count = 0;
int cookieIndex[100];
int cookieIndexCount[100];
void process_request(int);
int searchArray(int[], int);
void fillArray(int *);
int Fork(void);

/*------------------------------------------------------------------------
 * Program:   fortune-cookie-server
 *
 * Purpose:   allocate a socket and then repeatedly execute the following:
 *		(1) wait for the next connection from a client
 *		(2) send a random fortune cookie to the client
 *		(3) close the connection
 *		(4) go back to step (1)
 *
 * Syntax:    fortune-server [ port ]
 *
 *		 port  - protocol port number to use
 *
 *------------------------------------------------------------------------
 */

int main(int argc, char* argv[]) {
  
  struct protoent *ptrp;	 // pointer to a protocol table entry	
  struct sockaddr_in sad;        // structure to hold server's address	
  struct sockaddr_in cad;        // structure to hold client's address	
  int sd, sd2;	                 // socket descriptors			
  //int n;                         // byte count
  int port;		         // protocol port number		
  int alen;		         // length of address	
  pid_t pid[MAX_REQ];
  int request = 0; //number of processes
  // Check command-line argument for protocol port.
  
  if (argc > 1) {			
    port = atoi(argv[1]);	        
  }
  else {
    printf("Usage: fileserver [ port ]\n");
    exit(-1);
  }

  if (port <= 0) {
    // test for illegal value	
    // print error message and exit	
    fprintf(stderr,"SERVER: bad port number %s\n",argv[1]);
    exit(1);
  }
 
  memset((char *)&sad,0,sizeof(sad)); // clear sockaddr structure	
  sad.sin_port = htons((unsigned short)port);
  sad.sin_family = AF_INET;	      // set family to Internet	        
  sad.sin_addr.s_addr = INADDR_ANY;   // set the local IP address	
  
  // Map TCP transport protocol name to protocol number 
  //    --> Look at /etc/protocols
  ptrp = getprotobyname("tcp");
  if ( ptrp == NULL) {
    fprintf(stderr, "SERVER: cannot map \"tcp\" to protocol number");
    exit(1);
  }
  
  // Create a socket 
  sd = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
  if (sd < 0) {
    fprintf(stderr, "SERVER: socket creation failed\n");
    exit(1);
  }
  
  // Bind a local address to the socket 
  if (bind(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
    fprintf(stderr,"SERVER: bind failed\n");
    exit(1);
  }
  
   // Specify size of request queue 
  if (listen(sd, QLEN) < 0) {
    fprintf(stderr,"SERVER: listen failed\n");
    exit(1);
  }

  // Create the fortune cookies
  init_cookies("cookies.txt");

  fillArray(cookieIndex);
  fillArray(cookieIndexCount);

  // Main server loop - accept and handle requests 
  while (1) {
    
   
    alen = sizeof(cad);
    
    if ((sd2=accept(sd, (struct sockaddr *)&cad, (socklen_t*)(&alen))) < 0) {
      fprintf(stderr, "SERVER: accept failed\n");
      exit(1);
    }
    request++;
    
    if(request < MAX_REQ){
    // Serve use requests
    pid[request-1] = Fork();
    sleep(5000);
    
   

    if(getpid() == 0){
      process_request(sd2);
      request--;
      close(sd2);
      _exit(SIGCHLD);
      }
    //close(sd2);
    }
  }
}

/*
 *------------------------------------------------------
 * process_request: accept and process client requests
 *------------------------------------------------------
 */
void process_request(int sock)  {
  char* result = malloc(BUFFSIZE); 
  int index;                     // index to cookie list
  char * cookie;                    // pointer to buffer for fortune cookie
  char buf[REQTSIZE];         // buffer for client request
  int highestMax = 0;
  // allocate cookie buffer
  cookie = malloc(BUFFSIZE);
  
  int num = 0;
  
  /*
    TO-DO
    The server algorithm is as follows:

    read the request from a client;
    
    if the request is 'cook'
       generate a random index within the range;
       retrieve the fortune cookie at that index; // use get_cookie(i)
       return the text of the fortune cookie to the client;
  */
  //int r;
  
  
  srandom((int)time(NULL));     // initalize random number generator

  //r = random() % NUM_WORD;
  int lent;
  readn(sock,&lent, 4);
  readn(sock,buf, lent);
 
  if(strcmp(buf, "cook")== 0){
   
    index = random() % count_cookies();
   
    cookie = get_cookie(index);
    
    int check = searchArray(cookieIndex, index);
    
    if(check  == -1){
      cookieIndex[count] = index;
     
      cookieIndexCount[count]++;
      
      count++;
      
    }
    else
      {
	
	cookieIndexCount[searchArray(cookieIndex, index)]++;
      }
    size_t len = strlen(cookie)+1;
    writen(sock,&len , 4);
    
    writen(sock, cookie, len+1);
  }
  
  else if(strcmp(buf, "stat") == 0){
    if(count != 0){
    for(int i = 0; i < count; i++){
      
      if(cookieIndexCount[i] > highestMax){
	highestMax = cookieIndexCount[i];
	num = i;
	
}
    }
    
     strcat(result, "The cookie with the highest frequency is: \n");
    strcat(result, get_cookie(cookieIndex[num]));
    strcat(result, "\n The freuency is: ");
    int x = cookieIndexCount[num]; 
    char stuff[5];
    sprintf(stuff, "%d", x);
    strcat(result,  stuff);
    strcat(result, ". \n ");
  //free(cookie);
    }
    else{
      strcat(result, "No fortune-cookie has been accessed today.\n");
    }
    int len2 = strlen(result)+ 1;
    writen(sock, &len2,4);
    writen(sock, result, len2);
  close(sock);
}
}

  int searchArray(int array[], int target){
    for(int i =0; i <= count; i++){
      if(array[i] == target){
	return i;
	  }
    }
    return -1;
  }

void fillArray(int array[]){
  for(int i = 0; i < sizeof(array); i++){
    array[i] = 0;
}
};

int Fork(void)
{
  int pidy = fork();
  if(-1 == pidy)
{
  perror("Error! No child process was created.");
  exit(-1);
}
  return pidy;
}
