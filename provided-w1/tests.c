#include "filter.h"
#include <assert.h>
#include <check.h>
#include <float.h>
#include <limits.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

struct image generate_rand_img() {
  struct image img;
  do {
    img.size_x = rand() % 128;
  } while (img.size_x == 0);
  do {
    img.size_y = rand() % 128;
  } while (img.size_y == 0);
  img.px = malloc(img.size_x * img.size_y * sizeof(struct pixel));
  if (img.px == NULL)
    assert(0 && "Rerun test, malloc failed");
  for (long i = 0; i < img.size_y * img.size_x; i++) {
    img.px[i].red = rand();
    img.px[i].green = rand();
    img.px[i].blue = rand();
    img.px[i].alpha = rand();
  }

  return img;
}
struct image duplicate_img(struct image img) {
  struct image img_dup;

  img_dup.size_x = img.size_x;
  img_dup.size_y = img.size_y;
  img_dup.px = malloc(img.size_x * img.size_y * sizeof(struct pixel));
  if (img_dup.px == NULL)
    assert(0 && "Rerun test, malloc failed");
  for (long i = 0; i < img.size_y * img.size_x; i++) {
    img_dup.px[i].red = img.px[i].red;
    img_dup.px[i].green = img.px[i].green;
    img_dup.px[i].blue = img.px[i].blue;
    img_dup.px[i].alpha = img.px[i].alpha;
  }

  return img_dup;
}

/* Grayscale function should not crash when the weights are
 * at the limits of the double data size */
START_TEST(grayscale_double_limit) {
  srand(time(NULL) ^ getpid());

  /* Generate random png image */
  struct image img = generate_rand_img();

  /* Limiting cases of double */
  double weight_limits[] = {DBL_MIN, DBL_TRUE_MIN, DBL_MAX, DBL_EPSILON,
                            DBL_MIN_EXP};
  int n_limits = sizeof(weight_limits) / sizeof(weight_limits[0]);

  /* grayscale requires array of 3 weights */
  for (unsigned i0 = 0; i0 < n_limits; i0++)
    for (unsigned i1 = 0; i1 < n_limits; i1++)
      for (unsigned i2 = 0; i2 < n_limits; i2++) {
        double weights[3] = {weight_limits[i0], weight_limits[i1],
                             weight_limits[i2]};
        /* Merely checking that the function does not crash */
        filter_grayscale(&img, weights);
      }

  /* Not strictly necessary unless using CK_FORK=no */
  free(img.px);
}
END_TEST

START_TEST(grayscale_functionality) {
  srand(time(NULL) ^ getpid());

  /* Generate random png image */
  struct image img = generate_rand_img();
  uint8_t rand_alpha = rand();
  double weights[] = {0, 0, 0};
  uint16_t sz_x = img.size_x, sz_y = img.size_y;
  for (long i = 0; i < sz_y; i++)
    for (long j = 0; j < sz_x; j++)
      img.px[i * sz_x + j].alpha = rand_alpha;

  filter_grayscale(&img, weights);

  ck_assert_uint_eq(img.size_x, sz_x);
  ck_assert_uint_eq(img.size_y, sz_y);
  ck_assert_ptr_ne(img.px, NULL);
  for (long i = 0; i < sz_y; i++)
    for (long j = 0; j < sz_x; j++) {
      long idx = i * sz_x + j;
      ck_assert_uint_eq(img.px[idx].red, 0);
      ck_assert_uint_eq(img.px[idx].green, 0);
      ck_assert_uint_eq(img.px[idx].blue, 0);
      ck_assert_uint_eq(img.px[idx].alpha, rand_alpha);
    }

  free(img.px);
}
END_TEST

START_TEST(edge_threshold) {
  struct image img;
  struct pixel pxl;
  uint8_t threshold;

  img.size_x = img.size_y = 1;
  img.px = &pxl;

  /* These calls should not crash */
  threshold = 0;
  filter_edge_detect(&img, &threshold);
  threshold = 255;
  filter_edge_detect(&img, &threshold);
}
END_TEST

