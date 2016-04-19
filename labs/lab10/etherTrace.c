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
 *
 *
 * Modified by: X. Meng (xmeng at bucknell.edu)
 * Spring 2013 for CSCI 363
 * Skeleton file for lab exercises
 */

#include <arpa/inet.h>
#include "etherTrace.h"
#include "prot_strings.h"
#include "hexdump.h"
#include "netinet/ip.h"
#include "netinet/tcp.h"
#include "netinet/udp.h"
#include "stdio.h"
//************************************************************************

int main(int argc, char *argv[]){
     
  unsigned char *data_buff = (unsigned char*)malloc(MAXFRMLEN);
  unsigned char *data_addr;

  int input_file;   // data input file id
  int done = 0;     // loop control
  int count = 0;    // packet count
  int size_read;    // amount read in bytes

  struct pcap_hdr_s  file_header;
  struct pcaprec_hdr_s rec_header_buffer;
  struct ether_header *ethptr;  /* /usr/include/net/ethernet.h */
 
  if (argc != 2) {
    printf("Usage: etherTrace [trace file name]\n");
    exit(-1);
  }
  
  input_file = open(argv[1], O_RDONLY);

  if (-1 == input_file) {
    perror("etherTrace: ");
    exit(-1);
  }

  // read pcap global header
  read_file_header(input_file, &file_header);
    

  // parse and print pcap global header
  parse_global_header(&file_header);
    
  // read header of FIRST frame record
  size_read = read (input_file, &rec_header_buffer, 
		    sizeof(struct pcaprec_hdr_s));
  if (size_read <= 0) {
    perror("etherTrace: no records ");
    exit(-1);
  }

  while (!done) {

    printf(" === packet count %d === \n", count);
    count ++;

    // parse and print the record buffer
    parse_rec_header(&rec_header_buffer);

    // read the captured ethernet frame
    size_read = read (input_file, data_buff, rec_header_buffer.caplen);

    // align the buffer with ethernet frame header
    ethptr = (struct ether_header *)data_buff;

    parse_eth_header(ethptr);

    if (is_ip_packet(ethptr)) {  // if this is an Ethernet frame
	                         // align the buffer with an IP header
      data_addr = (unsigned char *)(data_buff + sizeof(struct ether_header));
      struct iphdr * ip_pkt = (struct iphdr*)(data_addr);
      parse_ip_header(ip_pkt);
      uint8 transport_proto = ip_pkt->protocol;
      if (transport_proto == TCP_PROTO) { // see prot_strings.h
	// student work:
	// 1. align tcp header with data buffer
	data_addr = (unsigned char *)(data_buff + sizeof(struct ether_header) + sizeof(struct iphdr));
	struct tcphdr * tcp_pkt = (struct tcphdr*)(data_addr);
	// 2. call parse_tcp_header to process the packet information
	parse_tcp_header(tcp_pkt);
      } else if (transport_proto == UDP_PROTO) { // see prot_strings.h
	// student work:
	// 1. align udp header with data buffer
	data_addr = (unsigned char *)(data_buff + sizeof(struct ether_header) + sizeof(struct iphdr));
	struct udphdr * udp_pkt = (struct udphdr*)(data_addr);
	// 2. call parse_udp_header to process the packet information
	parse_udp_header(udp_pkt);
      }
    }

    printf("\n");
 
    // read next frame record
    size_read = read (input_file, &rec_header_buffer, 
		      sizeof(struct pcaprec_hdr_s));
    if (size_read <= 0) {
      done = 1; // reach the end of input file
    }
  }
  
  close(input_file);
  
  return(0);
}

//************************************************************************

/*
 * Read trace file header
 */
void read_file_header(int fd, struct pcap_hdr_s *fhd) {

  // read the file header
  int retval = read(fd, fhd, sizeof(struct pcap_hdr_s));
  if (retval <= 0) {
    perror("File header error:");
    exit(-1);
  }
}

/*
 * Print trace file header
 */
void parse_global_header(struct pcap_hdr_s *fhd) {

  printf("======GLOBAL FILE  HEADER========================\n");
  printf("magic number %x major verion %d minor version %d\n", 
	 fhd->magic_number, fhd->version_major, fhd->version_minor);
  printf("maximum captured packet length %d\n", fhd->snaplen);
  printf("link layer type %d\n", fhd->network);
  printf("=================================================\n");
}

//************************************************************************

char* parse_rec_header(struct pcaprec_hdr_s *ph) {
  
  long time_val = ph->tv_sec;
  printf("time captured  %s\n",  ctime((const time_t *)&(time_val)));
   
    // read the frame content into 'buf'
  printf("Number of bytes captured: %d\n", ph->caplen);
  printf("Length of the original packet: %d\n", ph->len);
  return NULL;
}

//************************************************************************

char* parse_eth_header(struct ether_header *eh) {

  char mystr[VENDORSTRLEN];

  printf("======ETHERNET HEADER============================\n");
  
  printf(" Destination address: %s\n", 
	 ether_addr_to_string((char*)(eh->ether_dhost)));
  printf(" Source address: %s\n", 
	 ether_addr_to_string((char*)(eh->ether_shost)));
  printf("    Vendor: %s\n", 
	 get_nic_vendor((char*)(eh->ether_dhost), mystr, VENDORSTRLEN));
  
	return NULL;
}

