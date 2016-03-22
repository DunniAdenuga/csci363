/*
 * The structures and functions here emulate the behaviors of
 * that in <pwd.h> so students can exercise without tempering
 * the actual system files.
 *
 * Xiannong Meng
 * For CSCI 363
 * 2013-04-07
 */


#ifndef _PWD_H_
#define _PWD_H_

#define PASS_FILE "passwd"   /* real file should be /etc/passwd if used */

typedef unsigned int uid_t;
typedef unsigned int gid_t;

/* 
 * The following structure is identical to the system one.
 * See man getpwnam
 */
struct passwd {
    char   *pw_name;       /* username */
    char   *pw_passwd;     /* user password */
    uid_t   pw_uid;        /* user ID */
    gid_t   pw_gid;        /* group ID */
    char   *pw_gecos;      /* real name */
    char   *pw_dir;        /* home directory */
    char   *pw_shell;      /* shell program */
};

/* Returns the passwd structure for a given user name */
struct passwd * getpwnam(const char *name);

#endif
