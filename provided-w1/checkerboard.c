#include "pngparser.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>

#define DEBUG 1

int main(int argc, char *argv[]) {
  struct pixel palette[2]; // we have 2 pixels
  struct image *img = NULL; // initially no image

  if (argc != 7) { // we have to receive exactly 7 arguments 
    /*
    ./program 1 2 3 4 5 6 7
    Here, "./program" is the argument at index 0
    */
    goto error; // otherwise we raise error
  }

  /* Name the arguments */

  //* Pretty straightforward lines
  const char *output_name = argv[1];
  const char *height_arg = argv[2];
  const char *width_arg = argv[3];
  const char *square_width_arg = argv[4];
  const char *hex_color_arg1 = argv[5];
  const char *hex_color_arg2 = argv[6];
  //* End of pretty straightforward lines

  char *end_ptr; //? what is the purpose of this guy?

  /* Parse the arguments */

  //* Pretty straightforward lines
  if (strlen(hex_color_arg1) != 6) { // if hex size doesn't match
    goto error;
  }

  if (strlen(hex_color_arg2) != 6) { // if hex size doesn't match
    goto error;
  }
  //* End of pretty straightforward lines

  long height = strtol(height_arg, &end_ptr, 10);

  if (height <= 0 || height >= USHRT_MAX || *end_ptr) { 
    goto error;
  }

  long width = strtol(width_arg, &end_ptr, 10);
  /** the line right above
   * parsing the number from "width_arg"
   */

  if (width <= 0 || width >= USHRT_MAX || *end_ptr) {
    goto error;
  }

  unsigned long n_pixels = height * width; // calculating the number of pixels

  long color1 = strtol(hex_color_arg1, &end_ptr, 16);
  /** the line right above
   * parsing the number from "hex_color_arg1"
   */

  if (*end_ptr) { 
    goto error;
  }

  long color2 = strtol(hex_color_arg2, &end_ptr, 16);
  /** the line right above
   * parsing the number from "hex_color_arg2"
   */

  if (*end_ptr) {
    goto error;
  }

  /* Overflow checking */
  if (width > LONG_MAX / height || width < LONG_MIN / height) { // ? I didn't get the formula for the overflow check
    goto error;
  }

  long square_width = strtol(square_width_arg, &end_ptr, 10);
  /** the line right above
   * parsing the number from "hex_color_arg2"
   */

  if (square_width <= 0 || *end_ptr || square_width > height || height % square_width != 0) {
    goto error;
  }

  /* We assign colors to the palette */
  //* Not very straightforward lines
  palette[0].red = (color1 & 0xff0000) >> 16;
  palette[0].green = (color1 & 0x00ff00) >> 8;
  palette[0].blue = (color1 & 0x0000ff);
  palette[0].alpha = 0xff;
  // Hopefully no bugs here
  palette[1].red = (color2 & 0xff0000) >> 16;
  palette[1].green = (color2 & 0x00ff00) >> 8;
  palette[1].blue = (color2 & 0x0000ff);
  palette[1].alpha = 0xff;
  //* End of not very straightforward lines

  /* Memory allocation and error handling */

  if (DEBUG) printf("<1>\n");
  img = malloc(sizeof(struct image)); // allocating memory
  if (DEBUG) printf("<2>\n");

  if (!img) { // if img is null
    goto error_mem; // we raise memory error
  }

  img->px = malloc(sizeof(struct pixel) * n_pixels); // allocating memory for pixels array
  if (DEBUG) printf("<3>\n");

  if (!img->px) { // if array is null
    free(img); // free memory allocated for img
    goto error_img; // and raise memory error
  }

  if (DEBUG) printf("<4>\n");

  img->size_x = width; // pretty straightforward
  img->size_y = height; // pretty straightforward

  { // approximately understood what is going on in here
    struct pixel(*image_data)[width] = (struct pixel(*)[width])img->px; //? wat?

    /* We segment the image into squares and fill each square with its color */
    for (int i = 0; i < (height + square_width - 1) / square_width; i++) {
      for (int j = 0; j < (width + square_width - 1) / square_width; j++) {

        /* Calculate the color based on the square index */
        int color = (i + j) % 2;

        /* Fill a square */
        int square_top_left_x = j * square_width;
        int square_top_left_y = i * square_width;

        /* This iterates over a square and fills it with the correct color */
        for (int x = 0; x < square_width; x++) {
          for (int y = 0; y < square_width; y++) {
            image_data[square_top_left_y + y][square_top_left_x + x].red =
                palette[color].red;
            image_data[square_top_left_y + y][square_top_left_x + x].green =
                palette[color].green;
            image_data[square_top_left_y + y][square_top_left_x + x].blue =
                palette[color].blue;
            image_data[square_top_left_y + y][square_top_left_x + x].alpha =
                0xff;
          }
        }
      }
    }
  }

  if (DEBUG) printf("<5>\n");

  store_png(output_name, img, palette, 2);

  if (DEBUG) printf("<6>\n");

  free(img->px);
  free(img);

  if (DEBUG) printf("<7>\n");

  return 0;

/* Error handling code */
error:
  printf(
      "Usage: %s output_name height width square_width hex_color1 hex_color2\n",
      argv[0]);
  return 1;
error_px:
  free(img->px);
error_img:
  free(img);
error_mem:
  printf("Couldn't allocate memory\n");
  return 1;
}


/**
 * @brief Some interesting test cases
 * 1) 
 * ./checkerboard png_1.png 99 100 10 ffffff aaaaaa -> causes an error
 * ./checkerboard png_1.png 100 88 10 ffffff aaaaaa -> does not cause an error
 * @note There is something different in the way height and width are treated
 */