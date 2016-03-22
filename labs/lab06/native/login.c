/* login - log into the system		Author: Patrick van Kleef */
/*                                              MINIX project     */
/* compile with -lcurses                                          */
/* modified X.M.                                                  */
/*   sep-8-2000 added shadow.h                                    */
/*              changed from getpwnam() to getspnam()             */
/* modified X.M.                                                  */
/*    feb-23-2003 to fit the needs of CSCI 363 lab                */
/* modified X.M.                                                  */
/*    apr-12-2013 to fit the needs of CSCI 363 lab                */
#include <signal.h>
#include <sgtty.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

/* 
 * The following two header files are revised as emulation for 
 * the original system one.
 * See the *.h files for some explanation.
 *
 * Xiannong Meng
 * 2013-04-07
 */
#include "pwd.h"
#include "shadow.h"  /* most systems now implement shadow passwd */

#define BUFLEN 64

int main(int argc, char* argv[]) {

  char    buf[BUFLEN],
    buf1[BUFLEN],
    *crypt();

  int     n, n1, bad;
  struct termios newtty, savedtty;
  struct passwd *pwd, *getpwnam();
  struct spwd *spwd;
  extern struct spwd *getspnam(); 

  /* save the original terminal setting */
  tcgetattr (0, &savedtty);
  newtty = savedtty;

  /* Get login name and passwd. */
  for (;;) {
    bad = 0;
    do {
      write(1,"CSCI363 login: ",15);
      n = read (0, buf, BUFLEN);
    } while (n < 2);
    buf[n - 1] = 0;  /* n-1 removes the newline char '\n' */

    /* Look up login/passwd. */
    if ((spwd = getspnam (buf)) == 0)
      bad++;
	  
    if (bad || strlen (spwd->sp_pwdp) != 0) {
      newtty.c_lflag &= ~ECHO;   // turn off echoing
      tcsetattr (0, TCSANOW, &newtty);
      write(1,"Password: ",10);
      n1 = read (0, buf1, BUFLEN);
      buf1[n1 - 1] = 0;
      write(1,"\n",1);

      /* retrieve and reset the original terminal setting */
      tcsetattr (0, TCSANOW, &savedtty);
      if (bad || strcmp (spwd->sp_pwdp, crypt(buf1, spwd->sp_pwdp))) {
	write (1,"Login incorrect\n",16);
	continue;  /* go back to the for() loop */
      }
    }

    /* Successful login. */
    pwd = getpwnam(buf);
    setgid (pwd->pw_gid);
    setuid (pwd->pw_uid);
    chdir (pwd->pw_dir);

    if (pwd->pw_shell) {
      execl(pwd->pw_shell, "-", (char*)0);
    }
    execl("/bin/sh", "-", (char*)0);  /* default shell */
    write(1,"exec failure\n",13);
  }
}
