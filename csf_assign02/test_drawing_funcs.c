#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"
#include "drawing_funcs.h"
#include "tctest.h"
#include <inttypes.h>

// an expected color identified by a (non-zero) character code
typedef struct {
  char c;
  uint32_t color;
} ExpectedColor;

// struct representing a "picture" of an expected image
typedef struct {
  ExpectedColor colors[20];
  const char *pic;
} Picture;

typedef struct {
  struct Image small;
  struct Image large;
  struct Image tilemap;
  struct Image spritemap;
} TestObjs;

// dimensions and pixel index computation for "small" test image (objs->small)
#define SMALL_W        8
#define SMALL_H        6
#define SMALL_IDX(x,y) ((y)*SMALL_W + (x))

// dimensions of the "large" test image
#define LARGE_W        24
#define LARGE_H        20

// create test fixture data
TestObjs *setup(void) {
  TestObjs *objs = (TestObjs *) malloc(sizeof(TestObjs));
  init_image(&objs->small, SMALL_W, SMALL_H);
  init_image(&objs->large, LARGE_W, LARGE_H);
  objs->tilemap.data = NULL;
  objs->spritemap.data = NULL;
  return objs;
}

// clean up test fixture data
void cleanup(TestObjs *objs) {
  free(objs->small.data);
  free(objs->large.data);
  free(objs->tilemap.data);
  free(objs->spritemap.data);

  free(objs);
}

uint32_t lookup_color(char c, const ExpectedColor *colors) {
  for (unsigned i = 0; ; i++) {
    assert(colors[i].c != 0);
    if (colors[i].c == c) {
      return colors[i].color;
    }
  }
}

void check_picture(struct Image *img, Picture *p) {
  unsigned num_pixels = img->width * img->height;
  assert(strlen(p->pic) == num_pixels);

  for (unsigned i = 0; i < num_pixels; i++) {
    char c = p->pic[i];
    uint32_t expected_color = lookup_color(c, p->colors);
    uint32_t actual_color = img->data[i];
    ASSERT(actual_color == expected_color);
  }
}

// prototypes of test functions
void test_draw_pixel(TestObjs *objs);
void test_draw_rect(TestObjs *objs);
void test_draw_circle(TestObjs *objs);
void test_draw_circle_clip(TestObjs *objs);
void test_draw_tile(TestObjs *objs);
void test_draw_sprite(TestObjs *objs);
void test_blur_colors(TestObjs *objs);
void test_get_pix(TestObjs *objs);
void test_set_pix(TestObjs *objs);
void test_is_in_bounds(TestObjs *objs);
void test_bounds_edge(TestObjs *objs);
void test_blur_colors_math(TestObjs *objs);
void test_rec_in_bounds(TestObjs *objs);
void test_put_pixel(TestObjs *objs);

