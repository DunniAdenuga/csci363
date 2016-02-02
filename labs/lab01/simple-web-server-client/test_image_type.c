#include <stdio.h>
#include <stdlib.h>

#include "image_type.h"

int
main(int argc, char * argv[]) {

  if (argc != 2) {
    fprintf(stderr, "usage: %s image_file_name\n", argv[0]);
    exit(1);
  }

  int img_type = determine_image_type(argv[1]);
  print_image_type(img_type);

  return 0;
}
