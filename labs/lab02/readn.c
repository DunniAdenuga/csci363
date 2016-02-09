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

/* Note that to compile this separately, you need to 
 * include <sys/types.h> and provide a header file 
 * exporting the functions as extern.
 */

/* Read "n" bytes from a descriptor. */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "readn.h"

ssize_t				
readn(int fd, void *vptr, size_t n)
{
	size_t	nleft;
	ssize_t	nread;
	char	*ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nread = read(fd, ptr, nleft)) < 0) {
			if (errno == EINTR)
				nread = 0; /* and call read() again */
			else
				return(-1);
		} else if (nread == 0)
			break; /* EOF */

		nleft -= nread;
		ptr   += nread;
	}
	return(n - nleft); /* return >= 0 */
}
/* end readn */

/* Wrapper function for readn */

ssize_t
wrp_readn(int fd, void *ptr, size_t nbytes)
{
     ssize_t		n;
     
     if ( (n = readn(fd, ptr, nbytes)) < 0) {
	  printf("readn error\n");
	  exit(-1);
     }
     return(n);
}