char *edge_deserts[] = {
    "test_imgs/desert_edge_40.png", "test_imgs/desert_edge_80.png",
    "test_imgs/desert_edge_c0.png", "test_imgs/desert_edge_e0.png"};
uint8_t edge_thresholds[] = {0x40, 0x80, 0xc0, 0xe0};

START_TEST(edge_example_image) {
  struct image *img, *img_edge, img_dup;

  ck_assert_int_eq(load_png("test_imgs/desert.png", &img), 0);
  img_dup = duplicate_img(*img);
  filter_edge_detect(img, &edge_thresholds[_i]);

  /* Compare to known good image */
  ck_assert_int_eq(load_png(edge_deserts[_i], &img_edge), 0);

  ck_assert_uint_eq(img_edge->size_x, img->size_x);
  ck_assert_uint_eq(img_edge->size_y, img->size_y);
  for (long j = 0; j < img->size_x * img->size_y; j++) {
    ck_assert_uint_eq(img_edge->px[j].red, img->px[j].red);
    ck_assert_uint_eq(img_edge->px[j].green, img->px[j].green);
    ck_assert_uint_eq(img_edge->px[j].blue, img->px[j].blue);
    ck_assert_uint_eq(img_dup.px[j].alpha, img->px[j].alpha);
  }
  free(img_dup.px);
  free(img_edge->px);
  free(img->px);
  free(img_edge);
  free(img);
}
END_TEST

START_TEST(edge_checkerboard) {
  struct image *img, *img_edge, img_dup;

  ck_assert_int_eq(load_png("test_imgs/ck.png", &img), 0);
  uint8_t threshold = 0x10;
  img_dup = duplicate_img(*img);
  filter_edge_detect(img, &threshold);

  /* Compare to known good image */
  ck_assert_int_eq(load_png("test_imgs/ck_edge.png", &img_edge), 0);

  ck_assert_uint_eq(img_edge->size_x, img->size_x);
  ck_assert_uint_eq(img_edge->size_y, img->size_y);
  for (long j = 0; j < img->size_x * img->size_y; j++) {
    ck_assert_uint_eq(img_edge->px[j].red, img->px[j].red);
    ck_assert_uint_eq(img_edge->px[j].green, img->px[j].green);
    ck_assert_uint_eq(img_edge->px[j].blue, img->px[j].blue);
    ck_assert_uint_eq(img_dup.px[j].alpha, img->px[j].alpha);
  }
  free(img_dup.px);
  free(img_edge->px);
  free(img->px);
  free(img_edge);
  free(img);
}
END_TEST

char *grayscale_sources[] = {"test_imgs/desert.png", "test_imgs/summer.png"};
char *grayscale_output[] = {"test_imgs/desert_gray.png",
                            "test_imgs/summer_gray.png"};
START_TEST(grayscale_examples) {
  double weights[] = {0.2125, 0.7154, 0.0721};
  struct image *img, *out;
  // load images
  ck_assert_int_eq(load_png(grayscale_sources[_i], &img), 0);
  
  // apply grayscale filter
  filter_grayscale(img, weights);

  // load correct images
  ck_assert_int_eq(load_png(grayscale_output[_i], &out), 0);

  // compare out and img
  ck_assert_uint_eq(img->size_y, out->size_y);
  long limit = img->size_x * img->size_y;
  for (int i = 0; i < limit; i++) {
    ck_assert_int_eq(img->px[i].red, out->px[i].red);
    ck_assert_int_eq(img->px[i].green, out->px[i].green);
    ck_assert_int_eq(img->px[i].blue, out->px[i].blue);
    ck_assert_int_eq(img->px[i].alpha, out->px[i].alpha);
  }
}
END_TEST

/* Verify that the black image is inverted properly to a white image.
 * Then invert the result again and verify that you get a black image back
 * The alpha channel needs to be intact in both cases */