int main(int argc, char **argv) {
  if (argc > 1) {
    // user specified a specific test function to run
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  // TODO: add TEST() directives for your helper functions
  TEST(test_draw_pixel);
  TEST(test_draw_rect);
  TEST(test_draw_circle);
  TEST(test_draw_circle_clip);
  TEST(test_draw_tile);
  TEST(test_draw_sprite);
  TEST(test_blur_colors);
  TEST(test_get_pix);
  TEST(test_set_pix);
  TEST(test_is_in_bounds);
  TEST(test_bounds_edge);
  TEST(test_blur_colors_math);
  TEST(test_rec_in_bounds);
  TEST(test_put_pixel);

  TEST_FINI();
}

void test_draw_pixel(TestObjs *objs) {
  // initially objs->small pixels are opaque black
  ASSERT(objs->small.data[SMALL_IDX(3, 2)] == 0x000000FFU);
  ASSERT(objs->small.data[SMALL_IDX(5, 4)] == 0x000000FFU);
  // printf("INSIDE TEST DRAW");
  // test drawing completely opaque pixels
  draw_pixel(&objs->small, 3, 2, 0xFF0000FF); // opaque red
  ASSERT(objs->small.data[SMALL_IDX(3, 2)] == 0xFF0000FF);
  draw_pixel(&objs->small, 5, 4, 0x800080FF); // opaque magenta (half-intensity)
  ASSERT(objs->small.data[SMALL_IDX(5, 4)] == 0x800080FF);

  // test color blending
  draw_pixel(&objs->small, 3, 2, 0x00FF0080); // half-opaque full-intensity green
  ASSERT(objs->small.data[SMALL_IDX(3, 2)] == 0x7F8000FF);
  draw_pixel(&objs->small, 4, 2, 0x0000FF40); // 1/4-opaque full-intensity blue
  ASSERT(objs->small.data[SMALL_IDX(4, 2)] == 0x000040FF);
}

void test_draw_rect(TestObjs *objs) {
  struct Rect red_rect = { .x = 2, .y = 2, .width=3, .height=3 };
  struct Rect blue_rect = { .x = 3, .y = 3, .width=3, .height=3 };
  draw_rect(&objs->small, &red_rect, 0xFF0000FF); // red is full-intensity, full opacity
  draw_rect(&objs->small, &blue_rect, 0x0000FF80); // blue is full-intensity, half opacity

  const uint32_t red   = 0xFF0000FF; // expected full red color
  const uint32_t blue  = 0x000080FF; // expected full blue color
  const uint32_t blend = 0x7F0080FF; // expected red/blue blend color
  const uint32_t black = 0x000000FF; // expected black (background) color

  Picture expected = {
    { {'r', red}, {'b', blue}, {'n', blend}, {' ', black} },
    "        "
    "        "
    "  rrr   "
    "  rnnb  "
    "  rnnb  "
    "   bbb  "
  };

  check_picture(&objs->small, &expected);
}

void test_draw_circle(TestObjs *objs) {
  Picture expected = {
    { {' ', 0x000000FF}, {'x', 0x00FF00FF} },
    "   x    "
    "  xxx   "
    " xxxxx  "
    "  xxx   "
    "   x    "
    "        "
  };

  draw_circle(&objs->small, 3, 2, 2, 0x00FF00FF);
  check_picture(&objs->small, &expected);
}

void test_draw_circle_clip(TestObjs *objs) {
  Picture expected = {
    { {' ', 0x000000FF}, {'x', 0x00FF00FF} },
    " xxxxxxx"
    " xxxxxxx"
    "xxxxxxxx"
    " xxxxxxx"
    " xxxxxxx"
    "  xxxxx "
  };

  draw_circle(&objs->small, 4, 2, 4, 0x00FF00FF);

  check_picture(&objs->small, &expected);
}

void test_draw_tile(TestObjs *objs) {
  ASSERT(read_image("img/PrtMimi.png", &objs->tilemap) == IMG_SUCCESS);

  struct Rect r = { .x = 4, .y = 2, .width = 16, .height = 18 };
  draw_rect(&objs->large, &r, 0x1020D0FF);

  struct Rect grass = { .x = 0, .y = 16, .width = 16, .height = 16 };
  draw_tile(&objs->large, 3, 2, &objs->tilemap, &grass);

  Picture pic = {
    {
      { ' ', 0x000000ff },
      { 'a', 0x52ad52ff },
      { 'b', 0x1020d0ff },
      { 'c', 0x257b4aff },
      { 'd', 0x0c523aff },
    },
    "                        "
    "                        "
    "             a     b    "
    "            a      b    "
    "            a     ab    "
    "           ac      b    "
    "           ac a    b    "
    "      a a  ad  a   b    "
    "     a  a aad  aa ab    "
    "     a  a acd aaacab    "
    "    aa  cdacdaddaadb    "
    "     aa cdaddaaddadb    "
    "     da ccaddcaddadb    "
    "    adcaacdaddddcadb    "
    "   aaccacadcaddccaab    "
    "   aacdacddcaadcaaab    "
    "   aaaddddaddaccaacb    "
    "   aaacddcaadacaaadb    "
    "    bbbbbbbbbbbbbbbb    "
    "    bbbbbbbbbbbbbbbb    "
  };

  check_picture(&objs->large, &pic);
}

void test_draw_sprite(TestObjs *objs) {
  ASSERT(read_image("img/NpcGuest.png", &objs->spritemap) == IMG_SUCCESS);

  struct Rect r = { .x = 1, .y = 1, .width = 14, .height = 14 };
  draw_rect(&objs->large, &r, 0x800080FF);

  struct Rect sue = { .x = 128, .y = 136, .width = 16, .height = 15 };
  draw_sprite(&objs->large, 4, 2, &objs->spritemap, &sue);

  Picture pic = {
    {
      { ' ', 0x000000ff },
      { 'a', 0x800080ff },
      { 'b', 0x9cadc1ff },
      { 'c', 0xefeae2ff },
      { 'd', 0x100000ff },
      { 'e', 0x264c80ff },
      { 'f', 0x314e90ff },
    },
    "                        "
    " aaaaaaaaaaaaaa         "
    " aaaaaaaaaaaaaa         "
    " aaaaaaaaaaaaaa         "
    " aaaaaaabccccccbc       "
    " aaaaacccccccccccc      "
    " aaaacbddcccddcbccc     "
    " aaacbbbeccccedbccc     "
    " aaacbbceccccebbbccc    "
    " aaabbbccccccccbbccc    "
    " aaaabbbcccccccb ccb    "
    " aaaabaaaaabbaa  cb     "
    " aaaaaaaaafffea         "
    " aaaaaaaaaffeea         "
    " aaaaaaacffffcc         "
    "        cffffccb        "
    "         bbbbbbb        "
    "                        "
    "                        "
    "                        "
  };

  check_picture(&objs->large, &pic);
}

void test_blur_colors(TestObjs *objs) {
  uint32_t computed_color;
  uint32_t background;
  uint32_t foreground;

  background = 0x000000FF;
  foreground = 0xFF0000FF;
  computed_color = blur_colors(foreground, background);
  ASSERT(computed_color==0xFF0000FF);

  background = 0x000000FF;
  foreground = 0xFF0000A5;
  computed_color = blur_colors(foreground, background);
  ASSERT(computed_color==0xA50000FF);

  background = 0x00000000;
  foreground = 0x00000000;
  computed_color = blur_colors(foreground, background);
  ASSERT(computed_color==0x000000FF);
}

void test_blur_colors_math(TestObjs *objs) {
  uint32_t computed_color;
  uint32_t background;
  uint32_t foreground;

  background = 0x0000000C;
  foreground = 0x00FF0000; 
  uint32_t f;
  uint32_t b;
  uint8_t a = (foreground & 255);
  uint32_t final_color = 255;

  for(int i = 1; i < 4; i++) {
    f = ((foreground & (255U << (8*i))) >> (8*i)); 
    b = ((background & (255U << (8*i))) >> (8*i));
    final_color += (((a*f + (255 - a)*b)/255) << (8*i));
  }
  computed_color = blur_colors(foreground, background);
  ASSERT(computed_color==final_color);

  a = 10; // alpha
  background  = 255;
  foreground = 255;
  uint32_t blended = (a*foreground + (255 - a)*background)/255;
  ASSERT(blur_colors(foreground, background) == blended);
}

void test_get_pix(TestObjs *objs) {
  struct Rect red_rect = { .x = 2, .y = 2, .width=3, .height=3 };
  struct Rect blue_rect = { .x = 3, .y = 3, .width=3, .height=3 };
  draw_rect(&objs->small, &red_rect, 0xFF0000FF); // red is full-intensity, full opacity
  draw_rect(&objs->small, &blue_rect, 0x0000FF80); // blue is full-intensity, half opacity

  const uint32_t red   = 0xFF0000FF; // expected full red color
  const uint32_t blue  = 0x000080FF; // expected full blue color
  const uint32_t blend = 0x7F0080FF; // expected red/blue blend color
  const uint32_t black = 0x000000FF; // expected black (background) color

  Picture expected = {
    { {'r', red}, {'b', blue}, {'n', blend}, {' ', black} },
    "        "
    "        "
    "  rrr   "
    "  rnnb  "
    "  rnnb  "
    "   bbb  "
  };

  struct Image *img1 = &objs->small;
  char c = expected.pic[0];
  uint32_t expected_color = lookup_color(c, expected.colors);
  uint32_t actual_color = img1->data[0];
  ASSERT(actual_color == expected_color);
  ASSERT(expected_color == get_pix(img1, 0, 0));
}

void test_set_pix(TestObjs *objs) {
  draw_pixel(&objs->small, 3, 2, 0xFF0000FF); 
  set_pix(&objs->small, 3, 2, 0x0000FF40); 
  ASSERT(objs->small.data[SMALL_IDX(3, 2)] != 0xFF0000FF); // Checks blending is happening
}

void test_put_pixel(TestObjs *objs) { // doesn't blur
  ASSERT(objs->small.data[SMALL_IDX(3, 2)] == 0x000000FFU);
  put_pixel(&objs->small, 3, 2, 0xFF0000FF); 
  ASSERT(objs->small.data[SMALL_IDX(3, 2)] == 0xFF0000FF);
}

void test_is_in_bounds(TestObjs *objs) {
  int result;
  draw_circle(&objs->small, 3, 2, 2, 0x00FF00FF);
  result = is_in_bounds(&objs->small, 10, 10);
  ASSERT(0 == result); // Out of bounds
  result = is_in_bounds(&objs->small, 2, 2);
  ASSERT(1 == result);
}

void test_bounds_edge(TestObjs *objs) {
  int result;
  draw_circle(&objs->small, 3, 2, 2, 0x00FF00FF);
  result = is_in_bounds(&objs->small, 500, 10000);
  ASSERT(0 == result); // Out of bounds
  result = is_in_bounds(&objs->small, -6, -4);
  ASSERT(0 == result);
  result = is_in_bounds(&objs->small, -6.55555555555555, 4);
  ASSERT(0 == result);

  draw_pixel(&objs->small, -5, -4.7777777777777, 0xFF0000FF); // maybe this will segfault
  draw_pixel(&objs->small, 10000, -2, 0xFF0000FF);
}

void test_rec_in_bounds(TestObjs *objs) {
  int result;
  ASSERT(read_image("img/PrtMimi.png", &objs->tilemap) == IMG_SUCCESS);
  struct Rect r = { .x = 4, .y = 2, .width = 16, .height = 18 };
  draw_rect(&objs->large, &r, 0x1020D0FF);
  struct Rect grass = { .x = 0, .y = 16, .width = 16, .height = 16 };
  result = rec_in_bounds(&objs->tilemap, &grass);
  ASSERT(result == 1);

  struct Rect grass2 = { .x = 0, .y = 16, .width = 200, .height = 200 };
  result = rec_in_bounds(&objs->tilemap, &grass2);
  ASSERT(result == 0);
}



