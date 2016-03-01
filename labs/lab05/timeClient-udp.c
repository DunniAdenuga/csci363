/*
 * The time client requests a time reading from the server and
 * prints the results on screen as an integer as well as as a ctime()
 * string.
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define TIMEPORT 14001  // customer designed port

int main(int argc, char *argv[])  {

     int s, cc;
     long time;
     char * time_host;
     struct addrinfo *ptrh;
     struct sockaddr_in  sa;  /* socket addr. structure, server   */
     struct sockaddr_in  cl;  /* socket addr. structure, client   */

    
     if (argc != 2)  {
       fprintf(stderr, "usage : %s host\n", argv[0]);
       exit(1);
     }

     time_host = argv[1];

     /*
      * Allocate an open socket.
      */
     if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
       perror("UDP socket error.");
       exit(1);
     }

     /*
      * Get server host information
      */
     getaddrinfo(time_host, NULL, NULL, &ptrh);

     /*
      * Set up socket address data for the server
      */
     memset((char *)&sa, 0, sizeof(sa));
     memcpy(&sa, (struct sockaddr_in *)(ptrh->ai_addr), ptrh->ai_addrlen);
     sa.sin_family      = AF_INET;
     sa.sin_port        = htons(TIMEPORT);

     /*
      * Set up socket address data for the client
      */
     memset((char *)&cl, 0, sizeof(cl));
     cl.sin_family      = AF_INET;
     cl.sin_addr.s_addr = htonl(INADDR_ANY);
     /* port 0 allows the system to assign any available port */
     cl.sin_port        = htons(0);

     /*
      * Bind the socket to the client
      */
     if (bind(s, (struct sockaddr *)(&cl), sizeof cl) < 0) {
       perror("UDP client bind error.");
       exit(1);
     }


     /* Send in a request, the exact content isn't critical */
     int request = 1;
     sendto(s, &request, sizeof(int), 0, (struct sockaddr *)&sa, sizeof(sa));

     /* Wait for the response                                               */
     /* The expected result is an long int representing the sec since apoch */
     cc = recvfrom(s, &time, sizeof(long), 0, NULL, NULL);
     if (cc < 0) {
       perror("remotetime: recvfrom");
       exit(1);
     }

     printf("time : %u %u\n", time, ntohl(time));
     /* Returned value is a long giving number of seconds since          */
     /* Jan 1, 1900.  Need to convert to UNIX time, which is number of   */
     /* seconds since Jan 1, 1970.                                       */
     //     time = ntohl(time) - 2208988800;    /* magic constant to convert    */
                                         /* ... network time to UNIX     */
                                         /* ... time                     */
    
     /* 
      * Check the server implementation. If the server doesn't
      * adjust its value to Jan 1, 1900, then do the following.
      * Otherwise do the above by adding the magic number.
      */
     time = ntohl(time);
     /* ctime() converts UNIX time   */
     /* to character string          */
     char * mytime = ctime((time_t *)&time);
     printf("%s", mytime);

     close(s);
}

