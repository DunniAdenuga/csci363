/*
 * The program listens on multiple ports for client requests.
 * The library call 'select()' is used to detect if any requests are received
 * at any of the ports.
 *
 * For each request that is detected, the server spaws a process to handle the
 * request.
 *
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "tcplib.h"
#include <utmp.h>
#include <time.h>

#define TIMEPORT 14001
#define ECHO_PORT 14002
#define WHO 14102
#define STDIN 0

/* services supported */
void echoService(int);
void timeService(int);
int  socketUDPServer(int);
void getwhoService(int);

int main(int argc, char *argv[])   {

  int timeSock, echoSock, whoSock;   /* sockets on which the server listens */
  int timeWork, echoWork, whoWork;   /* sockets where the service is provided */
  int nfound;               /* number of connection requests found */
  int maxfdp1;              /* maximum number the server need to check */

  fd_set readmask;          /* file descriptor mask */
  
  /* create two sockets */
  timeSock = socketUDPServer(TIMEPORT);
  echoSock = socketServer(ECHO_PORT);
  whoSock = socketServer(WHO);
  /* initialize the readmask to be all zero */
  FD_ZERO(&readmask);
  /* TODO: call proper macro to initalize the fd_set readmask variable */
  FD_SET(STDIN, &readmask);
  /* servers' main loop */
  for (; ;)    {

    /* 
     * TODO: call proper macro to set two socket descriptor in readmask 
     * 1. set timeSock in mask
     * 2. set echoSock in mask
     */
    FD_SET(timeSock, &readmask);
    FD_SET(echoSock, &readmask);
    FD_SET(whoSock, &readmask);
    maxfdp1 = whoSock + 1;

    /* check how many are ready */

    /* 
     * TODO: call select() to check how many socket descriptors are ready
     * set that value in nfound
     */
    nfound = select(maxfdp1, &readmask, NULL, NULL, NULL);
    if (nfound < 0)  {
      perror("select error");
      exit(3);
    }
	  
    /* check which socket is read */
    int timeReady = 0;
    int echoReady = 0;
    int whoReady = 0; 
    /* TODO: check if timeSock is ready, set the value in timeReady */
    if(FD_ISSET(timeSock, &readmask)){
	timeReady = 1;
      }
    /* TODO: check if echoSock is ready, set the value in echoReady */
    if(FD_ISSET(echoSock, &readmask)){
	echoReady = 1;
      }
    if(FD_ISSET(whoSock, &readmask)){
	whoReady = 1;
      }
      if (timeReady){
      timeWork = timeSock;
      }
      if (echoReady){
      echoWork = acceptConn(echoSock);
      }
      if (whoReady){
      whoWork = acceptConn(whoSock);
      }

    int pid = fork();
    if (pid == 0) { // child
      if (timeReady) {
	printf("time service requested\n");
	fflush(stdout);
	timeService(timeWork);
	close(timeWork);
      }

      if (echoReady) {
	printf("echo service requested\n");
	fflush(stdout);
	echoService(echoWork);
	shutdown(echoWork, 2);
      }
      if (whoReady) {
	printf("getwho service requested\n");
	fflush(stdout);
	getwhoService(whoWork);
	//shutdown(whoWork, 2);
      }
      exit(0);
    } else { // end of child process
      //wait(NULL); // so the server exits gracefully
    }
    // end of server loop
       // end of program
      
      
  }
}
#include <sys/time.h>

/* time service code */
void timeService(int sock) {

  struct sockaddr_in cl;
  int len = sizeof(struct sockaddr_in);

  int request;
  int bytes_received = recvfrom(sock, &request, sizeof(int), 
				0, (struct sockaddr *)&cl, (socklen_t *)&len);

  printf("bytes received %d\n", bytes_received);

  struct timeval time;
  gettimeofday(&time, NULL);
     
  /**
     printf("%s", ctime((time_t *)&(time.tv_sec)));// ctime() converts UNIX
     printf("%u\n",time.tv_sec);
  **/

  /* If you prefer the time since 1/1/1900, add the magic number */
  //  long n = time.tv_sec + 2208988800; // make it since 1/1/1900

  /* Otherwise, don't adjust */
  long n = time.tv_sec;  // number of seconds since 1/1/1970
     
  n = htonl(n);
  
  sendto(sock, &n, sizeof(long), 0, (struct sockaddr *)&cl, len);
}

/* echo service code */
void echoService(int sock)  {

  char echo_buf[BUFSIZ+10];
  char buf[BUFSIZ+1];
  strcpy(echo_buf, "Echo: ");
  int  n = read(sock, buf, BUFSIZ);
  buf[n] = 0;
  strcat(echo_buf, buf);
  write(sock, echo_buf, strlen(echo_buf));
}

/* echo service code */
void getwhoService(int sock)  {
  
  char buf[BUFSIZ+1];
  int count = 0;
  char word[50];
  int size;
  read(sock, &size, 4);
  read(sock, word, size);
  printf("%s\n", word);
  struct utmp * one_user;
  setutent();
  one_user = getutent();

  while (one_user != NULL) {
    /* TODO: get and print the user name from the structure */
    if(one_user->ut_type == USER_PROCESS)
      {
	strcat(buf, one_user->ut_user);
	strcat(buf, "\n");
      }
    count ++;
    one_user = getutent();
  }

  endutent();

  strcat(buf, "Number of entries encountered: ");
  char* inter = (char *)malloc(10);
  sprintf(inter, "%d", count);
  strcat(buf, inter);
  //strcat(buf, "\n");
  
  int len = strlen(buf);
  
  buf[len] = 0;
  write(sock, &len, 4);
  write(sock, buf, len);
  close(sock);
}

/*
 * Create a UDP server socket and get it ready
 * for receiving.
 * 
 * parameters:
 *    int port: port number
 * 
 * return:
 *    int socket that is ready to receive data
 */
int socketUDPServer(int port) {

   int s;                   /* socket descriptor                */
   struct sockaddr_in sa;

   /*
    * Allocate an open socket.
    */
   if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
     perror("UDP socket error.");
     exit(1);
   }

   /*
    * Set up socket address data for the server
    */
   memset((char *)&sa, 0, sizeof(struct sockaddr_in));
   sa.sin_family      = AF_INET;
   sa.sin_addr.s_addr = htonl(INADDR_ANY);
   sa.sin_port        = htons(TIMEPORT);

   /*
    * Bind the socket to the service
    */
   if (bind(s, (struct sockaddr *)(&sa), sizeof(struct sockaddr_in)) < 0) {
     perror("UDP bind error.");
     exit(1);
   }

   return s;
}
