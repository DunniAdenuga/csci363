#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include<netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<netinet/ip_icmp.h>
#include <string.h>
#include <ifaddrs.h>
 #include <stdlib.h>
#include <sys/time.h>

#include "icmp_exp.h"

#define MAX 4096
#define HOPLIMIT 40

unsigned short
csum (unsigned short *buf, int nwords);
char* 
get_host_ip(void);

int
main (int argc, char *argv[])
{
    struct timeval timeout;
    struct timeval tv1a, tv2a, avg_tv1;
    struct timeval tv1b, tv2b, avg_tv2;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
  if (argc != 2)
    {
      printf ("need destination for tracert\n");
      exit (0);
    }
  
  //Create raw socket that uses ICMP protocol
  int sfd = socket (AF_INET, SOCK_RAW, IPPROTO_ICMP);
  char buf[MAX] = { 0 };
  struct ip *ip_hdr = (struct ip *) buf;
  int hop = 0;

  int one = 1;
  const int *val = &one;
  //Allows application to provide IP_HEADER
  if (setsockopt (sfd, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
    printf ("Cannot set HDRINCL!\n");

  //set timer option sor socket
  if (setsockopt (sfd, IPPROTO_IP, SO_RCVTIMEO, (char *)&timeout,
		  sizeof(timeout)) < 0)
    printf ("Cannot set timer!\n");
  struct sockaddr_in addr;
  addr.sin_port = htons (7);
  addr.sin_family = AF_INET;//address family set to internet

  //convert to network address and store
  inet_pton (AF_INET, argv[1], &(addr.sin_addr));

  
  while (hop < HOPLIMIT)
    {
      //Create IP-header
      ip_hdr->ip_hl = 5;
      ip_hdr->ip_v = 4;
      ip_hdr->ip_tos = 0;
      ip_hdr->ip_len = 20 + 8;
      ip_hdr->ip_id = 10000;
      ip_hdr->ip_off = 0;
      ip_hdr->ip_ttl = hop;
      ip_hdr->ip_p = IPPROTO_ICMP;
      inet_pton (AF_INET, get_host_ip(), &(ip_hdr->ip_src));
      inet_pton (AF_INET, argv[1], &(ip_hdr->ip_dst));
      ip_hdr->ip_sum = csum ((unsigned short *) buf, 9);
      
      //create icmp-header
      struct icmphdr *icmphd = (struct icmphdr *) (buf + 20);
      icmphd->type = ICMP_ECHO;
      icmphd->code = 0;
      icmphd->checksum = 0;
      icmphd->un.echo.id = 0;
      icmphd->un.echo.sequence = hop + 1;
      icmphd->checksum = csum ((unsigned short *) (buf + 20), 4);
      gettimeofday(&tv1a, NULL);
      sendto (sfd, buf, sizeof(struct ip) + sizeof(struct icmphdr), 0, SA & addr, sizeof addr);
       gettimeofday(&tv1b, NULL);
       avg_tv1.tv_sec = (tv1a.tv_sec + tv1b.tv_sec)/2;
       avg_tv1.tv_usec = (tv1a.tv_usec + tv1b.tv_usec)/2;
      char buff[MAX] = { 0 };
      struct sockaddr_in addr2;
      socklen_t len = sizeof (struct sockaddr_in);
      
      if(recvfrom (sfd, buff, sizeof(buff), 0, SA & addr2, &len) < 0){
	printf("%d * * *\n", hop);
      }
      
      else{
	gettimeofday(&tv2a, NULL);
	gettimeofday(&tv2b, NULL);
	avg_tv2.tv_sec = (tv2a.tv_sec + tv2b.tv_sec)/2;
       avg_tv2.tv_usec = (tv2a.tv_usec + tv2b.tv_usec)/2;
	
      struct icmphdr *icmphd2 = (struct icmphdr *) (buff + 20);
      if (icmphd2->type != 0){
	long long elapsed_time1 = (tv2a.tv_sec - tv1a.tv_sec)/1000000LL + (tv2a.tv_usec - tv1a.tv_usec);
	long long elapsed_time2 = (tv2b.tv_sec - tv1b.tv_sec)/1000000LL + (tv2b.tv_usec - tv1b.tv_usec);
	long long elapsed_time3 = (avg_tv2.tv_sec - avg_tv1.tv_sec)/1000000LL + (avg_tv2.tv_usec - avg_tv1.tv_usec);
	printf ("hop limit:%d Address:%s %lld ms  %lld ms  %lld ms \n", hop,  inet_ntoa (addr2.sin_addr), elapsed_time1, elapsed_time2, elapsed_time3 );
      }      
else
	{
	  printf ("Reached destination:%s with hop limit:%d\n",
		  inet_ntoa (addr2.sin_addr), hop);
	  exit (0);
	}
      }
      hop++;
    }

  return 0;
}

/*Calculates checksum*/
unsigned short
csum (unsigned short *buf, int nwords)
{
  unsigned long sum;
  for (sum = 0; nwords > 0; nwords--)
    sum += *buf++;
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return ~sum;
}

char* 
get_host_ip(void){
char* result = malloc (MAX);
struct ifaddrs *addrs, *tmp;
getifaddrs(&addrs);
tmp = addrs;
while (tmp) 
{
  if ((tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET)&& (strcmp(tmp->ifa_name, "eth0")== 0))
    {
        struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
	strcpy(result, inet_ntoa(pAddr->sin_addr));
    }
tmp = tmp->ifa_next;
}
     
  freeifaddrs(addrs);
  return result;
		   }

