/*
 * These functions are listed at
 * http://hayageek.com/rsa-encryption-decryption-openssl-c/
 *
 * Downloaded 02-20-2016
 * Xiannong Meng
 *
 * compile the program with the ssl library "-lssl"
 *
 * With minor revision and reorganization by X. Meng 02-20-2016
 *
 * From the article above, here is how to generate private and public
 * keys.
 *
 * Use the below command to generate RSA keys with length of 2048.
 * 
 * openssl genrsa -out private.pem 2048
 * Extract public key from private.pem with the following command.
 * 
 * openssl rsa -in private.pem -outform PEM -pubout -out public.pem
 *
 * public.pem is RSA public key in PEM format.
 * private.pem is RSA private key in PEM format.
 */

#include "rsa.h"

int padding = RSA_PKCS1_PADDING;
 
RSA * 
createRSA(unsigned char * key, int public)
{
  RSA *rsa= NULL;
  BIO *keybio ;
  keybio = BIO_new_mem_buf(key, -1);
  if (keybio==NULL)
    {
      printf( "Failed to create key BIO");
      return 0;
    }
  if(public)
    {
      rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
    }
  else
    {
      rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
    }
  if(rsa == NULL)
    {
      printf( "Failed to create RSA");
    }
 
  return rsa;
}
 
int 
public_encrypt(unsigned char * data, int data_len,
	       unsigned char * key, unsigned char *encrypted)
{
  RSA * rsa = createRSA(key,1);
  int result = RSA_public_encrypt(data_len,data,encrypted,rsa,padding);
  return result;
}

int 
private_decrypt(unsigned char * enc_data, int data_len,
		unsigned char * key, unsigned char *decrypted)
{
  RSA * rsa = createRSA(key,0);
  int  result = RSA_private_decrypt(data_len,enc_data,decrypted,rsa,padding);
  return result;
}
 
 
int 
private_encrypt(unsigned char * data, int data_len,
		unsigned char * key, unsigned char *encrypted)
{
  RSA * rsa = createRSA(key,0);
  int result = RSA_private_encrypt(data_len,data,encrypted,rsa,padding);
  return result;
}

int 
public_decrypt(unsigned char * enc_data, int data_len,
	       unsigned char * key, unsigned char *decrypted)
{
  RSA * rsa = createRSA(key,1);
  int  result = RSA_public_decrypt(data_len,enc_data,decrypted,rsa,padding);
  return result;
}
 
void 
printLastError(char *msg)
{
  char * err = malloc(130);;
  ERR_load_crypto_strings();
  ERR_error_string(ERR_get_error(), err);
  printf("%s ERROR: %s\n",msg, err);
  free(err);
}
