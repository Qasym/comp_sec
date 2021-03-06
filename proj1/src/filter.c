#include "pngparser.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define ARG_SIZE 255
#define DEBUG 0

/* This filter iterates over the image and calculates the average value of the
 * color channels for every pixel This value is then written to all the channels
 * to get the grayscale representation of the image
 */
void filter_grayscale(struct image *img, void *weight_arr) {
  struct pixel(*image_data)[img->size_x] =
      (struct pixel(*)[img->size_x])img->px;
  double *weights = (double *)weight_arr;

  for (unsigned short i = 0; i < img->size_y; i++) {
    for (unsigned short j = 0; j < img->size_x; j++) {
      double luminosity = 0;

      luminosity += weights[0] * image_data[i][j].red;
      luminosity += weights[1] * image_data[i][j].green;
      luminosity += weights[2] * image_data[i][j].blue;

      image_data[i][j].red = (uint8_t)luminosity;
      image_data[i][j].green = (uint8_t)luminosity;
      image_data[i][j].blue = (uint8_t)luminosity;
    }
  }
}

/* This filter blurs an image. The larger the radius, the more noticeable the
 * blur.
 *
 * For every pixel we define a square of side 2*radius+1 centered around it.
 * The new value of the pixel is the average value of all pixels in the square.
 *
 * Pixels of the square which fall outside the image do not count towards the
 * average. They are ignored (e.g. 5x5 box will turn into a 3x3 box in the
 * corner).
 *
 */
void filter_blur(struct image *img, void *r) {
  struct pixel(*image_data)[img->size_x] =
      (struct pixel(*)[img->size_x])img->px;
  int radius = *((int *)r); // cast to int* and dereference
  if (radius < 0) {
    radius = 0;
  }

  struct pixel(*new_data)[img->size_x] =
      malloc(sizeof(struct pixel) * img->size_x * img->size_y);

  if (!new_data) {
    return;
  }

  /* We iterate over all pixels */
  for (long i = 0; i < img->size_y; i++) {
    for (long j = 0; j < img->size_x; j++) {

      unsigned red = 0, green = 0, blue = 0, alpha = 0;
      /* We iterate over all pixels in the square */
      for (long y_offset = ((-radius < -i) ? -i : -radius);
           y_offset + i < img->size_y && y_offset < radius; y_offset++) {
        for (long x_offset = ((-radius < -j) ? -j : -radius);
             x_offset + j < img->size_x && x_offset < radius; x_offset++) {

          struct pixel current = image_data[i + y_offset][j + x_offset];

          red += current.red;
          blue += current.blue;
          green += current.green;
          alpha += current.alpha;
        }
      }

      int num_pixels = (2 * radius + 1) * (2 * radius + 1);
      /* Calculate the average */
      red /= num_pixels;
      green /= num_pixels;
      blue /= num_pixels;
      alpha /= num_pixels;

      /* Assign new values */
      new_data[i][j].red = red;
      new_data[i][j].green = green;
      new_data[i][j].blue = blue;
      new_data[i][j].alpha = alpha;
    }
  }

  free(img->px);
  img->px = (struct pixel *)new_data;
  return;
}

/* We allocate and return a pixel */
struct pixel *get_pixel() {
  struct pixel *px;
  px = malloc(sizeof(struct pixel));
  return px;
}

/* This filter just negates every color in the image */
void filter_negative(struct image *img, void *noarg) {
  struct pixel(*image_data)[img->size_x] =
      (struct pixel(*)[img->size_x])img->px;

  /* Iterate over all the pixels */
  for (long i = 0; i < img->size_y; i++) {
    for (long j = 0; j < img->size_x; j++) {

      struct pixel current = image_data[i][j];
      struct pixel *neg = get_pixel();

      /* The negative is just the maximum minus the current value */
      neg->red = 255 - current.red;
      neg->green = 255 - current.green;
      neg->blue = 255 - current.blue;
      neg->alpha = current.alpha;

      /* Write it back */
      image_data[i][j] = *neg;
    }
  }
}

/* Set the transparency of the picture to the value (0-255) passed as argument
 */
