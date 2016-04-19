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

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <net/if.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <string.h>

/*
 * This function takes in a protocol number (indicated by p), the
 * pointer to a string buffer, and the size of that buffer. It will
 * write to the buffer a string description that matches the protocol
 * number p.
 *
 * Return value: if the function can resolve the protocol number to a
 * description string, it returns the same pointer to the buffer passed
 * in str, otherwise it returns NULL and leaves an error message in the
 * string buffer.
 *
 */

char* get_ip_protocol(unsigned short p, char* str, int count) {

  switch(p) {

  case 0: strncpy(str, "HOPOPT, IPv6 Hop-by-Hop Option.",
			  count); break;

  case 1: strncpy(str, "ICMP, Internet Control Message Protocol.",
			  count); break;

  case 2: strncpy(str, "IGAP, IGMP for user Authentication Protocol.\nIGMP, Internet Group Management Protocol.\nRGMP, Router-port Group Management Protocol.", count); break;

  case 3: strncpy(str, "GGP, Gateway to Gateway Protocol.",
		  count); break;

  case 4: strncpy(str, "IP in IP encapsulation.",
			  count); break;

  case 5: strncpy(str, "ST, Internet Stream Protocol.",
			  count); break;

  case 6: strncpy(str, "TCP, Transmission Control Protocol.",
			  count); break;

  case 7: strncpy(str, "UCL, CBT.",
			  count); break;

  case 8: strncpy(str, "EGP, Exterior Gateway Protocol.",
			  count); break;

  case 9: strncpy(str, "IGRP, Interior Gateway Routing Protocol.",
			  count); break;

  case 10: strncpy(str, "BBN RCC Monitoring.",
		   count); break;

  case 11: strncpy(str, "NVP, Network Voice Protocol.",
		   count); break;

  case 12: strncpy(str, "PUP.",
		   count); break;

  case 13: strncpy(str, "ARGUS.",
		   count); break;

  case 14: strncpy(str, "EMCON, Emission Control Protocol.",
		   count); break;

  case 15: strncpy(str, "XNET, Cross Net Debugger.",
		   count); break;

  case 16: strncpy(str, "Chaos.",
		   count); break;

  case 17: strncpy(str, "UDP, User Datagram Protocol.",
		   count); break;

  case 18: strncpy(str, "TMux, Transport Multiplexing Protocol.",
		   count); break;

  case 19: strncpy(str, "DCN Measurement Subsystems.",
		   count); break;

  case 20: strncpy(str, "HMP, Host Monitoring Protocol.",
		   count); break;

  case 21: strncpy(str, "Packet Radio Measurement.",
		   count); break;

  case 22: strncpy(str, "XEROX NS IDP.",
		   count); break;

  case 23: strncpy(str, "Trunk-1.",
		   count); break;

  case 24: strncpy(str, "Trunk-2.",
		   count); break;

  case 25: strncpy(str, "Leaf-1.",
		   count); break;

  case 26: strncpy(str, "Leaf-2.",
		   count); break;

  case 27: strncpy(str, "RDP, Reliable Data Protocol.",
		   count); break;

  case 28: strncpy(str, "IRTP, Internet Reliable Transaction Protocol.",
		   count); break;

  case 29: strncpy(str, "ISO Transport Protocol Class 4.",
		   count); break;

  case 30: strncpy(str, "NETBLT, Network Block Transfer.",
		   count); break;

  case 31: strncpy(str, "MFE Network Services Protocol.",
		   count); break;

  case 32: strncpy(str, "MERIT Internodal Protocol.",
		   count); break;

  case 33: strncpy(str, "SEP, Sequential Exchange Protocol.\nDCCP, Datagram Congestion Control Protocol.",
		   count); break;

  case 34: strncpy(str, "Third Party Connect Protocol.",
		   count); break;

  case 35: strncpy(str, "IDPR, Inter-Domain Policy Routing Protocol.",
		   count); break;

  case 36: strncpy(str, "XTP, Xpress Transfer Protocol.",
		   count); break;

  case 37: strncpy(str, "Datagram Delivery Protocol.",
		   count); break;

  case 38: strncpy(str, "IDPR, Control Message Transport Protocol.",
		   count); break;

  case 39: strncpy(str, "TP++ Transport Protocol.",
		   count); break;

  case 40: strncpy(str, "IL Transport Protocol.",
		   count); break;

  case 41: strncpy(str, "IPv6 over IPv4.",
		   count); break;

  case 42: strncpy(str, "SDRP, Source Demand Routing Protocol.",
		   count); break;

  case 43: strncpy(str, "IPv6 Routing header.",
		   count); break;

  case 44: strncpy(str, "IPv6 Fragment header.",
		   count); break;

  case 45: strncpy(str, "IDRP, Inter-Domain Routing Protocol.",
		   count); break;

  case 46: strncpy(str, "RSVP, Reservation Protocol.",
		   count); break;

  case 47: strncpy(str, "GRE, General Routing Encapsulation.",
		   count); break;

  case 48: strncpy(str, "MHRP, Mobile Host Routing Protocol.",
		   count); break;

  case 49: strncpy(str, "BNA.",
		   count); break;

  case 50: strncpy(str, "ESP, Encapsulating Security Payload.",
		   count); break;

  case 51: strncpy(str, "AH, Authentication Header.",
		   count); break;

  case 52: strncpy(str, "Integrated Net Layer Security TUBA.",
		   count); break;

  case 53: strncpy(str, "IP with Encryption.",
		   count); break;

  case 54: strncpy(str, "NARP, NBMA Address Resolution Protocol.",
		   count); break;

  case 55: strncpy(str, "Minimal Encapsulation Protocol.",
		   count); break;

  case 56: strncpy(str, "TLSP, Transport Layer Security Protocol using Kryptonet key management.",
		   count); break;

  case 57: strncpy(str, "SKIP.",
		   count); break;

  case 58: strncpy(str, "ICMPv6, Internet Control Message Protocol for IPv6.\nMLD, Multicast Listener Discovery.",
		   count); break;
  
  case 59: strncpy(str, "IPv6 No Next Header.",
		   count); break;
  
  case 60: strncpy(str, "Destination Options for IPv6.",
		   count); break;

  case 61: strncpy(str, "Any host internal protocol.",
		   count); break;

  case 62: strncpy(str, "CFTP.",
		   count); break;

  case 63: strncpy(str, "Any local network.",
		   count); break;

  case 64: strncpy(str, "SATNET and Backroom EXPAK.",
		   count); break;

  case 65: strncpy(str, "Kryptolan.",
		   count); break;

  case 66: strncpy(str, "MIT Remote Virtual Disk Protocol.",
		   count); break;

  case 67: strncpy(str, "Internet Pluribus Packet Core.",
		   count); break;

  case 68: strncpy(str, "Any distributed file system.",
		   count); break;

  case 69: strncpy(str, "SATNET Monitoring.",
		   count); break;

  case 70: strncpy(str, "VISA Protocol.",
		   count); break;

  case 71: strncpy(str, "Internet Packet Core Utility.",
		   count); break;

  case 72: strncpy(str, "Computer Protocol Network Executive.",
		   count); break;

  case 73: strncpy(str, "Computer Protocol Heart Beat.",
		   count); break;

  case 74: strncpy(str, "Wang Span Network.",
		   count); break;

  case 75: strncpy(str, "Packet Video Protocol.",
		   count); break;

  case 76: strncpy(str, "Backroom SATNET Monitoring.",
		   count); break;

  case 77: strncpy(str, "SUN ND PROTOCOL-Temporary.",
		   count); break;

  case 78: strncpy(str, "WIDEBAND Monitoring.",
		   count); break;

  case 79: strncpy(str, "WIDEBAND EXPAK.",
		   count); break;

  case 80: strncpy(str, "ISO-IP.",
		   count); break;

  case 81: strncpy(str, "VMTP, Versatile Message Transaction Protocol.",
		   count); break;

  case 82: strncpy(str, "SECURE-VMTP",
		   count); break;

  case 83: strncpy(str, "VINES.",
		   count); break;

  case 84: strncpy(str, "TTP.",
		   count); break;

  case 85: strncpy(str, "NSFNET-IGP.",
		   count); break;

  case 86: strncpy(str, "Dissimilar Gateway Protocol.",
		   count); break;

  case 87: strncpy(str, "TCF.",
		   count); break;

  case 88: strncpy(str, "EIGRP.",
		   count); break;

  case 89: strncpy(str, "OSPF, Open Shortest Path First Routing Protocol.\nMOSPF, Multicast Open Shortest Path First.",
		   count); break;

  case 90: strncpy(str, "Sprite RPC Protocol.",
		   count); break;

  case 91: strncpy(str, "Locus Address Resolution Protocol.",
		   count); break;

  case 92: strncpy(str, "MTP, Multicast Transport Protocol.",
		   count); break;

  case 93: strncpy(str, "AX.25.",
		   count); break;

  case 94: strncpy(str, "IP-within-IP Encapsulation Protocol.",
		   count); break;

  case 95: strncpy(str, "Mobile Internetworking Control Protocol.",
		   count); break;

  case 96: strncpy(str, "Semaphore Communications Sec. Pro.",
		   count); break;

  case 97: strncpy(str, "EtherIP.",
		   count); break;

  case 98: strncpy(str, "Encapsulation Header.",
		   count); break;

  case 99: strncpy(str, "Any private encryption scheme.",
		   count); break;

  case 100: strncpy(str, "GMTP.",
		    count); break;

  case 101: strncpy(str, "IFMP, Ipsilon Flow Management Protocol.",
		    count); break;

  case 102: strncpy(str, "PNNI over IP.",
		    count); break;

  case 103: strncpy(str, "PIM, Protocol Independent Multicast.",
		    count); break;

  case 104: strncpy(str, "ARIS.",
		    count); break;

  case 105: strncpy(str, "SCPS.",
		    count); break;

  case 106: strncpy(str, "QNX.",
		    count); break;

  case 107: strncpy(str, "Active Networks.",
		    count); break;

  case 108: strncpy(str, "IPPCP, IP Payload Compression Protocol.",
		    count); break;

  case 109: strncpy(str, "SNP, Sitara Networks Protocol.",
		    count); break;

  case 110: strncpy(str, "Compaq Peer Protocol.",
		    count); break;

  case 111: strncpy(str, "IPX in IP.",
		    count); break;

  case 112: strncpy(str, "VRRP, Virtual Router Redundancy Protocol.",
		    count); break;

  case 113: strncpy(str, "PGM, Pragmatic General Multicast.",
		    count); break;

  case 114: strncpy(str, "any 0-hop protocol.",
		    count); break;

  case 115: strncpy(str, "L2TP, Level 2 Tunneling Protocol.",
		    count); break;

  case 116: strncpy(str, "DDX, D-II Data Exchange.",
		    count); break;

  case 117: strncpy(str, "IATP, Interactive Agent Transfer Protocol.",
		    count); break;

  case 118: strncpy(str, "ST, Schedule Transfer.",
		    count); break;

  case 119: strncpy(str, "SRP, SpectraLink Radio Protocol.",
		    count); break;

  case 120: strncpy(str, "UTI.",
		    count); break;

  case 121: strncpy(str, "SMP, Simple Message Protocol.",
		    count); break;

  case 122: strncpy(str, "SM.",
		    count); break;

  case 123: strncpy(str, "TP, Performance Transparency Protocol.",
		    count); break;

  case 124: strncpy(str, "SIS over IPv4.",
		    count); break;

  case 125: strncpy(str, "IRE.",
		    count); break;

  case 126: strncpy(str, "RTP, Combat Radio Transport Protocol.",
		    count); break;

  case 127: strncpy(str, "RUDP, Combat Radio User Datagram.",
		    count); break;

  case 128: strncpy(str, "SCOPMCE.",
		    count); break;

  case 129: strncpy(str, "PLT.",
		    count); break;

  case 130: strncpy(str, "PS, Secure Packet Shield.",
		    count); break;

  case 131: strncpy(str, "IPE, Private IP Encapsulation within IP.",
		    count); break;

  case 132: strncpy(str, "CTP, Stream Control Transmission Protocol.",
		    count); break;

  case 133: strncpy(str, "ibre Channel.",
		    count); break;

  case 134: strncpy(str, "SVP-E2E-IGNORE.",
		    count); break;

  case 135: strncpy(str, "obility Header.",
		    count); break;

  case 136: strncpy(str, "DP-Lite, Lightweight User Datagram Protocol.",
		    count); break;

  case 137: strncpy(str, "PLS-in-IP.",
		    count); break;

  case 253: strncpy(str, "Experimentation and testing.",
		    count); break;

  case 254: strncpy(str, "Experimentation and testing.",
		    count); break;
  
  case 255: strncpy(str, "Reserved.",
		    count); break;
  
  default:  {
    strncpy(str, "No protocol declared.",
	    count);
    str = NULL;
  }
  }

  return(str);
}

