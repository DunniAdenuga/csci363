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
 */

/*
 *  Revision:
 *  Xiannong Meng 01-07-2013
 *  -- Change the use of 'gethostbyname' to 'getaddrinfo'
 */ 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "readn.h"
#include "writen.h"

extern ssize_t				
readn(int, void *, size_t n);
extern ssize_t						
writen(int fd, const void *vptr, size_t n);
extern int errno;
#define MAX 999
/*------------------------------------------------------------------------
 * Program:   getfile 
 *
 * Purpose:   allocate a socket, connect to a server, transfer requested
 *            file to local host, and print file contents to stdout
 *
 * Syntax:    getfile [ host ] [ port ] [ file ] 
 *
 *		 host  - name of a computer on which server is executing
 *		 port  - protocol port number server is using
 *               file  - complete path of file to retrieve
 *
 *------------------------------------------------------------------------
 */

int main(int argc, char* argv[]) {

  struct addrinfo *ptrh; // pointer to a host table entry	
  struct protoent *ptrp; // pointer to a protocol table entry	
  struct sockaddr_in sad; // structure to hold an IP address	
  int   status;          // calling status
  int	sd;		 // socket descriptor			
  int	port;		 // protocol port number		
  char	*host;		 // pointer to host name		
  char	buf[1000];	 // buffer for data from the server	
  //int   bytes_read;      // number of bytes read from socket
  
  // Check command-line arguments
  if (argc < 3) {
    printf("usage: %s [ host ] [ port ]\n", argv[0]);
    exit(-1);
  }

  host = argv[1];		
  port = atoi(argv[2]);	

  if (port <= 0)  {
    // test for legal value		
    // print error message and exit	
    fprintf(stderr,"%s: bad port number %s\n",argv[0], argv[2]);
    exit(1);
  }

  // Convert host name to equivalent IP address and copy to 'sad'. 
  status = getaddrinfo(host, NULL, NULL, &ptrh);
  if ( status != 0 ) {
    fprintf(stderr,"%s: invalid host: %s\n", argv[0], host);
    exit(1);
  }

  memset((char *)&sad,0,sizeof(sad)); // clear sockaddr structure	
  memcpy(&sad, (struct sockaddr_in *)(ptrh->ai_addr), ptrh->ai_addrlen);
  sad.sin_port = htons((unsigned short)port);

  // Map TCP transport protocol name to protocol number. 
  ptrp = getprotobyname("tcp");
  if ( ptrp == NULL ) {
    fprintf(stderr, "%s: cannot map \"tcp\" to protocol number\n", argv[0]);
    exit(1);
  }
  
  // Create a socket. 
  sd = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
  if (sd < 0) {
    fprintf(stderr, "%s: socket creation failed\n", argv[0]);
    exit(1);
  }
  
  // Connect the socket to the specified server. 
  if (connect(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
    fprintf(stderr,"%s: connect failed\n", argv[0]);
    exit(1);
  }

  /*
   * TO-DO
   */
  /*
    Build and send the request to the server;
    
    Read back the furtune cookie from the server;
    Print it on the screen;
  */
  int size_read = 0;
  int x = strlen("cook")+ 1;
  writen(sd, &x, 4);
  writen(sd, "cook", x);
  readn(sd, &size_read, 4);
  
  readn(sd, buf, size_read);

  printf("From the server:\n %s\n", buf);
  
  // Close the socket.   
  close(sd);
  
  // Terminate the client program gracefully. 
  exit(0);
}