//************************************************************************

int is_ip_packet(struct ether_header *eh) {
  return (ntohs(eh->ether_type) == ETHERTYPE_IP);
}

//************************************************************************

int is_arp_packet(struct ether_header *eh) {
  return (ntohs(eh->ether_type) == ETHERTYPE_ARP);
}

//************************************************************************

char *parse_arp_header(struct arphdr *ahdr) { // see /usr/include/net/if_arp.h

  return NULL;
}

//************************************************************************

char *parse_icmp_header(struct icmphdr *ihdr) { // see /usr/include/netinet/ip_icmp.h

  return NULL;
}

//************************************************************************

char *parse_udp_header(struct udphdr *uhdr) { // see /usr/include/netinet/udp.h
 printf("============== UDP Packet ================\n");
  // student work:
  // 1. print source port number in decimal
 printf("udp source port: %d\n", ntohs(uhdr->source));
  // 2. print destination port number in decimal
 printf("udp dest port: %d\n", ntohs(uhdr->dest));
  // 3. print pakcet length in decimal
 printf("udp packet length: %d\n", ntohs(uhdr->len));
  // 3. print check-sum in hexadecimal
 printf("udp checksum: 0x%x\n", ntohs(uhdr->check));
  return NULL;
}

//************************************************************************

char *parse_tcp_header(struct tcphdr *thdr) { // see /usr/include/netinet/tcp.h
   printf("============== TCP Packet ================\n");
  // student work:
  // 1. print source port number in decimal
   printf("tcp source port: %d", ntohs(thdr->source));
  // 2. print desitnation port number in decimal
   printf("tcp dest port: %d", ntohs(thdr->dest));
  // 3. print sequence number in hexadecimal
   printf("tcp sequence number: %d", ntohs(thdr->seq));
  // 4. print acknowledgement number in hexadecimal
   printf("tcp acknowledgement number: 0x%x", ntohs(thdr->ack_seq));
  // 5. print the "ack" bit value
   printf("tcp ack bit: %d", ntohs(thdr->ack));
  return NULL;
}

//************************************************************************

char *parse_unknown(char* buf, int length) {

  // when this is the case, perhaps it is a good idea to do a hexdump of
  // the whole packet (something in the style of what xxd does)

  return NULL;
}

//************************************************************************

char* parse_ip_header(struct iphdr* iph) {

  // 1. print a header indicating this is an IP header
  printf("============== IP Header ================\n");
  // 2. print the Type of Service field (tos) in hexadecimal
  printf("Type Of Service, 0x%x\n", ntohs(iph->tos));
  // 3a. print the IP packet header length in decimal
  printf("header length, %d words\n", iph->ihl);
  // 3b. print the total IP packet length in decimal
  printf("packet length, %d bytes\n", ntohs(iph->tot_len));
  // 4. print the identification bits in hexadecimal
  printf("packet ID, 0x%x \n", ntohs(iph->id));
  // 5. print the next 3 flag bits
  //    a) reserved bit, skip
  //    b) fragmentation bit (whether or not to fragment)
  printf("FB, 0x%x \n", iph->frag_off & 0x4000);
  //    c) more fragment bit (is there more to come)
  printf("MFB, 0x%x \n", iph->frag_off & 0x2000);
  //    To print individual bits, you need to extract these bits
  //    using a bit mask and bitwise AND; find the mask value in /usr/include/netinet/ip.h

  // 6. print the next 5 bits as fragment offset in decimal
  printf("Fragment Offset, %x \n", iph->frag_off & 0x1E00);
  // 7. print the value of TTL (time-to-live) in decimal
  printf("TTL, %d \n", iph->ttl);
  // 8. print the upper level protocol value in decimal and name in string
  char* str = (char*)malloc(999);
  printf("upper level Protocol in decimal: %d, in string: %s \n", iph->protocol, get_ip_protocol(iph->protocol, str, sizeof(str)));
  // 9. print the header checksum in hexadecimal
  printf("packet checksum, 0x%x \n", ntohs(iph->check));
  // 10. print the source and destination IP addresses
      struct in_addr ip_addr;
      ip_addr.s_addr = iph->saddr;
      printf("IP source address - %s\n" , inet_ntoa(ip_addr));
      ip_addr.s_addr = iph->daddr;
      printf("IP destination address - %s\n" , inet_ntoa(ip_addr));
  return NULL;
}

/*
 * Convert ether address to a readable format. Ethernet address
 * is six bytes long.
 */
char * ether_addr_to_string(char *host_addr) {
  // we should use 'ether_ntoa(3)', but somehow it doesn't work

  char * retval = (char*)malloc(BUFSIZE+1);
  sprintf(retval, "%02x:%02x:%02x:%02x:%02x:%02x",
	  (u_char)(host_addr[0]),
	  (u_char)host_addr[1],
	  (u_char)host_addr[2],
	  (u_char)host_addr[3],
	  (u_char)host_addr[4],
	  (u_char)host_addr[5]);

  if (strlen(retval) >= BUFSIZE)
    retval[BUFSIZE] = 0; // terminate the string
  return retval;
}

