/*
 * Implementation file of "shadow." See shadow.h for explanation.
 *
 * Xiannong Meng
 * For CSCI 363
 * 2013-04-07
 */
#include "shadow.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

/* build an spwd structure from a shadow file line */
struct spwd * build_sp_struct(const char * sp_line, const char * uname);

int SPWD_LEN = 256;

struct spwd * getspnam(const char * uname) {

     int ulen = strlen(uname);

     if (ulen <= 0) {
         return 0;
     }
     FILE *f = fopen(SHADOW_FILE, "r");
     if (f == NULL) {
        return 0;
     }


     size_t len = 0;
     int ret_val = 0;
     char *line = NULL;
     do {
         ret_val = getline(&line, &len, f);
         if (ret_val > 0) {
            line[len] = 0;  /* terminate the string */
         }
     }  while (ret_val > 0 && strncmp(uname, line, ulen) != 0);
     if (ret_val <= 0)  { /* reaches the end without finding it */
        return 0;
     } else {
         struct spwd * record = build_sp_struct(line, uname);
	 /*
         if (line != NULL)
            free(line);
	 */
         return record;
     } 
}

struct spwd * build_sp_struct(const char* sp_line, const char * uname) {

     char * pass_start = strstr(sp_line, ":");
     char * pass_end = strstr(pass_start+1, ":");

     struct spwd * record = (struct spwd *)malloc(sizeof(struct spwd));

     int len = strlen(uname);
     record->sp_namp = (char *)malloc(len + 1);
     strncpy(record->sp_namp, uname, len);
     len = (int)(pass_end - pass_start - 1);

     if (len < 0)
        return 0;

      record->sp_pwdp = (char *)malloc(len + 1);
      strncpy(record->sp_pwdp, pass_start+1, len);
      return record; 
}
