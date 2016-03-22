/* rshClient - remote shell client                                */
/*                                                                */
/* compile with -lcurses                                          */
#include <stdio.h>
#include <signal.h>
#include <sgtty.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "tcplib.h"

#define RSHPORT 14102
#define BUFLEN 30
int x;
int main(int argc, char *argv[])
{
  void passAll(int, char*);

  int sock;
  char *host;
  //  struct sgttyb args;
  char prompt[BUFSIZ];

  if (argc != 2)
  {
    fprintf(stderr,"usage : %s server-name\n", argv[0]);
    exit(1);
  }

  host = argv[1];
  do{
  sock = socketClient(host, RSHPORT);
  if (sock <= 0)
  {
    fprintf(stderr,"connection failed\n");
    exit(2);
  }


  /* build the prompt */
  strcpy(prompt, host);
  strcat(prompt, "% ");

  passAll(sock, prompt);
  }while(x == 0);
  return 0;
}

void passAll(int sock, char * prompt)
{
  int maxfdp1, nfound, nread;
  char buff[BUFSIZ+1];
  //char buff1[BUFSIZ+1];
  char buff2[BUFSIZ+1];
  char userName[BUFSIZ+1];
  char* passWord;
  fd_set readmask;
  int n = strlen(prompt);
  x = 0;
  int ans;
  
    write(1,"CSCI363 login: ",15);
    nread = read(0, userName, BUFSIZ);
    write(sock, userName, strlen(userName));

    //write(1,"Password: ",15);
    //nread = read(0, passWord, BUFSIZ);
    passWord = readline("Password: ");
    //printf("word - %s\n",passWord);
    write(sock, passWord, strlen(passWord));
    
    read(sock, &ans, 4);
    //printf("ans - %d\n", ans);
    if(ans==1){
      x = 1;
    for (; ;){
    write(1, prompt, n);
    FD_ZERO(&readmask);
    FD_SET(0, &readmask);
    FD_SET(sock, &readmask);
    maxfdp1 = sock + 1;

    nfound = select(maxfdp1, &readmask, (fd_set *)0, (fd_set*)0,
		    (struct timeval *)0);

    if (nfound < 0)
    {
      exit(3);
    }
    
    
    if (FD_ISSET(0, &readmask))
    {
      /* 
       * check to see if stdin has data, if so read and pass it to the 
       * network server rshd.
       */
      nread = read(0, buff, BUFSIZ);
      if (nread < 0)
	exit(4);
      else if (nread == 0)
	exit(0);
	//break;

      if (write(sock, buff, nread) != nread)
	exit(5);
    }
    
      /* 
       * check to see if socket has data, if so read and pass it to the 
       * local screen.
       */
   if (FD_ISSET(sock, &readmask))
    {
      write(1, "\n", 1);
      nread = read(sock, buff2, BUFSIZ);
      if (nread <= 0)
	break;
      if (write(1, buff2, nread) != nread)
	exit(6);
    }
    }
    }
  
}
