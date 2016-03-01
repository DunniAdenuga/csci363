/*
 * getwho enquires the utmp database for the the users who are currently
 * on the system.
 *
 * X. Meng for CSCI 363 lab
 * 2013-02-22
 */
#include <stdio.h>
#include <utmp.h>
#include <time.h>

int main(int argc, char *argv[]) {

  int count = 0;
  struct utmp * one_user;
  setutent();
  one_user = getutent();

  while (one_user != NULL) {
    /* TODO: get and print the user name from the structure */
    if(one_user->ut_type == USER_PROCESS)
      {
	printf("%s\n", one_user->ut_user);
      }
    count ++;
    one_user = getutent();
  }

  endutent();

  printf("a total of %d entries was encounted\n", count);

  return 0;
}
