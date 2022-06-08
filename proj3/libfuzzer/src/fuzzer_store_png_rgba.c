#include "pngparser.h"
#include <stdio.h>

// LibFuzzer stub
int LLVMFuzzerTestOneInput(char* filename) {
  struct image *test_img = NULL;

  if (load_png("testfile.png", &test_img) == 0) {
      store_png(filename, &test_img, NULL, 0);
      free(test_img);
  }

  return 0;
}
