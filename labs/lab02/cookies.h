#ifndef COOKIE_H
#define COOKIE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

static char * cookies;
static int cookie_count;
static char * cookie_buf;

void init_cookies(char *);
int  count_cookies(void);
char * get_cookie(int);
char * get_i_th_cookie(int);
int  get_file_size(char *);
int  read_file(char *, char *, int);

#endif
