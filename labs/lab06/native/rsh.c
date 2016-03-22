/* rsh - remote shell service                                     */
/* compile with -lcurses                                          */
/* modified X.M.                                                  */
/*    feb-23-2003 to fit the needs of CSCI 363 lab                */
/*
 * Revised 2013-04-07 to add a local version of "pwd.h"
 * Xiannong Meng
 * for CSCI 363
 */
#include <signal.h>
#include <sgtty.h>
#include <unistd.h>
#include <stdlib.h>

/* 
 * The following header file has been modified for student use
 * See the header file itself for explanation.
 */
#include "pwd.h"

/* 
 * The following header file is customized, not the original one
 * See the header file for explanation.
 */
#include "shadow.h"  /* most systems now implement shadow passwd */

void rshService(int sock) 
{
  /* close stdin, stdout, and stderr */
  close(0); close(1); close(2);

  /* duplicate stdin, stdout, stderr as 'sock', the network device */
  if (dup(sock) != 0 || dup(sock) != 1 || dup(sock) != 2)
  {
    exit(2);
  }
  //execl("/bin/tcsh", "-", (char*)0);
  execl("/bin/bash", "-", (char*)0);
  // we shouldn't reach here if shell is okay
  exit(1);
}
