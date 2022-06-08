#include "pngparser.h"
#include <stdio.h>

// LibFuzzer stub
int LLVMFuzzerTestOneInput(const uint8_t* Data, size_t Size) {
  struct image *test_img = NULL;

  const char* filename = (const char*) Data;

  int len = sizeof(Data) / sizeof(uint8_t);
  struct pixel* palette = malloc(sizeof(struct pixel) * len);
  for (int i = 0; i < len; i++) {
    palette[i].red = 15;
    palette[i].green = 15;
    palette[i].blue = 15;
    palette[i].alpha = 15;
  }

  if (load_png("testfile.png", &test_img) == 0) {
      if (store_png(filename, &test_img, palette, Size) == 0) {
          free(palette);
      }
      free(test_img);
  }

  return 0;
}
