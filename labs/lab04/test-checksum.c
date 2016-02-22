#include <stdio.h>
#include <stdint.h>

extern int inet_checksum(uint16_t *, int);

int
main(int argc, char * argv[]) {

  int chk_sum;
  int num_bytes = 0;
  uint16_t data1[] = {1, 1};
  uint16_t data2[] = {26208, 21845, 36620}; // Kurose 6th ed, p 203
  uint16_t data3[] = {26208, 21845, 65};    // odd num of bytes

  num_bytes = 4; // 4 bytes for data1
  chk_sum = inet_checksum(data1, 4); // expect 65533
  if (chk_sum == 65533) 
    printf("result 1 is good %d\n", chk_sum);
  else
    printf("result 1 is not good %d\n", chk_sum);

  num_bytes = 6; // 6 bytes for data2
  chk_sum = inet_checksum(data2, 6); // expect 46397
  if (chk_sum == 46397)
    printf("result 2 is good %d\n", chk_sum);
  else
    printf("result 2 is not good %d\n", chk_sum);

  num_bytes = 5; // 5 bytes for data3
  chk_sum = inet_checksum(data3, 5); // expect 17417
  if (chk_sum == 17417)
    printf("result 3 is good %d\n", chk_sum);
  else
    printf("result 3 is not good %d\n", chk_sum);


  return 0;
}
