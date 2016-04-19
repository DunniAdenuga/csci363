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

#ifndef __PROT_STRINGS_H__
#define __PROT_STRINGS_H__

#define VENDORSTRLEN 300
#define IPSTRLEN 300

extern char* get_nic_vendor(char* p, char* str, int count);
extern char* get_ip_protocol(unsigned short p, char* str, int count);
extern char* get_eth_protocol(unsigned short t, char* str, int count);

/* 
 * The following defines a map between a protocol's symbolic name 
 * and its numeric value as defined by "ipheader->protocol"
 * See prot_strings.c for a complete list of numeric values
 *
 * See also the website
 * <http://www.iana.org/assignments/protocol-numbers/protocol-numbers.xml>
 * for an "official" copy of the complete list.
 * 
 * x.m.
 */
#define ICMP_PROT 1
#define TCP_PROTO 6
#define UDP_PROTO 17
#define IP_ENCRPT 53

#endif /* __PROT_STRINGS_H__ */