void filter_transparency(struct image *img, void *transparency) {
  struct pixel(*image_data)[img->size_x] =
      (struct pixel(*)[img->size_x])img->px;
  uint8_t local_alpha = *((uint8_t *)transparency);

  /* Iterate over all pixels */
  for (long i = 0; i < img->size_y; i++) {
    for (long j = 0; j < img->size_x; j++) {

      image_data[i][j].alpha = local_alpha;
    }
  }
}

void printArr(long *arr) {
  printf("<array_contents>\n");
  for (int i = 0; i < 9; i++) {
    printf("%d\n", arr[i]);
  }
  printf("<array_contents>\n");
}

double detection_helper_mult(long *pixels, double *weights) {
  if (DEBUG)
    printf("<start: detection_helper_mult>\n");

  if (DEBUG)
    printArr(pixels);

  double result = 0;
  if (DEBUG)
    printf("<1>\n");
  if (DEBUG)
    printf("pixels[0] = %d ;; weights[0][0] = %f\n", pixels[0], weights[0]);
  result += pixels[0] * weights[0];
  if (DEBUG)
    printf("<2>\n");
  result += pixels[1] * weights[1];
  if (DEBUG)
    printf("<3>\n");
  result += pixels[2] * weights[2];
  if (DEBUG)
    printf("<4>\n");
  result += pixels[3] * weights[3];
  if (DEBUG)
    printf("<5>\n");
  result += pixels[4] * weights[4];
  if (DEBUG)
    printf("<6>\n");
  result += pixels[5] * weights[5];
  if (DEBUG)
    printf("<7>\n");
  result += pixels[6] * weights[6];
  if (DEBUG)
    printf("<8>\n");
  result += pixels[7] * weights[7];
  if (DEBUG)
    printf("<9>\n");
  result += pixels[8] * weights[8];
  if (DEBUG)
    printf("<10>\n");

  if (DEBUG)
    printf("<end: detection_helper_mult>\n");
  return result;
}

/* This filter is used to detect edges by computing the gradient for each
 * pixel and comparing it to the threshold argument. When the gradient exceeds
 * the threshold, the pixel is replaced by black, otherwise white.
 * Alpha is unaffected.
 *
 * For each pixel and channel, the x-gradient and y-gradient are calculated
 * by using the following convolution matrices:
 *     Gx            Gy
 *  -1  0  +1     +1 +2 +1
 *  -2  0  +2      0  0  0
 *  -1  0  +1     -1 -2 -1
 * The convolution matrix are multiplied with the neighbouring pixels'
 * channel values. At edges, the indices are bounded.
 * Suppose the red channel values for the pixel and its neighbours are as
 * follows: 11 22 33 44 55 66 77 88 99 the x-gradient for red is: (-1*11 + 1*33
 * + -2*44 + 2*66 + -1*77 + 1*99).
 *
 * The net gradient for each channel = sqrt(g_x^2 + g_y^2)
 * For the pixel, the net gradient = sqrt(g_red^2 + g_green^2 + g_blue_2)
 */
