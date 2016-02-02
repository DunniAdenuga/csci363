/*
 * Original version from Doug Comer's book "Internetworking with TCP/IP"
 * Revised for CSCI 363
 * Xiannong Meng <xmeng@bucknell.edu>
 * 2015-12-11
 */

/* webclient.c */

#define _GNU_SOURCE     // we want to use strcasestr()
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "tcplib.h"

#define BUFFSIZE	512
#define HTTPPORT        80

int make_request(int, char *[], char *);    // request service from web server

/*-----------------------------------------------------------------------
 *
 * Program: webclient
 * Purpose: fetch page from webserver and dump to stdout with headers
 * Usage:   webclient <compname> <path> [appnum]
 * Note:    Appnum is optional. If not specified the standard www appnum
 *          (80) is used.
 *
 *-----------------------------------------------------------------------
 */
int
main(int argc, char *argv[]) {

  if (argc < 4 || argc > 5) {
    (void) fprintf(stderr, "%s%s%s", "usage: ", argv[0], 
		   " <srvr_name> GET|HEAD <path> [app_port_num]\n");
    (void) fprintf(stderr, "for example %s www.cnn.com GET / 80\n",
		   argv[0]);
    exit(1);
  }

  char * get_loc = strcasestr(argv[2], "GET");
  char * head_loc = strcasestr(argv[2], "HEAD");
  int    which = -1;    // GET or HEAD
  char   *argv_cp[5];
  int    i;

  if (get_loc != NULL || head_loc != NULL)  {  // GET/HEAD request, fix argv's
    if (get_loc != NULL)
      which = 0;
    else if (head_loc != NULL)
      which = 1;

    argv_cp[0] = argv[0];          // program name, e.g., webclient
    argv_cp[1] = argv[1];          // web server name, e.g., polairs
    argv_cp[2] = argv[3];          // path, e.g., "/", skipping command get/head

    if (argc == 5)   // there is a 'port' argument
      argv_cp[3] = argv[4];

    argc --;
  }

  if (which == 1) {  // it's a HEAD request
    make_request(argc, argv_cp, "HEAD");
  } else {
    make_request(argc, argv_cp, "GET");
  }

  return 0;
}

/*-----------------------------------------------------------------------
 * Construct and send an HTTP request to the webserver
 *
 * parameters:
 *   argc:  number of arguments in the argument array
 *   argv:  the arguments in an array of strings
 *   svc:   what type of request, e.g., GET/HEAD or others
 *-----------------------------------------------------------------------
 */
int
make_request(int argc, char *argv[], char * svc)
{
  int	app;
  int	conn;
  char  * host;
  char  * path;
  char	buff[BUFFSIZE];
  int	len;

  /* convert arguments to binary computer and appnum */

  host = argv[1];
  path = argv[2];

  if (argc == 4) 
    app = atoi(argv[3]);
  else
    app = HTTPPORT;
	
  /* contact the web server */

  conn = socketClient(host, app);
  if (conn < 0) 
    exit(1);

  /* send an HTTP/1.1 request to the webserver */

  sprintf(buff, "%s %s HTTP/1.1\r\nHost: %s\r\n", svc, path, host);
  strcat(buff, "User-Agent: self-module\r\n");
  strcat(buff, "Accept: text/html,applicaiton/xhtml+xml\r\n");
  strcat(buff, "Accept-Language: en-us,en;q=0.5\r\n");
  //	strcat(buff, "Accept-Encoding: gzip,defalte\r\n");
  strcat(buff, "Accept-Charset: ISO-8859-1,utf-8;q=0.7\r\n");
  strcat(buff, "Keep-Alive: timeout=60\r\n");
  //  strcat(buff, "Connection: keep-alive\r\n");
  strcat(buff, "Connection: close\r\n");
  strcat(buff, "\r\n");

  len = strlen(buff);
  (void) send(conn, buff, len, 0);

  /* dump all data received from the server to stdout */

  while((len = recv(conn, buff, BUFFSIZE, 0)) > 0)
    (void) write(1, buff, len);

  return 0;
}
