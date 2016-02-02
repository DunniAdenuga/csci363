/*
 * The function "determine_image_type()" is an ad-hoc way
 * to determine the type of a popular image file. The 
 * information is based on /usr/share/misc/magic.mime.
 * The types of image checked so far include
 *    0    other
 *    1    jpeg
 *    2    gif
 *    3    png
 *    
 * Xiannong Meng xmeng@bucknell.edu
 * 2016-01-02
 */

/*
 * From the file /usr/share/misc/magic we have the following:
 *  JPEG: 0    beshort    0xffd8                     (2 bytes)
 *  GIF:  0    string     GIF8                       (4 bytes)
 *  PNG:  0    string     \x89PNG\x0d\x0a\x1a\x0a    (8 bytes)
 */
#include <stdio.h>
#include <string.h>
#include "image_type.h"

/*
 *-------------------------------------------------------------
 * The function "determine_image_type()" is an ad-hoc way
 * to determine the type of a popular image file. The 
 * information is based on /usr/share/misc/magic.mime.
 *
 * Parameters:
 *    path: the name, including path, of the file to check
 *
 * Return:
 *    The type of the 
 *-------------------------------------------------------------*/
int determine_image_type(char * path) {

  char data[8]; // maximum bytes to read/test
  int fd = open(path, O_RDONLY);
  
  if (fd == -1)  // something is wrong, return default
    return OTHER;

  read(fd, data, 8);

  /* determine the type of image, see /usr/share/misc/magic */
  // JPEG  0 beshort 0xffd8                   (2 bytes)
  ushort  test_s;
  memcpy(&test_s, data, sizeof(ushort));
  if (htobe16(test_s) == 0xffd8)
    return JPEG;

  // GIF  0 string GIF8                       (4 bytes)
  uint test_i;
  memcpy(&test_i, data, sizeof(uint));
  if (htobe32(test_i) == 0x47494638)
    return GIF;

  // PNG  0 string \x89PNG\x0d\x0a\x1a\x0a    (8 bytes)
  ulong test_l;
  memcpy(&test_l, data, sizeof(ulong));
  if (htobe64(test_l) == 0x89504e470d0a1a0a)
    return PNG;

  return OTHER;
}

/*
 *--------------------------------------------------------
 * Given a image type in integer, print its string equivelant
 *--------------------------------------------------------
 */
void print_image_type(int t) {

  switch (t) {
  case OTHER: printf("plain text\n");
    break;
  case JPEG:  printf("jpeg\n");
    break;
  case GIF:   printf("gif\n");
    break;
  case PNG:   printf("png\n");
    break;
  default:    printf("unknown\n");
  }
}