void filter_edge_detect(struct image *img, void *threshold_arg) {
  if (DEBUG)
    printf("<1>\n");

  struct pixel(*image_data)[img->size_x] =
      (struct pixel(*)[img->size_x])img->px;
  uint8_t threshold = *(uint8_t *)threshold_arg;
  double weights_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
  double weights_y[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};

  double weightsX[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
  double weightsY[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
  double g_red_x, g_red_y, g_green_x, g_green_y, g_blue_x, g_blue_y;
  double g_red, g_green, g_blue;

  double g_total[img->size_y][img->size_x];

  long pixels[9];

  if (DEBUG)
    printf("<2>\n");

  for (long i = 0; i < img->size_y; i++) {
    for (long j = 0; j < img->size_x; j++) {
      if (DEBUG)
        printf("<3>\n");

      pixels[0] =
          image_data[(i - 1) < 0 ? 0 : i - 1][(j - 1) < 0 ? 0 : j - 1].red;
      pixels[1] = image_data[(i - 1) < 0 ? 0 : i - 1][j].red;
      pixels[2] = image_data[(i - 1) < 0 ? 0 : i - 1]
                            [(j + 1) >= img->size_x ? img->size_x - 1 : j + 1]
                                .red;
      pixels[3] = image_data[i][(j - 1) < 0 ? 0 : j - 1].red;
      pixels[4] = image_data[i][j].red;
      pixels[5] =
          image_data[i][(j + 1) >= img->size_x ? img->size_x - 1 : j + 1].red;
      pixels[6] = image_data[(i + 1) >= img->size_y ? img->size_y - 1 : i + 1]
                            [(j - 1) < 0 ? 0 : j - 1]
                                .red;
      pixels[7] =
          image_data[(i + 1) >= img->size_y ? img->size_y - 1 : i + 1][j].red;
      pixels[8] = image_data[(i + 1) >= img->size_y ? img->size_y - 1 : i + 1]
                            [(j + 1) >= img->size_x ? img->size_x - 1 : j + 1]
                                .red;

      if (DEBUG)
        printArr(pixels);

      if (DEBUG)
        printf("<4>\n");

      g_red_x = detection_helper_mult(pixels, weightsX);
      g_red_y = detection_helper_mult(pixels, weightsY);

      if (DEBUG)
        printf("<5>\n");

      pixels[0] =
          image_data[(i - 1) < 0 ? 0 : i - 1][(j - 1) < 0 ? 0 : j - 1].green;
      pixels[1] = image_data[(i - 1) < 0 ? 0 : i - 1][j].green;
      pixels[2] = image_data[(i - 1) < 0 ? 0 : i - 1]
                            [(j + 1) >= img->size_x ? img->size_x - 1 : j + 1]
                                .green;
      pixels[3] = image_data[i][(j - 1) < 0 ? 0 : j - 1].green;
      pixels[4] = image_data[i][j].green;
      pixels[5] =
          image_data[i][(j + 1) >= img->size_x ? img->size_x - 1 : j + 1].green;
      pixels[6] = image_data[(i + 1) >= img->size_y ? img->size_y - 1 : i + 1]
                            [(j - 1) < 0 ? 0 : j - 1]
                                .green;
      pixels[7] =
          image_data[(i + 1) >= img->size_y ? img->size_y - 1 : i + 1][j].green;
      pixels[8] = image_data[(i + 1) >= img->size_y ? img->size_y - 1 : i + 1]
                            [(j + 1) >= img->size_x ? img->size_x - 1 : j + 1]
                                .green;

      if (DEBUG)
        printf("<6>\n");

      g_green_x = detection_helper_mult(pixels, weightsX);
      g_green_y = detection_helper_mult(pixels, weightsY);

      if (DEBUG)
        printf("<7>\n");

      pixels[0] =
          image_data[(i - 1) < 0 ? 0 : i - 1][(j - 1) < 0 ? 0 : j - 1].blue;
      pixels[1] = image_data[(i - 1) < 0 ? 0 : i - 1][j].blue;
      pixels[2] = image_data[(i - 1) < 0 ? 0 : i - 1]
                            [(j + 1) >= img->size_x ? img->size_x - 1 : j + 1]
                                .blue;
      pixels[3] = image_data[i][(j - 1) < 0 ? 0 : j - 1].blue;
      pixels[4] = image_data[i][j].blue;
      pixels[5] =
          image_data[i][(j + 1) >= img->size_x ? img->size_x - 1 : j + 1].blue;
      pixels[6] = image_data[(i + 1) >= img->size_y ? img->size_y - 1 : i + 1]
                            [(j - 1) < 0 ? 0 : j - 1]
                                .blue;
      pixels[7] =
          image_data[(i + 1) >= img->size_y ? img->size_y - 1 : i + 1][j].blue;
      pixels[8] = image_data[(i + 1) >= img->size_y ? img->size_y - 1 : i + 1]
                            [(j + 1) >= img->size_x ? img->size_x - 1 : j + 1]
                                .blue;

      if (DEBUG)
        printf("<8>\n");

      g_blue_x = detection_helper_mult(pixels, weightsX);
      g_blue_y = detection_helper_mult(pixels, weightsY);

      if (DEBUG)
        printf("<9>\n");

      g_red = sqrt(g_red_x * g_red_x + g_red_y * g_red_y);
      g_green = sqrt(g_green_x * g_green_x + g_green_y * g_green_y);
      g_blue = sqrt(g_blue_x * g_blue_x + g_blue_y * g_blue_y);

      if (DEBUG)
        printf("<10>\n");

      g_total[i][j] = sqrt(g_red * g_red + g_green * g_green + g_blue * g_blue);

      if (DEBUG)
        printf("<11>\n");
    }
  }

  for (long i = 0; i < img->size_y; i++) {
    for (long j = 0; j < img->size_x; j++) {
      image_data[i][j].red = (g_total[i][j] > threshold) ? 0 : 255;
      image_data[i][j].green = (g_total[i][j] > threshold) ? 0 : 255;
      image_data[i][j].blue = (g_total[i][j] > threshold) ? 0 : 255;
    }
  }

  if (DEBUG)
    printf("<return filter_edge_detect>\n");
}

/* The filter structure comprises the filter function, its arguments and the
 * image we want to process */
struct filter {
  void (*filter)(struct image *img, void *arg);
  void *arg;
  struct image *img;
};

void execute_filter(struct filter *fil) { fil->filter(fil->img, fil->arg); }

int __attribute__((weak)) main(int argc, char *argv[]) {
  struct filter fil;
  char input[ARG_SIZE];
  char output[ARG_SIZE];
  char command[ARG_SIZE];
  char arg[ARG_SIZE];
  int radius;
  struct pixel px;
  uint8_t alpha, depth, threshold;
  uint32_t key;
  struct image *img = NULL;
  double weights[] = {0.2125, 0.7154, 0.0721};

  /* Some filters take no arguments, while others have 1 */
  if (argc != 4 && argc != 5) {
    goto error_usage;
  }

  fil.filter = NULL;
  fil.arg = NULL;

  /* Copy arguments for easier reference */
  strncpy(input, argv[1], ARG_SIZE);
  strncpy(output, argv[2], ARG_SIZE);
  strncpy(command, argv[3], ARG_SIZE);

  /* If the filter takes an argument, copy it */
  if (argv[4]) {
    strncpy(arg, argv[4], ARG_SIZE); //! string vulnerability
  }

  /* Error when loading a png image */
  if (load_png(input, &img)) {
    printf(input);
    printf(" PNG file cannot be loaded\n");
    exit(1);
  }

  /* Set up the filter structure */
  fil.img = img;

  /* Decode the filter */
  if (!strcmp(command, "grayscale")) {
    fil.filter = filter_grayscale;
    fil.arg = weights;
  } else if (!strcmp(command, "negative")) {
    fil.arg = NULL;
    fil.filter = filter_negative;
  } else if (!strcmp(command, "blur")) {
    /* Bad filter radius will just be interpretted as 0 - no change to the image
     */
    radius = atoi(arg);
    fil.filter = filter_blur;
    fil.arg = &radius;
  } else if (!strcmp(command, "alpha")) {

    char *end_ptr;
    long tmp_alpha = strtol(arg, &end_ptr, 16);

    if (tmp_alpha < 0 || tmp_alpha > 255) {
      goto error_usage;
    } else {
      alpha = tmp_alpha;
    }

    if (*end_ptr) {
      goto error_usage;
    }

    fil.filter = filter_transparency;
    fil.arg = &alpha;
  } else if (!strcmp(command, "edge")) {
    char *end_ptr;
    threshold = strtol(arg, &end_ptr, 16);

    if (*end_ptr) {
      goto error_usage;
    }

    fil.filter = filter_edge_detect;
    fil.arg = &threshold;
  }

  /* Invalid filter check */
  if (fil.filter) {
    execute_filter(&fil);
  } else {
    goto error_filter;
  }

  store_png(output, img, NULL, 0);
  free(img->px);
  free(img);
  return 0;

error_filter:
  free(img->px);
  free(img);
error_usage:
  printf("Usage: %s input_image output_image filter_name [filter_arg]\n",
         argv[0]);
  printf("Filters:\n");
  printf("grayscale\n");
  printf("negative\n");
  printf("blur radius_arg\n");
  printf("alpha hex_alpha\n");
  printf("edge hex_threshold\n");
  return 1;
}
