#ifndef _RDWRN_H
#define _RDWRN_H

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

ssize_t	readn(int fd, void *vptr, size_t n);
ssize_t writen(int fd, const void *vptr, size_t n);

#endif
