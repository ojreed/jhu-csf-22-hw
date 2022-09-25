/*
 * Assembly-language implementations of the drawing functions
 */

/* Offsets of struct Image fields */
#define IMAGE_WIDTH_OFFSET   0
#define IMAGE_HEIGHT_OFFSET  4
#define IMAGE_DATA_OFFSET    8

/* Offsets of struct Rect fields */
#define RECT_X_OFFSET        0
#define RECT_Y_OFFSET        4
#define RECT_WIDTH_OFFSET    8
#define RECT_HEIGHT_OFFSET   12

	.section .text

/***********************************************************************
   Helper functions
   You should declare these as .globl so that
   they can be called from the unit test code.
 ***********************************************************************/

/* TODO: implement your helper functions here */

/***********************************************************************
   Public API functions
 ***********************************************************************/

/*
 * Draw a pixel.
 *
 * Parameters:
 *   %edi     - pointer to struct Image
 *   %esi     - x cooedinate (pixel column)
 *   %edx     - y cooedinate (pixel row)
 *   %ecx     - uint32_t color value
 */
	.globl draw_pixel
draw_pixel:
	pushq %rbp // save pointer to string locally, implicitly align stack by subtracting 8 bytes
	movq %rsp, %rbp //save value originally in %rbp
	// Note: esp is the lower 32-bits of the stack ptr
	call is_in_bounds //if is_in_bounds(img,x,y), will access stack in this helper method
	cmp $1, %eax // is_in_bounds returns 1 or 0 
	jne .not_in_bounds //if value is not 1, jump to .not_in_bounds --> jne is ineqaulity
	//TODO: ADD func params to registers 
	//NEED: img, x, y --> edi esi edx ecx r8 r9
	// img --> edi
	// x --> esi
	// y --> rxc
	// %eax is for return values (from funcs)
	call get_pix //uint32 background = get_pix(img,x,y)
	movl %eax, %r12d // r12 is a calle-saved register, we need this value to peesist
	call blur_colors //background = blur_colors(color,background)
	movl %r12d, %eax //eax will be where we store the return val of blu_colors
	call set_pix //set_pix(img,x,y,background);
	ret
.not_in_bounds:
	ret //stores the return address to jump back to on the stack



/*
Takes input of an image and a Rect and checks that 
all corners of the rect are inbounds with the image
*/
.rec_in_bounds:
	ret


/*
draws a pixel on the passed image at x,y of given color
does not use blur
passed image, x,y, color
confirms that x,y are valid 
*/
.put_pixel:
	ret


/*
called by put and draw pix to actually set the value of a given pixel
passed image, x,y, color
*/
.set_pix:
	// img --> edi
	// x --> esi
	// TODO: probably change y to be stored in edx...
	// y --> rxc
	// color --> ecx
	movl IMAGE_WIDTH_OFFSET, %r10d // set r10 to proper offset to access 
	leaq %edi, %r11d //store address of edi in r11
	addl %r11d, %r10d // add address of image to r10 --> address of columns 
	imull (%r10), %edx //r1o = cols * edx = y --> y = product
	addl %esi, %edx // add esi = x to edx = y*cols --> x+y*cols == locataion (from data offset)

	movl IMAGE_DATA_OFFSET, %r10d // set r10 to proper offset to access data
	addl %r11d, %r10d // add address of image to r10 --> address of data
	addl %r10d, %edx //image offset to data + location == pixel address
  	movl %ecx, (%edx)// set img->data[location] equal to color
	ret

/*
is given an x,y and returns the correct color value at that location

Theoretically we need to pass in the image, x, y

*/
.get_pix
	// Note: esp is the lower 32-bits of the stack ptr
	// Note: let's use the stack method for params, the calling program will push stuff
	// onto the stack in reverse order 
	// img --> edi
	// x --> esi
	// y --> edx
	//eax is return register 

	//part 1: compute location offset (y*img->cols + x)
	movl IMAGE_WIDTH_OFFSET, %r10d // set r10 to proper offset to access 
	leaq %edi, %r11d //store address of edi in r11
	addl %r11d, %r10d // add address of image to r10 --> address of columns 
	//TODO ISSUE: r10 is an addres and below we want the value stored at addres
	imull (%r10d), %edx //r1o = cols * edx = y --> y = product
	addl %esi, %edx // add esi = x to edx = y*cols --> x+y*cols == locataion (from data offset)

	//part 2: access ofset location of img (img + offset)
	movl IMAGE_DATA_OFFSET, %r10d // set r10 to proper offset to access data
	addl %r11d, %r10d // add address of image to r10 --> address of data
	addl %r10d, %edx //image offset to data + location == pixel address
  	movl (%edx), %eax//return img->data[location]; 
	ret

