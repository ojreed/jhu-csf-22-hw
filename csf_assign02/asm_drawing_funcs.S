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
	pushq %rbp // Stores stack frame of calling func, location of ebp is a reference pt for our stack Parameters
	movq %rsp, %rbp // Taking current stack ptr and using it as frame for current func
	pushq %rdi //store image
	pushq %rsi //store x
	pushq %rdx //store y
	pushq %rcx //store color
	// Note: esp is the lower 32-bits of the stack ptr
	movl $4, %r10
	leaq (%rbp,%r10,8), %rdi //set img
	leaq 3(%rbp), %rsi //set x
	leaq 2(%rbp) %rdx //set y
	call is_in_bounds //if is_in_bounds(img,x,y), will access stack in this helper method
	cmp $1, %eax // is_in_bounds returns 1 or 0 
	jne not_in_bounds //if value is not 1, jump to .not_in_bounds --> jne is ineqaulity
	//TODO: ADD func params to registers 
	//NEED: img, x, y --> edi esi edx ecx r8d r9d
	// img --> edi
	// x --> esi
	// y --> rxc
	// col --> ecx
	// %eax is for return values (from funcs)
	leaq -256(%rbp), %rdi //set img
	leaq -192(%rbp), %rsi //set x
	leaq -128(%rbp), %rdx //set y
	call get_pix //uint32 background = get_pix(img,x,y)
	movl %eax, %r12d // r12 is a calle-saved register, we need this value to peesist
	movl %r12d, %esi //set b
	leaq -64(%rbp), %rdi //set f
	call blur_colors //background = blur_colors(color,background)
	movl %eax, %ecx //rbp will be where we store the return val of blur_colors
	leaq -256(%rbp), %rdi //set img
	leaq -192(%rbp), %rsi //set x
	leaq -128(%rbp), %rdx //set y
	call set_pix //set_pix(img,x,y,background);
	popq %rcx //remove color
	popq %rdx //remove y
	popq %rsi //remove x
	popq %rdi //remove image
	popq %rbp //fix stack
	ret
not_in_bounds:
	popq %rcx //remove color
	popq %rdx //remove y
	popq %rsi //remove x
	popq %rdi //remove image
	popq %rbp //fix stack
	ret



/*
Takes input of an image and a Rect and checks that 
all corners of the rect are inbounds with the image
*/
	.globl rec_in_bounds
rec_in_bounds:
	ret


/*
draws a pixel on the passed image at x,y of given color
does not use blur
passed image, x,y, color
confirms that x,y are valid 
*/
	.globl put_pixel
put_pixel:
	ret


/*
called by put and draw pix to actually set the value of a given pixel
passed image, x,y, color
*/

	.globl set_pix
set_pix:
	subq $8, %rsp // re-align stack
	// img --> rdi
	// x --> esi
	// TODO: probably change y to be stored in edx...
	// y --> rxc
	// color --> ecx
	movl $0, %r10d // set r10 to proper offset to access 
	leal (%rdi), %r11d //store address of edi in r11
	addl %r11d, %r10d // add address of image to r10 --> address of columns 
	movl %edx, %eax
	mull (%r10d) //r1o = cols * edx = y --> y = product
	movl %eax, %edx
	addl %esi, %edx // add esi = x to edx = y*cols --> x+y*cols == locataion (from data offset)
	movl $8, %r10d // set r10 to proper offset to access data
	addl %r11d, %r10d // add address of image to r10 --> address of data
	addl %r10d, %edx //image offset to data + location == pixel address
  	movl %ecx, (%edx)// set img->data[location] equal to color
	addq $8, %rsp // restore ptr
	ret

/*
is given an x,y and returns the correct color value at that location

Theoretically we need to pass in the image, x, y

*/
	.globl get_pix
get_pix:
	pushq %rbp // Stores stack frame of calling func, location of ebp is a reference pt for our stack Parameters
	movq %rsp, %rbp // Taking current stack ptr and using it as frame for current func
	//subq $8, %rsp
	// Note: esp is the lower 32-bits of the stack ptr
	// Note: let's use the stack method for params, the calling program will push stuff
	// onto the stack in reverse order 
	// img --> edi
	// x --> esi
	// y --> edx
	//eax is return register 

	//part 1: compute location offset (y*img->cols + x)
	movq $0, %r10 // set r10 to proper offset to access 
	leaq (%rdi), %r11 //store address of edi in r11
	addq %r11, %r10 // add address of image to r10 --> address of columns 
	movl %edx, %eax
	mull (%r10d) //r10d = cols * edx = y --> y = product
	movl %eax, %edx
	addl %esi, %edx // add esi = x to edx = y*cols --> x+y*cols == locataion (from data offset)

	//part 2: access ofset location of img (img + offset)
	movl $8, %r10d // set r10 to proper offset to access data
	addl %r11d, %r10d // add address of image to r10 --> address of data
	addl %r10d, %edx //image offset to data + location == pixel address
  	movl (%edx), %eax//return img->data[location]; 
	//addq $8, %rsp // restore ptr
	popq %rbp 
	ret

/*
helper function passed an image, x, y 
returns true if that point is in the bounds of the function
	// img --> edi
	// x --> esi
	// y --> edx
*/
	.globl is_in_bounds
