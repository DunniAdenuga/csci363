/*
 * rshd.c - main
 */
#include <signal.h>
#include <sgtty.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "tcplib.h"
#include "pwd.h"
#include <shadow.h>  /* most systems now implement shadow passwd */

#define BUFLEN 64
#define RSHPORT         14102       /* a non-privilege port so all can use */
#define MAX             1024

int result;
void authenticate(char* , char* );
int main(int argc,char *argv[])
{
  int s, t, n1, n2;
    int pid;
    extern void rshService(int);
    char userName[MAX];
    char passWord[MAX];
    
    s = socketServer(RSHPORT);
    while (1)
    {
      //printf("here\n");
      if (s > 0){
	    t = acceptConn(s);
	    //printf("t - %d\n", t);
      }
	else
	{
	    fprintf(stderr," socket error\n");
	    exit(1);
	}
      //printf("t - %d\n", t);
	if (t > 0)
	{
	  
	  pid = fork();
	  if (pid == 0) /* child process continue */
	    {
	     
	      n1 = read(t, userName, MAX);
	      userName[n1 - 1] = 0;
	      //printf("%s\n", userName);
	      n2 = read(t, passWord, MAX);
	      passWord[n2] = 0;
	      //printf("%s\n", passWord);
	      authenticate(userName, passWord);
	      if(result == 1){
		//printf("%d\n", result);
		write(t, &result, 4);
		rshService(t);
		shutdown(t, 2);
	      //close(t);
	      }
	      else if(result == 0){
		//printf("%d\n", result);
		write(t, &result, 4);
		close(t);
		continue;
	      }
	      
	      //exit(0);
	    }
	}
	else
	    fprintf(stderr," connection error\n");
    } /* while(1) */
    close(s);
    return 0;
}

void authenticate(char* user , char* pass){
  //char    buf[BUFLEN],buf1[BUFLEN],
   char  *crypt();

//int     n, n1, 
  int bad;
  struct termios newtty, savedtty;
  struct passwd *pwd, *getpwnam();
  struct spwd *spwd;
  //extern struct spwd *getspnam(); 

  /* save the original terminal setting */
  tcgetattr (0, &savedtty);
  newtty = savedtty;

  /* Get login name and passwd. */
  //for (;;) {
    bad = 0;
    

    /* Look up login/passwd. */
    if ((spwd = getspnam (user)) == 0)
      bad++;
	  
    if (bad || strlen (spwd->sp_pwdp) != 0) {
      newtty.c_lflag &= ~ECHO;   // turn off echoing
      tcsetattr (0, TCSANOW, &newtty);
      

      /* retrieve and reset the original terminal setting */
      tcsetattr (0, TCSANOW, &savedtty);
      if (bad || strcmp (spwd->sp_pwdp, crypt(pass, spwd->sp_pwdp))) {
	write (1,"Login Incorrect\n",16);
	//return "false";
	result=0;
	//printf("%d\n", result);
	return;
	//continue;  /* go back to the for() loop */
      }
    }
   
    /* Successful login. */
    pwd = getpwnam(user);
    setgid (pwd->pw_gid);
    setuid (pwd->pw_uid);
    chdir (pwd->pw_dir);
    result = 1;
    printf("Login Correct\n");
    //printf("%d\n", result);
    
    /*if (pwd->pw_shell) {
      execl(pwd->pw_shell, "-", (char*)0);
      }*/
    //execl("/bin/sh", "-", (char*)0);  /* default shell */
    //write(1,"exec failure\n",13);
    //}
}
