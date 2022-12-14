#ifndef DRAWING_FUNCS_H
#define DRAWING_FUNCS_H

#include <stdint.h>
#include "image.h"

struct Rect {
  int32_t x, y, width, height;
};

void draw_pixel(struct Image *img, int32_t x, int32_t y, uint32_t color);

void draw_rect(struct Image *img,
               const struct Rect *rect,
               uint32_t color);

void draw_circle(struct Image *img,
                 int32_t x, int32_t y, int32_t r,
                 uint32_t color);

void draw_tile(struct Image *img,
               int32_t x, int32_t y,
               struct Image *tilemap,
               const struct Rect *tile);

void draw_sprite(struct Image *img,
                 int32_t x, int32_t y,
                 struct Image *spritemap,
                 const struct Rect *sprite);

// Helper functions
uint32_t blur_colors(uint32_t foreground, uint32_t background);

uint32_t get_pix(struct Image *img, int32_t x, int32_t y);

void set_pix(struct Image *img, int32_t x, int32_t y, int32_t color);

void put_pixel(struct Image *img, int32_t x, int32_t y, uint32_t color);

int rec_in_bounds(struct Image *img,const struct Rect *tile);

int is_in_bounds(struct Image *img, int32_t x, int32_t y);

#endif // DRAWING_FUNCS_H
