/*
 * The implementation file of <pwd.h>. The files and functions
 * here are emulations of the Linux implementation.
 *
 * Xiannong Meng
 * For CSCI 363
 * 2013-04-07
 */

#include "shadow.h"
#include "pwd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int PW_LEN = 256;    /* maximum length of each line in passwd file */

/* this is a local, private function */
struct passwd * build_pw_struct(const char * pw_line, const char * uname);

/* 
 * Emulate the behavior of getpwnam()
 * Return a passwd structure for the given user name.
 */
struct passwd * getpwnam(const char *uname) {

  int ulen = strlen(uname);
  if (ulen <= 0)
    return 0;

  FILE *f = fopen(PASS_FILE, "r");  /* PASS_FILE defined in pwd.h */
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
    struct passwd * record = build_pw_struct(line, uname);
    /*
    if (line != NULL)
      free(line);
    */
    return record;
  }
}

/*
 * Extract entries from the password line
 * In general there are seven entries.
 */
struct passwd * build_pw_struct(const char * pw_line, const char * uname) {

  /* there are seven entries for each passwd line */
  /* login-name:passwd:uid:gid:real-name:home-dir:shell */

  /* parse and store information in an array, then extract individuals */
  char *fields[7];
  int i = 0;
  int len = 0;

  char * start = strstr(pw_line, ":");
  char * end = NULL;
  for  (i = 1; i < 6; i ++) {
    end = strstr(start+1, ":");
    len = (int)(end - start - 1);
    //printf("i = %d len = %d\n", i, len);
    if (len < 0 || len > PW_LEN) /* this should never happen */
      return 0;
    fields[i] = (char*)malloc(len + 1);
    strncpy(fields[i], start+1, len);
    fields[i][len] = 0;
    //printf("field %d = %s\n", i, fields[i]);
    start = end;
  }

  /* the last field should be the shell */
  fields[6] = (char*)malloc(PW_LEN+1);
  strncpy(fields[6], start+1, PW_LEN);

  /* prepare to copy information into the passwd structure */
  struct passwd * record = (struct passwd *)malloc(sizeof(struct passwd));
     
  /* first get the name */
  len = strlen(uname);
  record->pw_name = (char *)malloc(len + 1);
  strncpy(record->pw_name, uname, len);

  /* next get uid, the 3rd filed */
  record->pw_uid = atoi(fields[2]);

  /* next get gid, the 4th field */
  record->pw_gid = atoi(fields[3]);

  /* next get real name, the fifthe field */
  len = strlen(fields[4]);
  record->pw_gecos = (char*)malloc(len + 1);
  strncpy(record->pw_gecos, fields[4], len);

  /* next is the home directory, the sixth field */
  len = strlen(fields[5]);
  record->pw_dir = (char*)malloc(len + 1);
  strncpy(record->pw_dir, fields[5], len);
   
  /* last the login shell, if any */
  len = strlen(fields[6]);
  if (len <= 0 || strncmp(fields[6], "/bin/false", 10) == 0)
    return 0;  /* no good */

  /* we could also check the valid shells here, not implemented for now */
  record->pw_shell = (char*)malloc(len + 1);
  strncpy(record->pw_shell, fields[6], len);

  return record;
}