/*
 * Return value: if the function can resolve the Ethernet frame type to a
 * description string, it returns the same pointer to the buffer passed
 * in str, otherwise it returns NULL and leaves an error message in the
 * string buffer.
 *
 */

char* get_eth_protocol(unsigned short t, char* str, int count) {

  switch(t) {

  case ETHERTYPE_ARP: strncpy(str,"ARP, Address Resolution Protocol",
			      count); break;

  case ETHERTYPE_IP:  strncpy(str,"IPv4, Internet Protocol version 4",
			      count); break;
  
  case ETHERTYPE_REVARP: strncpy(str,"Reverse ARP",
			       count); break;

  case ETHERTYPE_PUP: strncpy(str,"PUP",
			       count); break;

  default: {
    strncpy(str,"Unknown Ethernet Protocol in Linux", count); 
    str = NULL;
  }
  }

  return(str);
}

/*
 * These high-order 3 octets (6 hex digits) are also known as the
 * Organizationally Unique Identifier or OUI.
 *
 * Ethernet addresses might be written unhyphenated (e.g., 123456789ABC),
 * or with one hyphen (e.g., 123456-789ABC), but should be written
 * hyphenated by octets (e.g., 12-34-56-78-9A-BC).
 *
 * These addresses are physical station addresses, not multicast nor
 * broadcast, so the second hex digit (reading from the left) will be
 * even, not odd.
 *
 * Return value: if the function can resolve the NIC vendor code to a
 * description string, it returns the same pointer to the buffer passed
 * in str, otherwise it returns NULL and leaves an error message in the
 * string buffer.
 *
 */