is_in_bounds:
	subq $8, %rsp
	//if (0 <= x && x < cols && 0 <= y && y < rows) 
	
	//check x > 0
	cmp $0, %esi // is 0<x
	jle out_o_bounds // jump if 0>=x

	//check y>0
	cmp $0, %edx // is 0<y
	jle out_o_bounds // jump if 0>=y

	//TODO:: AGAIN
	//currently r10 = address of col value --> how to access val

	//get cols
	//movl $0, %r10d // set r10 to proper offset to access 
	//leal (%rdi), %r11d //store address of edi in r11
	//addl %r11d, %r10d // add address of image to r10 --> address of columns
	cmp %esi, (%rdi) // is x < cols
	jle out_o_bounds // jump if x>=cols


	//get rows
	//movl $4, %r10d // set r10 to proper offset to access 
	//leal (%rdi), %r11d //store address of edi in r11
	//addl %r11d, %r10d // add address of image to r10 --> address of rows
	cmp %edx, (%rdi) // is y < cols
	jle out_o_bounds // jump if y>=cols

	//return 1 True --> in bounds
	movl $1, %eax //set return value to 1
	addq $8, %rsp // restore ptr
	ret
out_o_bounds:
	//return 0 False
	movl $0, %eax //set return value to 0
	addq $8, %rsp // restore ptr
	ret

/*
passed forground and background color and 
returns blurring of those two colors
*/
	.globl blur_colors
blur_colors:
	pushq %rbp // Stores stack frame of calling func, location of ebp is a reference pt for our stack Parameters
	movq %rsp, %rbp // Taking current stack ptr and using it as frame for current func
	//subq $8, %rsp
	// uint32_t final_color; will store final_color in %eax, return register
	// edi --> foreground
	// esi --> background
	// uint32_t f; use register %r8
	// uint32_t b; use register %r9
	movl $255, %r10d
	andl %edi, %r10d // uint8_t a = (foreground & 255); 
	movl $255, %eax // final_color = 255;


	/*
		setup vars
		edi - forground
		esi - background
		ecx - 8 consecutive 1 bits
		r11d - for loop counter
		edx - ecx shifter
		r13d - alpha
	*/
	// %r11 is the index counter
	movl $255, %ecx // %ecx = 255
	movl $1, %r11d // for(int i = 1; i < 4; i++) --> counter 
	movl $8, %edx // %edx = 8
	movl %edi, %r13d // %r13 is alpha
	andl %ecx, %r13d // gets first 8 bits of r13 which is the alpha value!

	// loop starts here
top_loop:

	pushq %rax//store final in stack
	movl $255, %ecx // %ecx = 255
	movl $8, %edx // %edx = 8
	//multiply 8 by i (8 on first iteration, 16 on 2nd)
	movl %edx, %eax 
	mull %r11d // %edx = (8*i)
	movl %eax, %edx
	//shift 8 ones into correct position
	movl %ecx, %r15d //move 8, ones out of cx register into r15 for shift
	//dl is 1 byte version of edx = total shift --> moved to cl to be used in shift
	movb %dl, %cl// move value to shift by into cl
	sall %cl, %r15d //f = ((foreground & (255U << (8*i))) >> (8*i)); shift!
	movl %r15d, %ecx //move result 8 ones shifted back into cx
	//and 8 ones with f - edi
	movl %edi, %r15d //move forground r15d for anding
	andl %ecx, %r15d // value in ecx is 255 << (8*i)
	//r15d stores anded f
	//and 8 ones with b - esi
	movl %esi, %r14d // %r14 now stores temp background
	andl %ecx, %r14d //b = ((background & (255U << (8*i))) >> (8*i));
	//r14d stores anded b
	movb %dl, %cl //dl is 1 byte version of edx = total shift --> moved to cl to be used in shift
	shrl %cl, %r14d // >> (8*i), %r14 = b
	shrl %cl, %r15d // >> (8*i), %r15 = f
	//multiply f * alpha
	movl %r15d, %eax //setup mult
	mull %r13d // %r15 = a*f
	movl %eax, %r15d //finish mult
	//subtract 255 - alpha
	movl $255, %r12d //setup add
	subl %r13d, %r12d // 255 - a = r12d
	//multiply 255 - a by b
	movl %r12d, %eax //setup mult
	mull %r14d // %eax = (255 - a)*b
	movl %eax, %r12d //r12 = eax = result of  (255 - a)*b
	// add two halfs of equation  %r15 = a*f r12 = (255 - a)*b
	addl %r15d, %r12d // %r12 = (a*f + (255 - a)*b))
	//movl $0, %edx
	//divide (a*f + (255 - a)*b)) by 255
	movl %r12d, %eax //store r12 in eax for division
	movl $255, %r9d
	divl %r9d //%eax = (a*f + (255 - a)*b)/255)
	movl %eax, %r12d //move eax back to r12
	//shift left resulting color chanel back in place with 8*i
	//movb %dl, %cl //put 8*i into shifter
	sall %cl, %r12d // %r12 = ((a*f + (255 - a)*b)/255) << (8*i)
	popq %rax // remove stored final sum from stack
	addl %r12d, %eax // store the color channel in %eax, final_color += (((a*f + (255 - a)*b)/255) << (8*i));
	addl $1, %r11d // increment counter
	cmpl $3, %r11d
	jle top_loop 
  	//return final_color; value is already in %eax lol
	//addq $8, %rsp // restore ptr
	popq %rbp 
	ret
	//ret


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