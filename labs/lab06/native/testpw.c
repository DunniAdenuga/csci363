#include <stdio.h>


/* the following function can be linked using -lcrypt */
extern char * crypt(const char *pass, const char *salt); 

/* the program demonstrates how crypt() generates the password */
int main(int argc, char* argv[]) {

    printf("test encrypting password ...\n");

    char salt[32];
    char pass[32];
    printf("type a 'salt' : ");
    scanf("%s", salt);
    printf("\n");
    printf("type password : ");
    scanf("%s", pass);
    printf("Encrypted password is : |%s|\n", crypt(pass, salt));
}