START_TEST(negative_functionality) {
  srand(time(NULL) ^ getpid());

  struct image img;
  long limit;

  // setting width and height
  do {
    img.size_x = rand() % 128;
  } while (img.size_x == 0);
  do {
    img.size_y = rand() % 128;
  } while (img.size_y == 0);

  // allocating memory for pixels
  limit = img.size_x * img.size_y;
  img.px = malloc(limit * sizeof(struct pixel));
  if (img.px == NULL)
    assert(0 && "Rerun test, malloc failed");

  // setting colors
  for (int i = 0; i < limit; i++) { 
    img.px[i].red = 0;
    img.px[i].green = 0;
    img.px[i].blue = 0;
    img.px[i].alpha = 128;
  }

  // inverting
  filter_negative(&img, NULL);

  // checking the inverted image
  for (int i = 0; i < limit; i++) {
    ck_assert_uint_eq(img.px[i].red, 255);
    ck_assert_uint_eq(img.px[i].green, 255);
    ck_assert_uint_eq(img.px[i].blue, 255);
    ck_assert_uint_eq(img.px[i].alpha, 128);
  }

  // invert back to original
  filter_negative(&img, NULL);

  // checking the inverted image
  for (int i = 0; i < limit; i++) {
    ck_assert_uint_eq(img.px[i].red, 0);
    ck_assert_uint_eq(img.px[i].green, 0);
    ck_assert_uint_eq(img.px[i].blue, 0);
    ck_assert_uint_eq(img.px[i].alpha, 128);
  }
}
END_TEST

/* Check if the filter doesn't crash when we pass a 0x0 image */
START_TEST(negative_zero_size) {
  struct image img;

  img.size_x = 0;
  img.size_y = 0;
  img.px = NULL;

  filter_negative(&img, NULL); //. should not crash
}
END_TEST

/* Check for the simple, non-uniform, 3x3 test image that the blur filter
 * gives the correct output for different values of the radius (0, 1, 2, 3) */
START_TEST(blur_functionality) {
  struct pixel black = {0, 0, 0, 255};
  struct pixel white = {252, 252, 252, 255};
  struct pixel px[3][3] = {
      {black, black, black}, {black, white, black}, {black, black, black}};
  struct image img = {3, 3, &px};

  // --- --- --- BEGIN --- --- --- //

  struct image copy;
  int radius;
  

  // check for radius = 0
  copy = duplicate_img(img);
  radius = 0;
  filter_blur(&copy, &radius);

  for (int i = 0; i < 9; i++) {
    ck_assert_uint_eq(copy.px[i].red, img.px[i].red);
    ck_assert_uint_eq(copy.px[i].blue, img.px[i].blue);
    ck_assert_uint_eq(copy.px[i].green, img.px[i].green);
    ck_assert_uint_eq(copy.px[i].alpha, img.px[i].alpha);
    ck_assert_uint_eq(copy.px[i].alpha, 255);
  }

  // check for radius = 1
  copy = duplicate_img(img);
  radius = 1;
  filter_blur(&copy, &radius);

  // generate correct output
  struct pixel dark0 = {28, 28, 28, 255};
  struct pixel dark1 = {42, 42, 42, 255};
  struct pixel dark2 = {63, 63, 63, 255};
  struct pixel px_r1[3][3] = {
    {dark2, dark1, dark2}, {dark1, dark0, dark1}, {dark2, dark1, dark2}
  };
  struct image img_r1 = {3, 3, &px_r1};

  for (int i = 0; i < 9; i++) {
    ck_assert_uint_eq(copy.px[i].red, img_r1.px[i].red);
    ck_assert_uint_eq(copy.px[i].blue, img_r1.px[i].blue);
    ck_assert_uint_eq(copy.px[i].green, img_r1.px[i].green);
    ck_assert_uint_eq(copy.px[i].alpha, img_r1.px[i].alpha);
    ck_assert_uint_eq(copy.px[i].alpha, 255);
  }

  // check for radius = 2
  copy = duplicate_img(img);
  radius = 2;
  filter_blur(&copy, &radius);
  
  // generate correct output 
  struct pixel px_r2[3][3] = {
    {dark0, dark0, dark0}, {dark0, dark0, dark0}, {dark0, dark0, dark0}
  };
  struct image img_r2 = {3, 3, &px_r2};

  for (int i = 0; i < 9; i++) {
    ck_assert_uint_eq(copy.px[i].red, img_r2.px[i].red);
    ck_assert_uint_eq(copy.px[i].blue, img_r2.px[i].blue);
    ck_assert_uint_eq(copy.px[i].green, img_r2.px[i].green);
    ck_assert_uint_eq(copy.px[i].alpha, img_r2.px[i].alpha);
    ck_assert_uint_eq(copy.px[i].alpha, 255);
  }

  // check for radius = 3
  copy = duplicate_img(img);
  radius = 3;
  filter_blur(&copy, &radius);

  for (int i = 0; i < 9; i++) {
    ck_assert_uint_eq(copy.px[i].red, img_r2.px[i].red);
    ck_assert_uint_eq(copy.px[i].blue, img_r2.px[i].blue);
    ck_assert_uint_eq(copy.px[i].green, img_r2.px[i].green);
    ck_assert_uint_eq(copy.px[i].alpha, img_r2.px[i].alpha);
    ck_assert_uint_eq(copy.px[i].alpha, 255);
  }

}
END_TEST

