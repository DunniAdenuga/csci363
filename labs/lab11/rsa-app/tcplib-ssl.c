/*
 * tcplib.c: this file contains necessary functions to establish TCP
 * connection using socket interface under Solaris.
 * Xiannong Meng
 * apr-22-1999
 * modified dec-15-2000, make socket reusable, call setsockopt();
 *
 * Revised 
 * 02-22-2016 
 * by Xiannong Meng
 * to include "secure write" and "secure read" that
 * incorporate the use of RSA encryption/decryption functions.
 */

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "tcplib-ssl.h"
#include "read-writen.h"

/*
 * global constants 
 */
#define BUF 9688
#define KEYLEN  2048
int ON = 1;
int OFF = -1;
int encrypted_length ;
int decrypted_length ;
int* tempE;
int* tempD;
unsigned char  encrypted[KEYLEN]={};
unsigned char  decrypted[KEYLEN]={};
extern char * public_key;   /* this is a global static value*/
extern char * private_key;   /* this is a global static value*/
char* remote_public_key; //other party's public key
/*
 * int socketClient(char *host, int port)
 *  socketClient returns to the calling function a socket descriptor, it
 * returns a negative value if failed.
 *  host : Internet host name of the remote server
 *  port : the port number on which the server is listening
 * return code:
 *   -1  remote host does not exist
 *   -2  socket creation failed
 *   -3  remote host connection failed
 */
int socketClient(char *host, int port)
{
   int s;                   /* socket descriptor                */
   struct sockaddr_in  sa;  /* socket addr. structure           */
   struct hostent * hp;     /* host entry                       */

   /* 
    * Look up the specified hostname
    */
   if ((hp = gethostbyname(host)) == NULL)
       return NOHOST;  /* host does not exist? */
  
   /*
    * Put host's address and address type into socket structure
    */
   bcopy((char *)hp->h_addr, (char *)&sa.sin_addr, hp->h_length);
   sa.sin_family = hp->h_addrtype;
   
   /*
    * Put the whois socket number into the socket structure.
    */
   sa.sin_port = htons(port);
   bzero(&(sa.sin_zero), 8);  /* initialize */

   /*
    * Allocate an open socket.
    */
   if ((s = socket(hp->h_addrtype, SOCK_STREAM, 0)) < 0)
       return SOCKFAIL;

   /* set socket option to be reusable */
   if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (void*)&ON, sizeof(int)) < 0)
     return SOCKFAIL;

   /*
    * Connect to the remote server.
    */
   if (connect(s,(struct sockaddr *)(&sa),sizeof(struct sockaddr)) < 0)
       return CONNFAIL;
   
   return s;
}


/*
 * int socketServer(int port)
 * socketServer returns to the calling function a socket descriptor, it
 * returns a negative value if failed.
 *  port : the port number on which the server is listening
 * return code:
 *   -1  host does not exist
 *   -2  socket creation failed
 */
int socketServer(int port)
{
   int s;                   /* socket descriptor                */
   struct sockaddr_in  sa;  /* socket addr. structure           */
   struct hostent * hp;     /* host entry                       */
   char localhost[MAXHOSTNAME+1];   /* local host name as character string */


   /*
    * Get our own host information
    */
   gethostname(localhost,MAXHOSTNAME);
   if ((hp = gethostbyname(localhost)) == NULL)
       return NOHOST;
   
   sa.sin_port = htons(port);
   bcopy((char *)hp->h_addr,(char *)&sa.sin_addr,hp->h_length);
   sa.sin_family = hp->h_addrtype;

   /*
    * The following line is added by X. Meng
    * without it, bind() fails.
    */
   sa.sin_addr.s_addr = htonl(INADDR_ANY);

   /*
    * Allocate an open socket for the incoming connections
    */
   if ((s = socket(hp->h_addrtype,SOCK_STREAM,0)) < 0)
       return SOCKFAIL;

   /* set socket option to be reusable */
   if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (void*)&ON, sizeof(int)) < 0)
     return SOCKFAIL;

   /*
    * Bind the socket to the service port so
    * we can hear incoming connection
    */
   if (bind(s,(struct sockaddr *)(&sa),sizeof sa) < 0)
	return BINDFAIL;

    /*
     * Set maximum connections we will fall behind
     */
   listen(s, BACKLOG);
   
   return s;
}


/*
 * int acceptConn(int s)
 *  accepts connection from a client and returns a socket ready for
 *  communications.
 */
int acceptConn(int s)
{
   struct sockaddr_in sa;      /* internet socket structure  */
   int i, t;

   i = sizeof sa;
   /*
    * We hang in accept() while waiting for new customers
    */
   if ((t = accept(s,(struct sockaddr *)(&sa),(socklen_t*)&i)) < 0)
       return ACPTFAIL;
   return t;
}

/*
 * Read RSA key from the file with the given name.
 * Return the key through the parameter "key", and
 * the length through function return value.
 */
int read_key(char * fname, int which) {

  int f = open(fname, O_RDONLY);

  if (f <= 0) {
    perror("open key file failed");
    exit(1);
  }

  struct stat fstat_buf;

  int status = fstat(f, &fstat_buf);
  if (status != 0) {
    perror("open key file failed");
    exit(1);
  }

  int len = fstat_buf.st_size;
  char * local_key = (char *)malloc(len+1);

  int ret_val = readn(f, local_key, len);

  close(f);

  if (len != ret_val) {
    perror("read key failed");
    exit(1);
  }
  
  if (which == PRIVATE) {
    private_key = (char *)malloc(len+1);
    strncpy(private_key, local_key, len);
    private_key[len] = 0;
  }  else {
    public_key = (char *)malloc(len+1);
    strncpy(public_key, local_key, len);
    public_key[len] = 0;
  }

  return ret_val;
}

