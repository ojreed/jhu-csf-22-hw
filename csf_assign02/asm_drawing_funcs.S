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
 *   %rdi     - pointer to struct Image
 *   %esi     - x coordinate (pixel column)
 *   %edx     - y coordinate (pixel row)
 *   %ecx     - uint32_t color value
 */
	.globl draw_pixel
draw_pixel:
	pushl %ebp // Stores stack frame of calling func, location of ebp is a reference pt for our stack Parameters
	movl %esp, %ebp // Taking current stack ptr and using it as frame for current func
	// Note: esp is the lower 32-bits of the stack ptr
	call is_in_bounds //if is_in_bounds(img,x,y), will access stack in this helper method
	cmp $1, %rax // is_in_bounds returns 1 or 0 
	jne .not_in_bounds //if value is not 1, jump to .not_in_bounds --> jne is ineqaulity
	//TODO: ADD func params to registers 
	//NEED: img, x, y --> rdi rsi rdx rcx r8 r9
	// img --> rdi
	// x --> rsi
	// y --> rxc
	// %rax is for return values (from funcs)
	call get_pix //uint32 background = get_pix(img,x,y)
	movl %rax, %r12d // r12 is a calle-saved register, we need this value to persist
	call blur_colors //background = blur_colors(color,background)
	movl %rbp, %r12d //rbp will be where we store the return val of blu_colors
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
	// img --> rdi
	// x --> rsi
	// TODO: probably change y to be stored in rdx...
	// y --> rxc
	// color --> rcx
	movl IMAGE_WIDTH_OFFSET, %r10d // set r10 to proper offset to access 
	leal %rdi, %r11d //store address of rdi in r11
	addl %r11d, %r10d // add address of image to r10 --> address of columns 
	imull (%r10), %rdx //r1o = cols * rdx = y --> y = product
	addl %rsi, %rdx // add rsi = x to rdx = y*cols --> x+y*cols == locataion (from data offset)

	movl IMAGE_DATA_OFFSET, %r10d // set r10 to proper offset to access data
	addl %r11d, %r10d // add address of image to r10 --> address of data
	addl %r10d, %rdx //image offset to data + location == pixel address
  	movl %rcx, (%rdx)// set img->data[location] equal to color
	ret

/*
is given an x,y and returns the correct color value at that location

Theoretically we need to pass in the image, x, y

*/
.get_pix
	// Note: esp is the lower 32-bits of the stack ptr
	// Note: let's use the stack method for params, the calling program will push stuff
	// onto the stack in reverse order 
	// img --> rdi
	// x --> rsi
	// y --> rdx
	//rax is return register 

	//part 1: compute location offset (y*img->cols + x)
	movl IMAGE_WIDTH_OFFSET, %r10d // set r10 to proper offset to access 
	leal %rdi, %r11d //store address of rdi in r11
	addl %r11d, %r10d // add address of image to r10 --> address of columns 
	//TODO ISSUE: r10 is an addres and below we want the value stored at addres
	imull (%r10d), %rdx //r1o = cols * rdx = y --> y = product
	addl %rsi, %rdx // add rsi = x to rdx = y*cols --> x+y*cols == locataion (from data offset)

	//part 2: access ofset location of img (img + offset)
	movl IMAGE_DATA_OFFSET, %r10d // set r10 to proper offset to access data
	addl %r11d, %r10d // add address of image to r10 --> address of data
	addl %r10d, %rdx //image offset to data + location == pixel address
  	movl (%rdx), %rax//return img->data[location]; 
	ret

/*
helper function passed an image, x, y 
returns true if that point is in the bounds of the function
	// img --> rdi
	// x --> rsi
	// y --> rdx
*/
.is_in_bounds:
	//if (0 <= x && x < cols && 0 <= y && y < rows) 
	
	//check x > 0
	cmp $0, %rsi // is 0<x
	jge .out_o_bounds // jump if 0>=x

	//check y>0
	cmp $0, %rdx // is 0<y
	jge .out_o_bounds // jump if 0>=y

	//TODO:: AGAIN
	//currently r10 = address of col value --> how to access val

	//get cols
	movl IMAGE_WIDTH_OFFSET, %r10d // set r10 to proper offset to access 
	leal %rdi, %r11d //store address of rdi in r11
	addl %r11d, %r10d // add address of image to r10 --> address of columns
	cmp %rsi, (%r10d) // is x < cols
	jge .out_o_bounds // jump if x>=cols


	//get rows
	movl IMAGE_HEIGHT_OFFSET, %r10d // set r10 to proper offset to access 
	leal %rdi, %r11d //store address of rdi in r11
	addl %r11d, %r10d // add address of image to r10 --> address of rows
	cmp %rdx, (%r10d) // is y < cols
	jge .out_o_bounds // jump if y>=cols

	//return 1 True --> in bounds
	movl $1, %rax //set return value to 1
	ret