/* Verify that the filter doesn't crash when we provide extreme values
 * for the radius (INT_MIN, INT_MAX, 0, image_width, image_height, all of the
 * previous values divided by 2, all of the previous values +- 1) */
struct image blur_radius_img;
int blur_radii[20];
START_TEST(blur_radius_edge_cases) {
  filter_blur(&blur_radius_img, &blur_radii[_i]);
}
END_TEST

/* Verify for a random image that the transparency filter works properly */
START_TEST(transparency_functionality) {
  
}
END_TEST

/* Check if the function crashes when we pass nullptr as the argument */
START_TEST(transparency_edge_case) {
  // todo: Implement */
}
END_TEST

int main() {
  Suite *s = suite_create("lib-Y0l0 tests");
  TCase *tc1 = tcase_create("edge case tests");
  suite_add_tcase(s, tc1);
  TCase *tc2 = tcase_create("basic functionality tests");
  suite_add_tcase(s, tc2);

  // todo: Add more tests here */
  /* Add tests for input limits, and general functionality tests */

  /* Tests for limits*/
  tcase_add_test(tc1, grayscale_double_limit);
  tcase_add_test(tc1, negative_zero_size);

  srand(time(NULL) ^ getpid());
  blur_radius_img = generate_rand_img();
  int tmp[20] = {
    INT_MIN, INT_MAX, 0, blur_radius_img.size_x, blur_radius_img.size_y,
    INT_MIN / 2, INT_MAX / 2, 0 / 2, blur_radius_img.size_x / 2, blur_radius_img.size_y / 2,
    INT_MIN + 1, INT_MAX + 1, 1, blur_radius_img.size_x + 1, blur_radius_img.size_y + 1,
    INT_MIN - 1, INT_MAX - 1, -1, blur_radius_img.size_x - 1, blur_radius_img.size_y - 1
  };
  memcpy(blur_radii, tmp, sizeof(blur_radii));
  tcase_add_loop_test(tc1, blur_radius_edge_cases, 0,
                      sizeof(blur_radii) / sizeof(blur_radii[0]));
  free(blur_radius_img.px);

  tcase_add_test(tc1, edge_threshold);
  tcase_add_test(tc1, transparency_edge_case);

  /* Tests for functionality */
  tcase_add_test(tc2, grayscale_functionality);
  
  tcase_add_loop_test(tc2, grayscale_examples, 0, 
                      sizeof(grayscale_sources) / sizeof(grayscale_sources[0])); //? will it result in 2

  tcase_add_test(tc2, negative_functionality);
  tcase_add_test(tc2, blur_functionality);
  tcase_add_test(tc2, transparency_functionality);
  tcase_add_loop_test(tc2, edge_example_image, 0,
                      sizeof(edge_deserts) / sizeof(edge_deserts[0]));
  tcase_add_test(tc2, edge_checkerboard);

  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_VERBOSE);

  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
