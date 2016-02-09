#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

ssize_t						
writen(int fd, const void *vptr, size_t n);

void
wrp_writen(int fd, void *ptr, size_t nbytes);
