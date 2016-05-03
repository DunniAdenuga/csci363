/*
 * This program runs a basic test for Ross Williams CRC Model.
 * The program is created following the instruction from Ross Williams's
 * paper at
 * <http://www.ross.net/crc/crcpaper.html>
 * 
 * 03-30-2013
 * Xiannong Meng
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crcmodel.h"

/*
 * The following could be local variabls.
 * Global variables make the flow of computing easier.
 */
cm_t   cm;
p_cm_t p_cm = &cm;

void setup() {
  /*
   * Parameter set-up begins
   */

  p_cm->cm_width = 16;
  p_cm->cm_poly  = 0x8005L;       // CRC16
  //  p_cm->cm_poly  = 0x1021L;   // CRC-CCITT

  /*
   * parameter set-up ends
   */

  // one can initalize the proceeding bits wit 0s '0L' or 1s 'FFFFL'
  p_cm->cm_init  = 0L;            
  //  p_cm->cm_init  = 0xFFFFL;
  p_cm->cm_refin = TRUE;
  p_cm->cm_refot = TRUE;
  p_cm->cm_xorot = 0L;

  cm_ini(p_cm);
}

int
main(int argc, char* argv[]) {

  char   ch;

  //char * message = "A"; 
  char * message = "123456789"; // r.w. original example
                                // check-sum 0xbb3d

  // initialize the crc calculation
  setup();

  int i = 0;
  int len = strlen(message);
  for (i = 0; i < len; i ++) {
    ch = message[i];
    cm_nxt(p_cm, ch);
  }
  ulong crc = cm_crc(p_cm);
  printf("crc value 0x%04x\n", crc);

  int crc_len = p_cm->cm_width/8;
  int new_len = len + crc_len;
  char *msg_to_send = (char*)malloc(new_len); // extra bytes for crc

  memmove(msg_to_send, message, len);
  memmove(&(msg_to_send[len]), &crc, crc_len);

  // initialize the crc calculation
  setup();

  // a byte is altered, so the checksum should be non-zero
  // comment out the following line to see the checksum of zero
  //msg_to_send[0] ++;   

  for (i = 0; i < new_len; i ++) {
    ch = msg_to_send[i];
    cm_nxt(p_cm, ch);
  }

  crc = cm_crc(p_cm);   // could be called in the loop on partial result
  printf("crc value after verification 0x%04x\n", crc);

  return 0;
}
