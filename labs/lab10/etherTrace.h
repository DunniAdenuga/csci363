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

#ifndef __ETHER_TRACE_H__
#define __ETHER_TRACE_H__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if_arp.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define int32 unsigned int
#define int16 unsigned short
#define uint32 unsigned int
#define uint16 unsigned short
#define uint8 unsigned char

#define MAXFRMLEN 65535   // ethernet frame size (actually 1500 suffices)
#define BUFSIZE   256    // general buffer size

/* The following structure is the same as defined in pcap.h */
struct pcap_hdr_s { // Global header for libpcap file
  uint32 magic_number;   /* magic number */
  uint16 version_major;  /* major version number */
  uint16 version_minor;  /* minor version number */
  int32  thiszone;       /* GMT to local correction */
  uint32 sigfigs;        /* accuracy of timestamps */
  uint32 snaplen;        /* max length of captured packets, in octets */
  uint32 network;        /* data link type */
};

/* 
 * The following structure is for the ethereal capture file,
 * except that tv_sec and tv_usec here are both 32 integers, where
 * time_t is a 64 bit int.
 */
struct pcaprec_hdr_s { // Record (packet) header
  uint32         tv_sec;
  uint32         tv_usec; /* time when packet was captured */
  uint32         caplen;	/* length of portion captured */
  uint32         len;	        /* length of original packet (off wire) */
};

char *ether_addr_to_string(char *host_addr);
void  process_one_ether_pkt(u_char * pkt_buf);
void  read_file_header(int fd, struct pcap_hdr_s *fhd);
void  parse_global_header(struct pcap_hdr_s *fhd);

char *parse_rec_header(struct pcaprec_hdr_s *ph);
char *parse_eth_header(struct ether_header *eh);
char *parse_ip_header(struct iphdr *ih);
char *parse_arp_header(struct arphdr *ah);
char *parse_icmp_header(struct icmphdr *ih);
char *parse_udp_header(struct udphdr *uh);
char *parse_tcp_header(struct tcphdr *th);
 
int is_ip_packet(struct ether_header *eh);
int is_arp_packet(struct ether_header *eh);
 
#endif /* __ETHER_TRACE_H__ */
