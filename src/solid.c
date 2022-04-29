#include "pngparser.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>

#define OUTPUT_NAME_SIZE 500

/* We allocate the memory for the palette for this image */
struct pixel *allocate_palette() {
  struct pixel *ptr = malloc(sizeof(struct pixel));
  return ptr;
}

int main(int argc, char *argv[]) {
  struct image *img = NULL;
  struct pixel *palette = allocate_palette();

  if (!palette) { // if palette is not allocated
    goto error;
  }

  /*
   * goto statements should be used only in two cases:
   *
   * 1) Cleanup at the end of the function (HERE)
   * 2) To jump out of multiple for loops without using flags
   *
   */

  // The user needs to provide all arguments
  if (argc != 5) {
    goto error;
  }

  /* Assign names to arguments for better abstraction */
  char output_name[OUTPUT_NAME_SIZE];
  strncpy(output_name, argv[1],
          OUTPUT_NAME_SIZE); //? Why strcpy this? not just simply using pointer

  const char *height_arg = argv[2]; //? what if -9223372036854775807
  const char *width_arg = argv[3];
  const char *hex_color_arg = argv[4];
  char *end_ptr;

  if (strlen(hex_color_arg) != 6) {
    goto error;
  }

  unsigned long height = strtol(height_arg, &end_ptr, 10);
  printf("height is: %lu\n", height);

  /* If the user provides negative height or the height is 0 and the end_ptr
   * hasn't moved we issue an error and free palette
   */
  if (height == 0 || height >= USHRT_MAX ||
      *end_ptr) //! does not check for negative values and zero
    goto error; // wat, where are braces??

  unsigned long width = strtol(width_arg, &end_ptr, 10);
  printf("width is : %lu\n", width);

  if (width == 0 || width >= USHRT_MAX ||
      *end_ptr) { //! does not check for negative values and zero
    goto error;
  }

  unsigned n_pixels = height * width;

  long color = strtol(hex_color_arg, &end_ptr, 16);

  if (*end_ptr) {
    goto error;
  }

  palette[0].red = (color & 0xff0000) >> 16;
  palette[0].green = (color & 0x00ff00) >> 8;
  palette[0].blue = (color & 0x0000ff);
  palette[0].alpha = 0xff;

  /* After calling malloc we must check if it was successful */
  img = malloc(sizeof(struct image));
  if (!img) {
    goto error_mem;
  }

  img->px = malloc(sizeof(struct pixel) * n_pixels);
  if (!img->px) {
    goto error_img;
  }

  img->size_x = width;
  img->size_y = height;

  {
    /* Cast a pixel array into a 2D array.
     * We need extra brackets to prevent goto from jumping into the scope of the
     * new variable
     */
    struct pixel(*image_data)[img->size_x] =
        (struct pixel(*)[img->size_x])img->px;

    /* Iterate over a new image and fill it with color */
    for (int i = 0; i < img->size_y; i++) {
      for (int j = 0; j < img->size_x; j++) {
        image_data[i][j].red = palette[0].red;
        image_data[i][j].green = palette[0].green;
        image_data[i][j].blue = palette[0].blue;
        image_data[i][j].alpha = 0xff;
      }
    }
  }

  if (store_png(output_name, img, palette, 1)) {
    goto error_px;
  }

  free(img->px);
  free(img);

  return 0;

  /* We use goto to jump to the corresponding error handling code.
   * This gets rid of repetitive if chunks we'd use otherwise
   */

error:
  free(palette);
  printf("Usage: %s output_name height width hex_color\n", argv[0]);
  return 1;

error_px:
  free(img->px);
error_img:
  free(img);
error_mem:
  printf("Couldn't allocate memory\n");
  return 1;
}