char* get_nic_vendor(unsigned char* p, char* str, int count) {

  unsigned int oui = p[2];
  oui = oui + (p[1]<<8);
  oui = oui + (p[0]<<16); 

  switch (oui) {
  case 0x00000C: strncpy(str, "Cisco", count); break ;
  case 0x00000E: strncpy(str, "Fujitsu", count); break ;
  case 0x00000F: strncpy(str, "NeXT", count); break ;
  case 0x000010: strncpy(str, "Sytek", count); break ;
  case 0x00001D: strncpy(str, "Cabletron", count); break ;
  case 0x000020: strncpy(str, "DIAB (Data Intdustrier AB)", count); break ;
  case 0x000022: strncpy(str, "Visual Technology", count); break ;
  case 0x00002A: strncpy(str, "TRW", count); break ;
  case 0x000032: strncpy(str, "GPT Limited (reassigned from GEC Computers Ltd)", count); break ;
  case 0x00005A: strncpy(str, "S & Koch", count); break ;
  case 0x00005E: strncpy(str, "IANA", count); break ;
  case 0x000065: strncpy(str, "Network General", count); break ;
  case 0x00006B: strncpy(str, "MIPS", count); break ;
  case 0x000077: strncpy(str, "Interphase Corporation", count); break ;
  case 0x00007A: strncpy(str, "Ardent", count); break ;
  case 0x000080: strncpy(str, "Cray Communications A/S", count); break ;
  case 0x000089: strncpy(str, "Cayman Systems Gatorbox", count); break ;
  case 0x000093: strncpy(str, "Proteon", count); break ;
  case 0x00009F: strncpy(str, "Ameristar Technology", count); break ;
  case 0x0000A2: strncpy(str, "Wellfleet", count); break ;
  case 0x0000A3: strncpy(str, "Network Application Technology", count); break ;
  case 0x0000A6: strncpy(str, "Network General (internal assignment, not for products)", count); break ;
  case 0x0000A7: strncpy(str, "NCD X-terminals", count); break ;
  case 0x0000A9: strncpy(str, "Network Systems", count); break ;
  case 0x0000AA: strncpy(str, "Xerox Xerox machines", count); break ;
  case 0x0000B3: strncpy(str, "CIMLinc", count); break ;
  case 0x0000B7: strncpy(str, "Dove Fastnet", count); break ;
  case 0x0000BC: strncpy(str, "Allen-Bradley", count); break ;
  case 0x0000C0: strncpy(str, "Western Digital", count); break ;
  case 0x0000C5: strncpy(str, "Farallon phone net card", count); break ;
  case 0x0000C6: strncpy(str, "HP Intelligent Networks Operation (formerly Eon Systems)", count); break ;
  case 0x0000C8: strncpy(str, "Altos", count); break ;
  case 0x0000C9: strncpy(str, "Emulex Terminal Servers", count); break ;
  case 0x0000D0: strncpy(str, "Develcon", count); break ;
  case 0x0000D7: strncpy(str, "Dartmouth College (NED Router)", count); break ;
  case 0x0000D8: strncpy(str, "3Com? Novell?   PS/2", count); break ;
  case 0x0000DD: strncpy(str, "Gould", count); break ;
  case 0x0000DE: strncpy(str, "Unigraph", count); break ;
  case 0x0000E2: strncpy(str, "Acer Counterpoint", count); break ;
  case 0x0000EF: strncpy(str, "Alantec", count); break ;
  case 0x0000FD: strncpy(str, "High Level Hardvare (Orion, UK)", count); break ;
  case 0x000102: strncpy(str, "BBN BBN internal usage (not registered)", count); break ;
  case 0x000103: strncpy(str, "3com Corporation", count); break;
  case 0x000C41: strncpy(str, "Linksys Group", count); break ;
  case 0x0010D1: strncpy(str, "BlazeNet", count); break ;
  case 0x001700: strncpy(str, "Kabel", count); break ;
  case 0x0020AF: strncpy(str, "3COM ???", count); break ;
  case 0x0020C9: strncpy(str, "Victron", count); break ;
  case 0x002094: strncpy(str, "Cubix", count); break ;
  case 0x008064: strncpy(str, "Wyse Technology / Link Technologies", count); break ;
  case 0x00802B: strncpy(str, "IMAC ???", count); break ;
  case 0x00802D: strncpy(str, "Xylogics, Inc.  Annex terminal servers", count); break ;
  case 0x00808C: strncpy(str, "Frontier Software Development", count); break ;
  case 0x0080C2: strncpy(str, "IEEE 802.1 Committee", count); break ;
  case 0x0080D3: strncpy(str, "Shiva", count); break ;
  case 0x00A03E: strncpy(str, "ATM Forum", count); break ;
  case 0x00AA00: strncpy(str, "Intel", count); break ;
  case 0x00DD00: strncpy(str, "Ungermann-Bass", count); break ;
  case 0x00DD01: strncpy(str, "Ungermann-Bass", count); break ;
  case 0x020701: strncpy(str, "Racal InterLan", count); break ;
  case 0x020406: strncpy(str, "BBN BBN internal usage (not registered)", count); break ;
  case 0x026086: strncpy(str, "Satelcom MegaPac (UK)", count); break ;
  case 0x02608C: strncpy(str, "3Com IBM PC; break ; Imagen; break ; Valid; break ; Cisco", count); break ;
  case 0x02CF1F: strncpy(str, "CMC Masscomp; break ; Silicon Graphics; break ; Prime EXL", count); break ;
  case 0x080002: strncpy(str, "3Com (Formerly Bridge)", count); break ;
  case 0x080003: strncpy(str, "ACC (Advanced Computer Communications)", count); break ;
  case 0x080005: strncpy(str, "Symbolics Symbolics LISP machines", count); break ;
  case 0x080008: strncpy(str, "BBN", count); break ;
  case 0x080009: strncpy(str, "Hewlett-Packard", count); break ;
  case 0x08000A: strncpy(str, "Nestar Systems", count); break ;
  case 0x08000B: strncpy(str, "Unisys", count); break ;
  case 0x080011: strncpy(str, "Tektronix, Inc.", count); break ;
  case 0x080014: strncpy(str, "Excelan BBN Butterfly, Masscomp, Silicon Graphics", count); break ;
  case 0x080017: strncpy(str, "NSC", count); break ;
  case 0x08001A: strncpy(str, "Data General", count); break ;
  case 0x08001B: strncpy(str, "Data General", count); break ;
  case 0x08001E: strncpy(str, "Apollo", count); break ;
  case 0x080020: strncpy(str, "Sun Sun machines", count); break ;
  case 0x080022: strncpy(str, "NBI", count); break ;
  case 0x080025: strncpy(str, "CDC", count); break ;
  case 0x080026: strncpy(str, "Norsk Data (Nord)", count); break ;
  case 0x080027: strncpy(str, "PCS Computer Systems GmbH", count); break ;
  case 0x080028: strncpy(str, "TI Explorer", count); break ;
  case 0x08002B: strncpy(str, "DEC", count); break ;
  case 0x08002E: strncpy(str, "Metaphor", count); break ;
  case 0x08002F: strncpy(str, "Prime Computer  Prime 50-Series LHC300", count); break ;
  case 0x080036: strncpy(str, "Intergraph CAE stations", count); break ;
  case 0x080037: strncpy(str, "Fuji-Xerox", count); break ;
  case 0x080038: strncpy(str, "Bull", count); break ;
  case 0x080039: strncpy(str, "Spider Systems", count); break ;
  case 0x080041: strncpy(str, "DCA Digital Comm. Assoc.", count); break ;
  case 0x080045: strncpy(str, "???? (maybe Xylogics, but they claim not to know this number)", count); break ;
  case 0x080046: strncpy(str, "Sony", count); break ;
  case 0x080047: strncpy(str, "Sequent", count); break ;
  case 0x080049: strncpy(str, "Univation", count); break ;
  case 0x08004C: strncpy(str, "Encore", count); break ;
  case 0x08004E: strncpy(str, "BICC", count); break ;
  case 0x080056: strncpy(str, "Stanford University", count); break ;
  case 0x080058: strncpy(str, "??? DECsystem-20", count); break ;
  case 0x08005A: strncpy(str, "IBM", count); break ;
  case 0x080067: strncpy(str, "Comdesign", count); break ;
  case 0x080068: strncpy(str, "Ridge", count); break ;
  case 0x080069: strncpy(str, "Silicon Graphics", count); break ;
  case 0x08006E: strncpy(str, "Concurrent Masscomp", count); break ;
  case 0x080075: strncpy(str, "DDE (Danish Data Elektronik A/S)", count); break ;
  case 0x08007C: strncpy(str, "Vitalink TransLAN III", count); break ;
  case 0x080080: strncpy(str, "XIOS", count); break ;
  case 0x080086: strncpy(str, "Imagen/QMS", count); break ;
  case 0x080087: strncpy(str, "Xyplex terminal servers", count); break ;
  case 0x080089: strncpy(str, "Kinetics AppleTalk-Ethernet interface", count); break ;
  case 0x08008B: strncpy(str, "Pyramid", count); break ;
  case 0x08008D: strncpy(str, "XyVision XyVision machines", count); break ;
  case 0x080090: strncpy(str, "Retix Inc Bridges", count); break ;
  case 0x484453: strncpy(str, "HDS ???", count); break ;
  case 0x800010: strncpy(str, "AT&T", count); break ;
  case 0xAA0000: strncpy(str, "DEC obsolete", count); break ;
  case 0xAA0001: strncpy(str, "DEC obsolete", count); break ;
  case 0xAA0002: strncpy(str, "DEC obsolete", count); break ;
  case 0xAA0003: strncpy(str, "DEC Global physical address for some DEC machines", count); break ;
  case 0xAA0004: strncpy(str, "DEC Local logical address for systems running DECNET", count); break ;
  case 0xFFFFFF: strncpy(str, "BROADCAST ADDRESS", count); break ;

  default: {
    strncpy(str, "unknown", count);
    str = NULL;
  }
  }

  return str;
}