.out_o_bounds:
	//return 0 False
	movl $0, %rax //set return value to 0
	ret

/*
passed forground and background color and 
returns blurring of those two colors
*/
.blur_colors:
	// uint32_t final_color; will store final_color in %rax, return register
	// rdi --> foreground
	// rsi --> background
	// uint32_t f; use register %r8
	// uint32_t b; use register %r9
	movl $255, %r10d
	and (%rdi), %r10d // uint8_t a = (foreground & 255); 
	movl $255, %rax // final_color = 255;

	// %r11 is the index counter
	movl $255, %rcx // %rcx = 255
	movl $1, %r11d // for(int i = 1; i < 4; i++) --> counter
	movl $8, %rdx // %rdx = 8
	movl %rdi, %r13d // %r13 is a
	and %rcx, %r13d // gets first 8 bits of r13 which is the alpha value!

	// loop starts here
	.top_loop:
	imul %r11d, %rdx // %rdx = (8*i)
	sal %rdx, %rcx //f = ((foreground & (255U << (8*i))) >> (8*i)); shift!
	movl %rdi, %r15d
	and %rcx, %r15d // value in rcx is 255 << (8*i)
	movl %rsi, %r14d // %r14 now stores temp background
	and %rcx, %r14d //b = ((background & (255U << (8*i))) >> (8*i));
	shr %rdx, %r14d
	shr %rdx, %r15d // >> (8*i), %r14 = b, %r15 = f
	imul %r13d, %r15d // %r15 = a*f
	movl $255, %r12d
	subl %13, %r12d // 255 - a
	imul %r14d, %r12d // %r12 = (255 - a)*b
	addl %r15d, %r12d // %r12 = (a*f + (255 - a)*b))
	divq $255, %r12d // %r12 = (a*f + (255 - a)*b)/255)
	sal %rdx, %r12d // %r12 ((a*f + (255 - a)*b)/255) << (8*i)
	addl %r12d, %rax // store the color channel in %rax, final_color += (((a*f + (255 - a)*b)/255) << (8*i));
	addl $1, %r11d // increment counter
	cmp %r11d, $4
	jle .top_loop 
  	//return final_color; value is already in %rax lol
	ret


/*
 * Draw a rectangle.
 * The rectangle has rect->x,rect->y as its upper left corner,
 * is rect->width pixels wide, and rect->height pixels high.
 *
 * Parameters:
 *   %rdi     - pointer to struct Image
 *   %rsi     - pointer to struct Rect
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
 *   %rdi     - pointer to struct Image
 *   %esi     - x coordinate of circle's center
 *   %edx     - y coordinate of circle's center
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
 * to the specified x/y coordinates of the destination image.
 * No blending of the tile pixel colors with the background
 * colors should be done.
 *
 * Parameters:
 *   %rdi - pointer to Image (dest image)
 *   %esi - x coordinate of location where tile should be copied
 *   %edx - y coordinate of location where tile should be copied
 *   %rcx - pointer to Image (the tilemap)
 *   %r8  - pointer to Rect (the tile)
 */
	.globl draw_tile
draw_tile:
	/* TODO: implement */
	ret

/*
 * Draw a sprite by copying all pixels in the region
 * enclosed by the sprite parameter in the spritemap image
 * to the specified x/y coordinates of the destination image.
 * The alpha values of the sprite pixels should be used to
 * blend the sprite pixel colors with the background
 * pixel colors.
 *
 * Parameters:
 *   %rdi - pointer to Image (dest image)
 *   %esi - x coordinate of location where sprite should be copied
 *   %edx - y coordinate of location where sprite should be copied
 *   %rcx - pointer to Image (the spritemap)
 *   %r8  - pointer to Rect (the sprite)
 */
	.globl draw_sprite
draw_sprite:
	/* TODO: implement */
	ret

/*
vim:ft=gas:
*/