/*
 * Read private key from a local file by the given name.
 * Store the key in the global variable 'private_key'
 *
 * Precondition: the private key is already stored in 
 * the file specified by 'fname'
 */
int load_private_key(char * fname) {

  int ret_val = read_key(fname, PRIVATE);

  return ret_val;
}

/*
 * Exchange public key with the remote host connected through
 * socket 'sock'.
 *
 * Send own public key to the remote host, and receive back
 * the public key of the remote host.
 *
 * Precondition: 
 *   1. Public key of this host is in a file specified
 *      by 'fname'
 *   2. The connection with the remote host has been established
 *      through socket 'sock'
 *
 * Postcondition:
 *   1. The public key of this host is sent to the remote host
 *   2. The public key of the remote host is received and stored
 *      in a global static variable 'public_key' defined in this file.
 */
int exchange_public_key(int sock, char * fname, int which) {
  
  /*
   * TODO
   *
   * 1. Read one's own public key into a buffer, you may use the
   *    'read_key' function defined above
   */
  int ret_val = read_key(fname, PUBLIC);
   /* 2. Exchange the public key with the communicating partner
   *    a. If this is the server, send the public key first, 
   *       then read the public key from the other party
   *    b. else if this is the client, the read/write is reversed.
   */
  
  int len;
  if(which == SERVER){
    write(sock, &ret_val, 4);
    write(sock, public_key, ret_val);
    read(sock, &len, 4);
    remote_public_key = malloc(len);
    read(sock, remote_public_key, len);
    strncpy(public_key, remote_public_key, len+1);
    //printf("remote-public-key: %s",remote_public_key); 
  }
    else{
      if(which == CLIENT){
    read(sock, &len, 4);
    remote_public_key = malloc(len+1);
    read(sock, remote_public_key, len);
    //printf("remote-public-key: %s",remote_public_key);
    write(sock, &ret_val, 4);
    write(sock, public_key, ret_val);
    strncpy(public_key, remote_public_key, len+1);
      }
    }
  /* 3. Return the key length
   */

  return len;   // need to return the actual value.
}

/*
 * writes(int sock, char * buf, int len)
 * Write 'len' bytes of data from 'buf' through 'sock'
 * in secure form. The data coming from 'buf' is plain
 * text, which has to be encrypted before sending
 * to the other party.
 */
int writes(int sock, char * buf, int len) {
  //int stuff;
  // stuff = (int)*buf;
  //printf("buf %d\n,", stuff); 
  //encrypted_length = malloc(4);
  encrypted_length  = encrypt((unsigned char *)buf, encrypted);
  //tempE = malloc(5);
  //*tempE = encrypted_length;
  printf("encrypt-len - %d\n", encrypted_length);
  //write(sock, &len, 4);
  int bytes_written = write(sock, encrypted, encrypted_length);
  //write(sock, &encrypted_length, encrypted_length);
  return encrypted_length;
}

/*
 * reads(int sock, char * buf, int len)
 * Read 'len' bytes of data into 'buf' through 'sock'
 * in secure form. The data stored in 'buf' should be in plain
 * text form to return to the calling function. The original
 * data read from  the other party is in encrypted form.
 * The reads() function is responsible for decrypting all
 * data.
 */
int reads(int sock, char * buf, int len) {
  char* tempBuf = malloc(BUF);
  int length = read(sock, tempBuf, BUF);
  //read(sock, &encrypted_length, 4);
 //printf("buf - %s\n", buf);
 //printf("encrypt-len - %d\n", *tempE);
 //decrypted_length = malloc(4);
  //printf("tempBuf %s\n", tempBuf);
  //printf("length %d\n", length);
  //printf("length %d\n", encrypted_length);
 decrypted_length = decrypt((unsigned char *)buf, (unsigned char *)tempBuf, 256);
 free(tempBuf);
 //tempD = malloc(4);
 //*tempD = decrypted_length;
  return decrypted_length;
}

//
int encrypt(unsigned char * plain_text, 
	    unsigned char * encrypted_text) {
  
   encrypted_length = public_encrypt((unsigned char *)plain_text, 
					strlen((const char *)plain_text),
					(unsigned char *)public_key, 
					encrypted_text);
  if(encrypted_length == -1)
    {
      printLastError("Public Encrypt failed ");
      exit(0);
    }
  printf("Encrypted length =%d\n",encrypted_length);
 


  return encrypted_length;
}

//
int decrypt(unsigned char * decrypted_text, 
	    unsigned char * encrypted_text, 
	    int encrypted_length) {

   decrypted_length = private_decrypt(encrypted_text, encrypted_length,
					 (unsigned char *)private_key, 
					 decrypted_text);
  if(decrypted_length == -1)
    {
      printLastError("Private Decrypt failed ");
      exit(0);
    }
  decrypted_text[decrypted_length] = 0;
  printf("Decrypted Text =%s\n",decrypted_text);
  printf("Decrypted Length =%d\n",decrypted_length);
 
  return decrypted_length;
}
