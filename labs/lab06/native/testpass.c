#include "shadow.h"
#include "pwd.h"
#include <stdio.h>
#include <unistd.h>

/* 
 * The program demonstrates how to get password entry and
 * shadow entry.
 *
 * The program assumes that a 'passwd' file and a 'shadow' file
 * exist in the current directory.
 */
int main(int argc, char *argv[]) {

    struct spwd * sp = getspnam("xiannong");
    char pass[32];

    if (sp == NULL) {
       printf("get name failed.\n");
    } else {
       printf("name -> %s\n", sp->sp_namp);
       printf("pass -> %s\n", sp->sp_pwdp);
       printf("type your password: ->");
       scanf("%s", pass);
       if (strcmp(crypt(pass, sp->sp_pwdp), sp->sp_pwdp) == 0)
          printf("password match\n");
       else
          printf("password doesn't match\n");
    }

    struct passwd * pw = getpwnam("xiannong");
    if (pw == NULL) {
       printf("get passwd faild.\n");
    } else {
       printf("name -> %s\n", pw->pw_name);
       printf("real name -> %s\n", pw->pw_gecos);
       printf("uid -> %u\n", pw->pw_uid);
       printf("shell -> %s\n", pw->pw_shell);
    }

    return 0;
}
