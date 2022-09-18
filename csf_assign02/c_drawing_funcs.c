// C implementations of drawing functions (and helper functions)

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "drawing_funcs.h"

////////////////////////////////////////////////////////////////////////
// Helper functions
////////////////////////////////////////////////////////////////////////

/*
NOTE: if source bit is out of bounds for sprite or tile --> end
else just dont paint the pix

O2 optimization level


*/


// TODO: implement helper functions
uint32_t blur_colors(uint32_t foreground, uint32_t background) {
  uint32_t final_color;
  uint8_t f;
  uint8_t b;
  uint8_t a = (foreground | 255);
  final_color += a;
  
  for(int i = 1; i < 4; i++) {
    f = (foreground | (255 << 8*i));
    b = (background | (255 << 8*i));
    final_color += (((a*f + (255 - a)*b)/255) << 8*i);
  }

  return final_color;
}

int is_in_bounds(struct Image *img, int32_t x, int32_t y) {
  uint8_t rows = img->height;
  uint8_t cols = img->width;
}

uint32_t get_pix(struct Image *img, int32_t x, int32_t y) {
  if (is_in_bounds()) {
    uint8_t rows = img->height;
    uint8_t cols = img->width;
    uint8_t location = y*cols + x;
    return img->data[location]; 
  }
}

void set_pix(struct Image *img, int32_t x, int32_t y, int32_t color) {
  uint8_t rows = img->height;
  uint8_t cols = img->width;
  uint8_t location = y*cols + x;
  img->data[location] = color; 
}

////////////////////////////////////////////////////////////////////////
// API functions
////////////////////////////////////////////////////////////////////////

//
// Draw a pixel.
//
// Parameters:
//   img   - pointer to struct Image
//   x     - x coordinate (pixel column)
//   y     - y coordinate (pixel row)
//   color - uint32_t color value
//
void draw_pixel(struct Image *img, int32_t x, int32_t y, uint32_t color) {
  // TODO: implement
  uint32_t background = get_pix(img,x,y); //get pix --> locates pixel returns color (set will set color from x,y)
  background = blur_colors(color,background);
  set_pix(img,x,y,background);
}

//
// Draw a rectangle.
// The rectangle has rect->x,rect->y as its upper left corner,
// is rect->width pixels wide, and rect->height pixels high.
//
// Parameters:
//   img     - pointer to struct Image
//   rect    - pointer to struct Rect
//   color   - uint32_t color value
//
void draw_rect(struct Image *img,
               const struct Rect *rect,
               uint32_t color) {
  // TODO: implement
  uint32_t x = rect->x;
  uint32_t y = rect->y;
  uint32_t width = rect->width;
  uint32_t height = rect->height;
  for (int i = x; i<x+width; i++) {
    for (int j = y; j<y+height; j++) {
      draw_pixel(img,i,j,color);
    }
  }
}

//
// Draw a circle.
// The circle has x,y as its center and has r as its radius.
//
// Parameters:
//   img     - pointer to struct Image
//   x       - x coordinate of circle's center
//   y       - y coordinate of circle's center
//   r       - radius of circle
//   color   - uint32_t color value
//
void draw_circle(struct Image *img,
                 int32_t x, int32_t y, int32_t r,
                 uint32_t color) {
  uint32_t xcorner = x-r;
  uint32_t ycorner = y-r;
  for (int i = xcorner; i < xcorner + 2*r; i++) {
    for (int j = ycorner; j < ycorner + 2*r; j++) {
      int32_t dist = (x-i)*(x-i) + (y-j)*(y-j);
      if(dist <= r*r){
        draw_pixel(img,i,j,color);
      }
    }
  }
}

//
// Draw a tile by copying all pixels in the region
// enclosed by the tile parameter in the tilemap image
// to the specified x/y coordinates of the destination image.
// No blending of the tile pixel colors with the background
// colors should be done.
//
// Parameters:
//   img     - pointer to Image (dest image)
//   x       - x coordinate of location where tile should be copied
//   y       - y coordinate of location where tile should be copied
//   tilemap - pointer to Image (the tilemap)
//   tile    - pointer to Rect (the tile)
//
void draw_tile(struct Image *img,
               int32_t x, int32_t y,
               struct Image *tilemap,
               const struct Rect *tile) {
  uint32_t color;
  uint32_t width = tile->width;
  uint32_t height = tile->height;
  for (int i = x; i<x+width; i++) {
    for (int j = y; j<y+height; j++) {
      color = get_pix(tilemap,i-x,j-y); //shift the pix coords back to (0,0) to start
      draw_pixel(img,i,j,color); //map color to coord on main image
    }
  }
}

//
// Draw a sprite by copying all pixels in the region
// enclosed by the sprite parameter in the spritemap image
// to the specified x/y coordinates of the destination image.
// The alpha values of the sprite pixels should be used to
// blend the sprite pixel colors with the background
// pixel colors.
//
// Parameters:
//   img       - pointer to Image (dest image)
//   x         - x coordinate of location where sprite should be copied
//   y         - y coordinate of location where sprite should be copied
//   spritemap - pointer to Image (the spritemap)
//   sprite    - pointer to Rect (the sprite)
//
void draw_sprite(struct Image *img, int32_t x, int32_t y, struct Image *spritemap, const struct Rect *sprite) {
  uint32_t height = sprite->height;
  uint32_t width = sprite->width;
  uint32_t color;
  uint32_t background_color;
  uint32_t combo_color;

  for (int i = x; i < x+width; i++) {
    for (int j = y; j < y+height; j++) {
      color = get_pix(spritemap,i-x,j-y); // color of sprite
      background_color = get_pix(img,i-x,j-y);
      combo_color = blur_colors(color, background_color);
      draw_pixel(img,i,j,combo_color); // img is the destination 
    }
  }
}
