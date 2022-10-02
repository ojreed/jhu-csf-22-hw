// C implementations of drawing functions (and helper functions)

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "drawing_funcs.h"

////////////////////////////////////////////////////////////////////////
// Helper functions
////////////////////////////////////////////////////////////////////////

// Blend pixel colors together.
//
// Parameters:
//   foreground    - color of the pixel we are "adding" to the image
//   background    - color of the pixel already in the image
//
uint32_t blur_colors(uint32_t foreground, uint32_t background) {
  uint32_t final_color;
  uint32_t f;
  uint32_t b;
  uint8_t a = (foreground & 255);
  final_color = 255;

  // Compute the correct color value for new pixel (r, g, and b values)
  for(int i = 1; i < 4; i++) {
    // Isolate the part of uint32_t that represents the r value, then g...
    f = ((foreground & (255U << (8*i))) >> (8*i)); 
    b = ((background & (255U << (8*i))) >> (8*i));
    final_color += (((a*f + (255 - a)*b)/255) << (8*i));
  }
  return final_color;
}

// Check if the pixel at (x,y) is valid for the given image.
//
// Parameters:
//   img   - pointer to struct Image
//   x     - x coordinate (pixel column)
//   y     - y coordinate (pixel row)
//
int is_in_bounds(struct Image *img, int32_t x, int32_t y) {
  int32_t rows = img->height;
  int32_t cols = img->width;
  if (0 <= x && x < cols && 0 <= y && y < rows) {
    return 1;
  }
  return 0;
}

// Return pixel at specified location (x,y).
//
// Parameters:
//   img   - pointer to struct Image
//   x     - x coordinate (pixel column)
//   y     - y coordinate (pixel row)
//
uint32_t get_pix(struct Image *img, int32_t x, int32_t y) {
  // First need to compute location of pixel in data array
  int32_t cols = img->width;
  int32_t location = y*cols + x;
  return img->data[location]; 
}

// Modifies the color of pixel at specified location (x,y) and BLENDS/BLURS.
//
// Parameters:
//   img   - pointer to struct Image
//   x     - x coordinate (pixel column)
//   y     - y coordinate (pixel row)
//  color  - uint32_t color value
//
void set_pix(struct Image *img, int32_t x, int32_t y, int32_t color) {
  // Compute location of pixel in data array first
  int32_t cols = img->width;
  int32_t location = y*cols + x;

  // Set pixel equal to given color
  img->data[location] = color; 
}

// Modifies the color of pixel at specified location (x,y) but does NOT blend.
//
// Parameters:
//   img   - pointer to struct Image
//   x     - x coordinate (pixel column)
//   y     - y coordinate (pixel row)
//  color  - uint32_t color value
//
void put_pixel(struct Image *img, int32_t x, int32_t y, uint32_t color) {
  if (is_in_bounds(img,x,y)) {
    set_pix(img,x,y,(color|255));
  }
}

// Ensures the rectangle to be constructed is in the image bounds.
//
// Parameters:
//   img   - pointer to struct Image
//   tile  - pointer to struct Rect
//
int rec_in_bounds(struct Image *img,const struct Rect *tile ) {
  int32_t t_width = tile->width;
  int32_t t_height = tile->height;
  int32_t i_width = img->width;
  int32_t i_height = img->height;
  int32_t x = tile->x;
  int32_t y = tile->y;
  if (0<=x && t_width+x<=i_width && 0<=y && t_height+y<=i_height) {
    return 1;
  }
  return 0;
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
  if (is_in_bounds(img,x,y)) {
    // Retrieve pixel color, blur colors, and then set new color
    uint32_t background = get_pix(img,x,y); 
    background = blur_colors(color,background);
    set_pix(img,x,y,background);
  }
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
  int32_t x = rect->x;
  int32_t y = rect->y;
  int32_t width = rect->width;
  int32_t height = rect->height;
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
  int32_t xcorner = x-r;
  int32_t ycorner = y-r;
  for (int i = xcorner; i <= xcorner + 2*r; i++) {
    for (int j = ycorner; j <= ycorner + 2*r; j++) {
      // Check bounds for program safety
      if (is_in_bounds(img,i,j) == 1) {
        // Now check to make sure we are drawing in the circle
        int32_t dist = (x-i)*(x-i) + (y-j)*(y-j);
        if(dist <= r*r){
          draw_pixel(img,i,j,color);
        }
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
  int32_t width = tile->width;
  int32_t height = tile->height;
  if (rec_in_bounds(tilemap,tile) == 1) {
    for (int i = x; i<x+width; i++) {
      for (int j = y; j<y+height; j++) {
        // Shift the pix coordinates back to (0,0) to start
        color = get_pix(tilemap,i-x + tile->x,j-y + tile->y); 
        // Map color to coordinate on main image
        put_pixel(img,i,j,color); 
      }
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
  uint32_t color;
  int32_t width = sprite->width;
  int32_t height = sprite->height;
  if (rec_in_bounds(spritemap,sprite) == 1) {
    for (int i = x; i<x+width; i++) {
      for (int j = y; j<y+height; j++) {
        // Shift the pix coords back to (0,0) to start
        color = get_pix(spritemap,i-x + sprite->x,j-y + sprite->y); 
        // Map color to coord on main image
        draw_pixel(img,i,j,color); 
      }
    }
  }
}
