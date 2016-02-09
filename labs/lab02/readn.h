#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

ssize_t				
readn(int fd, void *vptr, size_t n);
ssize_t
wrp_readn(int fd, void *ptr, size_t nbytes);
