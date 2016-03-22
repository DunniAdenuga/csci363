/*
 * The functions and structures in this file emulate the real
 * shadow password files. The purpose is to allow students
 * to exercise with these structures and files without temparing with
 * an actual system.
 *
 * For CSCI 363
 * Xiannong Meng
 * 2013-04-07
 */

/*
 * We simplify the shadow file to contain only the user name, group id,
 * and user id
 */
#ifndef _SHADOW_H_
#define _SHADOW_H_

#define SHADOW_FILE  "shadow"    /* the real file should be at /etc/shadow */

struct spwd {
    char *sp_namp;     /* Login name */
    char *sp_pwdp;     /* Encrypted password */
    long  sp_lstchg;   /* Date of last change (measured
                          in days since 1 Jan 1970) */
    long  sp_min;      /* Min # of days between changes */
    long  sp_max;      /* Max # of days between changes */
    long  sp_warn;     /* # of days before password expires
                          to warn user to change it */
    long  sp_inact;    /* # of days after password expires
                          until account is disabled */
    long  sp_expire;   /* Date when account expires (measured
                          in days since 1 Jan 1970) */
    unsigned long sp_flag;  /* Reserved */
};

/* Get shadow password entry for the given 'name' */
struct spwd * getspnam(const char * name);

#endif
