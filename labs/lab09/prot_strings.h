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

#endif /* __PROT_STRINGS_H__ */


