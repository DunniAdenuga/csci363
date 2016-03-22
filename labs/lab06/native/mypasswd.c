/*
 * The program emulates the behavior of the system program /usr/bin/passwd.
 *
 * The program resets a user password that is in the "shadow" file.
 * Before a password can be reset, a user name must exist in the "passwd"
 * file.
 *
 * Assumption: both "passwd" and "shadow" exist in current directory.
 *
 * For CSCI 363 lab exercises
 * Spring 2013
 * Xiannong Meng
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#include "shadow.h"
#include "pwd.h"

enum bool {false, true};

int main(int argc, char * argv[])
{
     struct passwd  *pwd;
     extern struct spwd *getspnam();
     char * getNewPasswd();
     char * crypt(const char *key, const char *salt);
     char * getRandomChars(int);
     char * user, tempUser[30];
     char * password, *savedPwd, *encryptedPwd;
     char line[BUFSIZ+1], newLine[BUFSIZ+1];
     FILE *fp;
     char *pwBegin, *pwEnd;
     enum bool found, sawNL;
     int len, lineLen;

     if (argc != 2)
     {
	  fprintf(stderr," usage : %s usrname\n", argv[0]);
	  exit(1);
     }
     user = argv[1];

     /* now we have the user name */
     /* first check if it is in the /etc/passwd file */
     pwd = getpwnam(user);
     if (pwd == 0)
     {
	  fprintf(stderr, "sorry %s is not a valid user\n", user);
	  exit(2);
     }
     
     /* locate the user in the shadow file */
     len = strlen(user);
     fp = fopen("shadow", "r+");
     if (fp == NULL)
     {
	  fprintf(stderr, "sorry shadow must exist\n");
	  exit(3);
     }
     found = false;
     while (fgets(line, BUFSIZ, fp) && !found)
     {
	  strncpy(tempUser, line, len);
	  tempUser[len] = 0;
	  if (strcmp(user, tempUser) == 0)
	  {
	       found = true;
	       break;
	  }
     }

     savedPwd = (char*)malloc(30);
     savedPwd[0] = 0;
     if (found)  // an existing user and password
     {
	  lineLen = strlen(line);
	  if (line[lineLen-1] == '\n')   // get rid of it
	  {
	       sawNL = true;
	       line[lineLen-1] = 0;
	  }
	  else
	       sawNL = false;
	  lineLen = strlen(line);
	  pwBegin = strchr(line, ':');
	  pwEnd = strchr(pwBegin+1, ':');
	  if ((pwEnd - pwBegin) > 1) // remove the existing password
	  {
	       char * current = pwBegin + 1;
	       strncpy(savedPwd, current, (pwEnd - pwBegin));
	       savedPwd[pwEnd-pwBegin-1] = 0;
	       strncpy(current, pwEnd, (lineLen - (pwEnd - line)));
	       current[(lineLen - (pwEnd - line))] = 0;
	  }
     }
     else        // new password
     {
	  sprintf(line, "%s::others", user);
	  sawNL = false;
	  pwBegin = strchr(line, ':');
	  lineLen = 0; // new line
     }

     /* at this point, the 'line' holds the necessary information, we need to
     read the new password and put into thel 'line' and write it back to the
     file*/
     password = getNewPasswd();
     if (savedPwd[0] == 0)
     {
	  savedPwd = getRandomChars(2);
/**
	  tmpnam(savedPwd);
	  savedPwd += 9;   // skip "/var/tmp/"
	  printf("var : %s\n", savedPwd);
**/
     }
     encryptedPwd = crypt(password, savedPwd);
     
     /* put the password back into the 'line' first */
     strcpy(newLine, user);
     strncat(newLine, ":", 1);
     strcat(newLine, encryptedPwd);
     strcat(newLine, pwBegin+1);
     strcat(newLine, "\n");
     /* locate the proper place */
     if (sawNL == true)
	  lineLen ++;
     fseek(fp, -lineLen, SEEK_CUR);
     /* then set the password in the shadow file */
     fputs(newLine, fp);
     
     fclose(fp);

     return 0;
}

char * getNewPasswd()
{
     struct termios newtty, savedtty;
     char * password, verifier[30];
     int len;

     password = (char *)malloc(30);
     /* first turn off the echo mode */
     //     ioctl(0, TIOCGETP, &savedtty);
     tcgetattr(0, &savedtty);
     newtty = savedtty;
     //     newtty.sg_flags &= ~ECHO;
     newtty.c_lflag &= ~ECHO;

     /* read the password from the user */
     do
     {
	  do 
	  {
	       write(1,"Password: ",10);
	       //	       ioctl(0, TIOCSETP, &newtty);
	       tcsetattr(0, TCSANOW, &newtty);
	       len = read (0, password, 30);
	       //	       ioctl(0, TIOCSETP, &savedtty);
	       tcsetattr(0, TCSANOW, &newtty);
	       write(1, "\n", 1);
	  } while (len < 2);
	  len --;
	  password[len] = 0;
	  do 
	  {
	       write(1,"Re-enter the password: ",22);
	       //	       ioctl(0, TIOCSETP, &newtty);
	       tcsetattr(0, TCSANOW, &newtty);
	       len = read (0, verifier, 30);
	       //	       ioctl(0, TIOCSETP, &savedtty);
	       tcsetattr(0, TCSANOW, &savedtty);
	       write(1, "\n", 1);
	  } while (len < 2);
	  len --;
	  verifier[len] = 0;
     } while (strcmp(password, verifier));

     return password;
}

#include <sys/time.h>

char * getRandomChars(int length)
{
     struct timeval time;
     char *str;
     int i;

     gettimeofday(&time, 0);
     srandom(time.tv_sec);

     str = (char *)malloc(length+1);
     for (i = 0; i < length; i ++)
	  str[i] = random() % ('~' - ';') + (int)';';
     str[length] = 0;
     
     return str;
}
