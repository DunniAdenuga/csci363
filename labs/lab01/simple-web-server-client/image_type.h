/*
 * The types of image checked so far include
 *    0    other
 *    1    jpeg
 *    2    gif
 *    3    png
 *    
 * Xiannong Meng xmeng@bucknell.edu
 * 2016-01-02
 */
#include <endian.h>

#include <fcntl.h>

#define OTHER 0
#define JPEG  1
#define GIF   2
#define PNG   3

int  determine_image_type(char * path);
void print_image_type(int);
