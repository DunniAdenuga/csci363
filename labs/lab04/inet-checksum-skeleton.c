#include <stdint.h>

/*
 *-------------------------------------------------------------------
 * Computes checksum of UDP packet, though the function can be applied
 * to any other internet data units.
 *
 * parameters:
 *   ptr:    pointer to data
 *   nbytes: length of data, assume nbytes < 2^(16) - 1
 * 
 * returns: Internet checksum of this unit of data
 *------------------------------------------------------------------- 
 */
/*
sum = 0;
for each of the 16-bit unit in UDP // including header and payload
  sum = sum + 16-bit unit;
  add the carry-over to the rightmost;
sum = one's complement of sum;
 */

int
  inet_checksum(uint16_t * ptr, int nbytes) {
  //initialize sum to zero
  uint32_t sum=0;
  
  int i;
  // calculate the sum of all 16 vit words
  int k = 0;
  for (i=0; i < nbytes;i=i+2){
  
    sum = sum + (uint32_t)ptr[k];
    k = k + 1;
    }
  if(nbytes%2 != 0){//odd
    sum = sum + (uint32_t)ptr[k];
  }
  
  while (sum>>16){
    sum = (sum & 0xFFFF)+(sum >> 16);
  }
  uint16_t retval = sum; 
  retval = ~retval;
  // Take the one's complement of sum
  //sum = ~sum;
  //return (0x0000ffff ^ ~retval);
  //return ((uint16_t) sum);
  return retval;
  }




