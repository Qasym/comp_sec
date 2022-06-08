#include "pngparser.h"
#include <stdio.h>

// LibFuzzer stub
int LLVMFuzzerTestOneInput(const char* name) {
  struct image *test_img;

  if (load_png(name, &test_img) == 0)
  	free(test_img);

  return 0;
}