/*
helper function passed an image, x, y 
returns true if that point is in the bounds of the function
	// img --> edi
	// x --> esi
	// y --> edx
*/
.is_in_bounds:
	//if (0 <= x && x < cols && 0 <= y && y < rows) 
	
	//check x > 0
	cmp $0, %esi // is 0<x
	jge .out_o_bounds // jump if 0>=x

	//check y>0
	cmp $0, %edx // is 0<y
	jge .out_o_bounds // jump if 0>=y

	//TODO:: AGAIN
	//currently r10 = address of col value --> how to access val

	//get cols
	movl IMAGE_WIDTH_OFFSET, %r10d // set r10 to proper offset to access 
	leaq %edi, %r11d //store address of edi in r11
	addl %r11d, %r10d // add address of image to r10 --> address of columns
	cmp %esi, (%r10d) // is x < cols
	jge .out_o_bounds // jump if x>=cols


	//get rows
	movl IMAGE_HEIGHT_OFFSET, %r10d // set r10 to proper offset to access 
	leaq %edi, %r11d //store address of edi in r11
	addl %r11d, %r10d // add address of image to r10 --> address of rows
	cmp %edx, (%r10d) // is y < cols
	jge .out_o_bounds // jump if y>=cols

	//return 1 True --> in bounds
	movl $1, %eax //set return value to 1
	ret

.out_o_bounds:
	//return 0 False
	movl $0, %eax //set return value to 0
	ret

/*
passed forground and background color and 
returns blurring of those two colors
*/
.blur_colors:
	// uint32_t final_color; will store final_color in %eax, return register
	// edi --> foreground
	// esi --> background
	// uint32_t f; use register %r8
	// uint32_t b; use register %r9
	movl $255, %r10d
	and (%edi), %r10d // uint8_t a = (foreground & 255); 
	movl $255, %eax // final_color = 255;

	// %r11 is the index counter
	movl $255, %ecx // %ecx = 255
	movl $1, %r11d // for(int i = 1; i < 4; i++) --> counter
	movl $8, %edx // %edx = 8
	movl %edi, %r13d // %r13 is a
	and %ecx, %r13d // gets first 8 bits of r13 which is the alpha value!

	// loop starts here
	.top_loop:
	imul %r11d, %edx // %edx = (8*i)
	sal %edx, %ecx //f = ((foreground & (255U << (8*i))) >> (8*i)); shift!
	movl %edi, %r15d
	and %ecx, %r15d // value in ecx is 255 << (8*i)
	movl %esi, %r14d // %r14 now stores temp background
	and %ecx, %r14d //b = ((background & (255U << (8*i))) >> (8*i));
	shr %edx, %r14d
	shr %edx, %r15d // >> (8*i), %r14 = b, %r15 = f
	imul %r13d, %r15d // %r15 = a*f
	movl $255, %r12d
	subl %13, %r12d // 255 - a
	imul %r14d, %r12d // %r12 = (255 - a)*b
	addl %r15d, %r12d // %r12 = (a*f + (255 - a)*b))
	divq $255, %r12d // %r12 = (a*f + (255 - a)*b)/255)
	sal %edx, %r12d // %r12 ((a*f + (255 - a)*b)/255) << (8*i)
	addl %r12d, %eax // store the color channel in %eax, final_color += (((a*f + (255 - a)*b)/255) << (8*i));
	addl $1, %r11d // increment counter
	cmp %r11d, $4
	jle .top_loop 
  	//return final_color; value is already in %eax lol
	ret


/*
 * Draw a rectangle.
 * The rectangle has rect->x,rect->y as its upper left corner,
 * is rect->width pixels wide, and rect->height pixels high.
 *
 * Parameters:
 *   %edi     - pointer to struct Image
 *   %esi     - pointer to struct Rect
 *   %edx     - uint32_t color value
 */
	.globl draw_rect
draw_rect:
	/* TODO: implement */
	ret

/*
 * Draw a circle.
 * The circle has x,y as its center and has r as its radius.
 *
 * Parameters:
 *   %edi     - pointer to struct Image
 *   %esi     - x cooedinate of circle's center
 *   %edx     - y cooedinate of circle's center
 *   %ecx     - radius of circle
 *   %r8d     - uint32_t color value
 */
	.globl draw_circle
draw_circle:
	/* TODO: implement */
	ret

/*
 * Draw a tile by copying all pixels in the region
 * enclosed by the tile parameter in the tilemap image
 * to the specified x/y cooedinates of the destination image.
 * No blending of the tile pixel colors with the background
 * colors should be done.
 *
 * Parameters:
 *   %edi - pointer to Image (dest image)
 *   %esi - x cooedinate of location where tile should be copied
 *   %edx - y cooedinate of location where tile should be copied
 *   %ecx - pointer to Image (the tilemap)
 *   %r8  - pointer to Rect (the tile)
 */
	.globl draw_tile
draw_tile:
	/* TODO: implement */
	ret

/*
 * Draw a sprite by copying all pixels in the region
 * enclosed by the sprite parameter in the spritemap image
 * to the specified x/y cooedinates of the destination image.
 * The alpha values of the sprite pixels should be used to
 * blend the sprite pixel colors with the background
 * pixel colors.
 *
 * Parameters:
 *   %edi - pointer to Image (dest image)
 *   %esi - x cooedinate of location where sprite should be copied
 *   %edx - y cooedinate of location where sprite should be copied
 *   %ecx - pointer to Image (the spritemap)
 *   %r8  - pointer to Rect (the sprite)
 */
	.globl draw_sprite
draw_sprite:
	/* TODO: implement */
	ret

/*
vim:ft=gas:
*/
