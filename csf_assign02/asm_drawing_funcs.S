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
	// img --> edi
	// x --> esi
	// y --> rdx
	// col --> ecx
	// %eax is for return values (from funcs)
	pushq %rbp // Stores stack frame of calling func, location of ebp is a reference pt for our stack Parameters
	movq %rsp, %rbp // Taking current stack ptr and using it as frame for current func
	//store callee %r12-r15
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	pushq %rbx

	//store inputs in callee
	movq %rdi, %r12 //store img
	movq %rsi, %r13 //store x
	movq %rdx, %r14	//store y
	movq %rcx, %r15 //store color

	//Process Bounds Checking
	call is_in_bounds //if is_in_bounds(img,x,y), will access stack in this helper method
	cmp $1, %eax // is_in_bounds returns 1 or 0 
	jne not_in_bounds //if value is not 1, jump to .not_in_bounds --> jne is ineqaulity

	//Process Get Pix
	movq %r12, %rdi //store im in first inpit
	movl %r13d, %esi //store x in second
	movl %r14d, %edx //store y in third
	call get_pix //uint32 background = get_pix(img,x,y)

	//Process blur colors
	movl %eax, %esi //set b from get_pix return
	movl %r15d, %edi //set f from
	call blur_colors //background = blur_colors(color,background)

	//Process set pix
	movq %r12, %rdi //store im in first inpit
	movl %r13d, %esi //store x in second
	movl %r14d, %edx //store y in third
	movl %eax, %ecx //taked blured color from output of blur and store in 4th
	call set_pix //set_pix(img,x,y,background);

	popq %rbx //pop used callees
	popq %r15 
	popq %r14
	popq %r13
	popq %r12
	popq %rbp //fix stack
	ret
not_in_bounds:
	popq %rbx //pop used callees
	popq %r15 
	popq %r14
	popq %r13
	popq %r12
	popq %rbp //fix stack
	ret



/*
Takes input of an image and a Rect and checks that 
all corners of the rect are inbounds with the image
*/
	.globl rec_in_bounds
rec_in_bounds:
	pushq %rbp // Stores stack frame of calling func, location of ebp is a reference pt for our stack Parameters
	movq %rsp, %rbp // Taking current stack ptr and using it as frame for current func
	//store callee %r12-r15
	pushq %r12 
	pushq %r13
	pushq %r14
	pushq %r15
	pushq %rbx

	// Store inputs in callee
	movq %rdi, %r12 //store img width,height,data
	movq %rsi, %r13 //store rect tile

	// Store info about img
	movl 0(%r12), %r15d // store image height in %r15d
	movl 4(%r12), %ebx // store image width in %ebx

	// Store info about rect
	//0(r13) = x, 4(r13) = y, 8(r13) = width, 12(r13) = height
	movl 0(%r13), %r12d // x
	movl 4(%r13), %ebp // y
	movl 8(%r13), %r14d  // width
	movl 12(%r13), %r13d // height

	//if (0<=x && t_width+x<=i_width && 0<=y && t_height+y<=i_height) {
    //return 1;
    //}
    //return 0;
	cmpl $0, %r12d
	jl .LoutOfBounds
	cmpl $0, %ebp
	jl .LoutOfBounds
	movl %ebp, %r9d // moving y into r9d, for tile
	addl %r13d, %r9d // adding height to y
	cmp %r9d, %r15d // %r15d has img height
	jl .LoutOfBounds
	movl %r12d, %r9d // moving x into r9d
	addl %r14d, %r9d // adding width to x
	cmp %r9d, %ebx // %ebx had img width
	jl .LoutOfBounds
	movl $1, %eax
	jmp .Lloopdone
	.LoutOfBounds:
		movl $0, %eax
	.Lloopdone:
		//Pop used callees
		popq %rbx
		popq %r15 
		popq %r14
		popq %r13
		popq %r12
		popq %rbp 
		ret


/*
draws a pixel on the passed image at x,y of given color
does not use blur
passed image, x,y, color
confirms that x,y are valid 
*/
	.globl put_pixel
put_pixel:
	// img --> edi
	// x --> esi
	// y --> rdx
	// col --> ecx
	// %eax is for return values (from funcs)
	pushq %rbp // Stores stack frame of calling func, location of ebp is a reference pt for our stack Parameters
	movq %rsp, %rbp // Taking current stack ptr and using it as frame for current func
	//store callee %r12-r15
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	pushq %rbx

	//store inputs in callee
	movq %rdi, %r12 //store img
	movq %rsi, %r13 //store x
	movq %rdx, %r14	//store y
	movq %rcx, %r15 //store color

	//Process Bounds Checking
	call is_in_bounds //if is_in_bounds(img,x,y), will access stack in this helper method
	cmp $1, %eax // is_in_bounds returns 1 or 0 
	jne not_in_bounds_put //if value is not 1, jump to .not_in_bounds --> jne is ineqaulity

	//Process set pix
	movq %r12, %rdi //store im in first inpit
	movl %r13d, %esi //store x in second
	movl %r14d, %edx //store y in third
	movl %r15d, %ecx //taked blured color from output of blur and store in 4th
	call set_pix //set_pix(img,x,y,background);

	popq %rbx //pop used callees
	popq %r15 
	popq %r14
	popq %r13
	popq %r12
	popq %rbp //fix stack
	ret
not_in_bounds_put:
	popq %rbx //pop used callees
	popq %r15 
	popq %r14
	popq %r13
	popq %r12
	popq %rbp //fix stack
	ret



/*
called by put and draw pix to actually set the value of a given pixel
passed image, x,y, color
*/

	.globl set_pix
set_pix:
	// img --> rdi
	// x --> esi
	// y --> rdx
	// color --> ecx
	pushq %rbp // Stores stack frame of calling func, location of ebp is a reference pt for our stack Parameters
	movq %rsp, %rbp // Taking current stack ptr and using it as frame for current func
	//store callee %r12-r15
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15

	//start setting pix
	movl $0, %r10d // set r10 to proper offset to access 
	leal (%rdi), %r11d //store address of edi in r11
	addl %r11d, %r10d // add address of image to r10 --> address of columns 
	movl %edx, %eax //setup multiplication of edx
	mull (%r10d) //r1o = cols * edx = y --> y = product
	movl %eax, %edx //finish multiplication of exc
	addl %esi, %edx // add esi = x to edx = y*cols --> x+y*cols == locataion (from data offset)
	movl 8(%edi), %r10d //set r10d as the address of the data section of the image
	leal (%r10d,%edx,4), %eax //store the address of the correct index (edx) in eax
	movl %ecx, (%eax) //set the dereferenced pix pointer to the correct color
	popq %r15 //pop used callees
	popq %r14
	popq %r13
	popq %r12
	popq %rbp //fix stack
	ret

/*
is given an x,y and returns the correct color value at that location
Theoretically we need to pass in the image, x, y
*/
	.globl get_pix
get_pix:
	// img --> edi
	// x --> esi
	// y --> edx
	//eax is return register 
	pushq %rbp // Stores stack frame of calling func, location of ebp is a reference pt for our stack Parameters
	movq %rsp, %rbp // Taking current stack ptr and using it as frame for current 
	//store callee %r12-r15
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	pushq %rbx

	//start getting pix
	movl $0, %r10d // set r10 to proper offset to access 
	leal (%rdi), %r11d //store address of edi in r11
	addl %r11d, %r10d // add address of image to r10 --> address of columns 
	movl %edx, %eax //setup multiplication by edx
	mull (%r10d) //r1o = cols * edx = y --> y = product
	movl %eax, %edx //finish multiplication 
	addl %esi, %edx // add esi = x to edx = y*cols --> x+y*cols == locataion (from data offset)
	movl 8(%edi), %r10d //set r10d as the address of the data section of the image
	movl (%r10d,%edx,4), %eax //store the value of the correct index (edx) in eax to return
	pushq %rbx
	popq %r15 //pop used callees
	popq %r14
	popq %r13
	popq %r12
	popq %rbp //fix stack
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
	//subq $8, %rsp
	pushq %rbp // Stores stack frame of calling func, location of ebp is a reference pt for our stack Parameters
	movq %rsp, %rbp // Taking current stack ptr and using it as frame for current func
	//store callee %r12-r15
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	pushq %rbx

	//if (0 <= x && x < cols && 0 <= y && y < rows) 
	//check x > 0
	cmp $0, %esi // is 0<x
	jl out_o_bounds // jump if 0>=x

	//check y>0
	cmp $0, %edx // is 0<y
	jl out_o_bounds // jump if 0>=y

	//get cols
	cmp %esi, (%rdi) // is x < cols
	jle out_o_bounds // jump if x>=cols

	//get rows
	cmp %edx, 4(%rdi) // is y < cols
	jle out_o_bounds // jump if y>=rows

	//return 1 True --> in bounds
	movl $1, %eax //set return value to 1
	popq %rbx
	popq %r15 //pop used callees
	popq %r14
	popq %r13
	popq %r12
	popq %rbp
	ret
out_o_bounds:
	//return 0 False
	movl $0, %eax //set return value to 0
	popq %rbx
	popq %r15 //pop used callees
	popq %r14
	popq %r13
	popq %r12
	popq %rbp
	ret

/*
passed forground and background color and 
returns blurring of those two colors
*/
	.globl blur_colors
blur_colors:
	pushq %rbp // Stores stack frame of calling func, location of ebp is a reference pt for our stack Parameters
	movq %rsp, %rbp // Taking current stack ptr and using it as frame for current func
	//store callee %r12-r15
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	// uint32_t final_color; will store final_color in %eax, return register
	// edi --> foreground
	// esi --> background
	movl $255, %r10d
	andl %edi, %r10d // uint8_t a = (foreground & 255); 
	movl $255, %eax // final_color = 255;

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
	movl %edx, %eax //setup multiplication
	mull %r11d // %edx = (8*i)
	movl %eax, %edx //finish multiplication
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
	movl $255, %r9d //store 255 in a register to allow 
	divl %r9d //%eax = (a*f + (255 - a)*b)/255)
	movl %eax, %r12d //move eax back to r12
	//shift left resulting color chanel back in place with 8*i
	sall %cl, %r12d // %r12 = ((a*f + (255 - a)*b)/255) << (8*i)
	popq %rax // remove stored final sum from stack
	addl %r12d, %eax // store the color channel in %eax, final_color += (((a*f + (255 - a)*b)/255) << (8*i));
	addl $1, %r11d // increment counter
	cmpl $3, %r11d //end loop if counter reaches 3 iterations (for r g and b)
	jle top_loop 
	popq %r15 //pop used callees
	popq %r14
	popq %r13
	popq %r12
	popq %rbp 
	ret //return final_color; value is already in %eax lol


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
	pushq %rbp // Stores stack frame of calling func, location of ebp is a reference pt for our stack Parameters
	movq %rsp, %rbp // Taking current stack ptr and using it as frame for current func
	//store callee %r12-r15
	pushq %r12 
	pushq %r13
	pushq %r14
	pushq %r15
	pushq %rbx

	//store inputs in callee
	movq %rdi, %r12 //store img
	movq %rsi, %r13 //store rect
	movq %rdx, %r14	//store color

	//%r13 also stores x,y,width,weight
	//0(r13) = x, 4(r13) = y, 8(r13) = width, 12(r13) = height
	movl 0(%r13), %r15d // store i = x for the iterator
	movl 4(%r13), %ebx // store j = y

	rect_loop:
	// Moving all parameters into correct locations for draw_pixel call
	movq %r12, %rdi // r12 = img ptr --> rdi
	movl %r15d, %esi // r15d = i --> esi
	movl %ebx, %edx // ebx = j --> edx
	movl %r14d, %ecx // r14 = color --> ecx
	call draw_pixel
	addl $1, %ebx // increment j 
	movl 4(%r13), %r9d // moving y into r9d
	addl 12(%r13), %r9d // adding height to y
	cmp %r9d, %ebx
	jl rect_loop
	movl 4(%r13), %ebx // reset j = y for inner loop
	addl $1, %r15d // increment i
	movl 0(%r13), %r9d // moving x into r9d
	addl 8(%r13), %r9d // adding width to x
	cmp %r9d, %r15d
	jl rect_loop

	//Pop used callees
	popq %rbx
	popq %r15 
	popq %r14
	popq %r13
	popq %r12
	popq %rbp 
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
	pushq %rbp // Stores stack frame of calling func, location of ebp is a reference pt for our stack Parameters
	movq %rsp, %rbp // Taking current stack ptr and using it as frame for current func
	subq $32, %rsp //init stack pointer for alloced data
	//store callee %r12-r15
	pushq %r12 
	pushq %r13
	pushq %r14
	pushq %r15
	pushq %rbx

	
  	/*stack frame:
   * -8(%rbp) Image *img
   * -12(%rbp) uint32_t x
   * -16(%rbp) uint32_t y
   * -20(%rbp) uint32_t r
   * -32(%rbp) <-> -20(%rbp) alignment padding
   */
	movq %rdi, -8(%rbp)  //store image
	movl %esi, -12(%rbp) //store center x 
	movl %edx, -16(%rbp) //store center y
	movl %ecx, -20(%rbp) //store r


	//store inputs in callee
	//movq %rdx, %r12 //store color
	movl %r8d, %r12d //store color
	//r15 stores i = x for iterator
	//ebx stores j = y for iterator 

	
	//%r13 also stores x,y,width,weight
	//0(r13) = x, 4(r13) = y, 8(r13) = width, 12(r13) = 
	//set iterator ctrs to top left corner (center - r)
	movl -12(%rbp), %r15d // store i = x-r for the iterator
	subl -20(%rbp),%r15d //-r step
	movl -16(%rbp), %ebx // store j = y-r
	subl -20(%rbp),%ebx //-r step

	circ_loop:
	
	circ_check_bounds: //validate in_bounds
	//setup bounds checking parameters
	movq -8(%rbp), %rdi // -8(%rbp) = img ptr --> rdi
	movl %r15d, %esi // r15d = i --> esi
	movl %ebx, %edx // ebx = j --> edx
	//Process Bounds Checking
	call is_in_bounds //if is_in_bounds(img,x,y), will access stack in this helper method
	cmp $1, %eax // is_in_bounds returns 1 or 0 
	jne skip_circle_pt //if value is not 1, jump to .not_in_bounds --> jne is ineqaulity

	circ_check_dist:
	//circle distance
	//int32_t dist = (x-i)*(x-i) + (y-j)*(y-j);
	//r9 stores x segment 
	//r10 stores y segment
	movl -12(%rbp), %r9d //store center_x
	movl -16(%rbp), %r10d //store center_y
	subl %r15d, %r9d // = x-i
	subl %ebx, %r10d // = y-j
	//x part square
	movl %r9d, %eax //setup r9 as multiplied
	mull %r9d //multiply r9*r9
	movl %eax, %r9d //finish mult
	//y part square
	movl %r10d, %eax //setup r10 as multiplied
	mull %r10d //multiply r10*r10
	movl %eax, %r10d //finish mult
	//dist = sum of two parts
	addl %r10d, %r9d // dist = r9d = (x-i)*(x-i) + (y-j)*(y-j) = r9^2+r10^2
	movl -20(%rbp), %r10d // r10d = r 
	//square radius
	movl %r10d, %eax //setup r10 as multiplied
	mull %r10d //multiply r10*r10
	movl %eax, %r10d //finish mult
	//compare distance and r^2
	cmp %r10d, %r9d //cmp r^2 and distance
	jg skip_circle_pt //if distance is greater than r^2 then skip adding pix

	circ_draw_pix:
	// Moving all parameters into correct locations for draw_pixel call
	movq -8(%rbp), %rdi // -8(%rbp), = img ptr --> rdi
	movl %r15d, %esi // r15d = i --> esi
	movl %ebx, %edx // ebx = j --> edx
	movl %r12d, %ecx // r12 = color --> ecx
	call draw_pixel

	skip_circle_pt:
	//correct limit of loop < y + r
	addl $1, %ebx // increment j 
	movl -16(%rbp), %r9d // moving y center into r9d
	addl -20(%rbp), %r9d // adding r to y --> limit = center_y + r or corner y + 2r
	cmp %r9d, %ebx
	jle circ_loop
	//reset inner loop iter
	movl -16(%rbp), %ebx // store j = y-r
	subl -20(%rbp), %ebx //-r step
	addl $1, %r15d // increment i
	movl -12(%rbp), %r9d // moving x into r9d
	addl -20(%rbp), %r9d // adding r to x --> limit = x + r or corner x + 2r
	cmp %r9d, %r15d
	jle circ_loop
	
	//Pop used callees
	popq %rbx
	popq %r15 
	popq %r14
	popq %r13
	popq %r12
	addq $32, %rsp //restore stack ptr
	popq %rbp 
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
	//TODO: change input registers
	// 		fix loop	
	pushq %rbp // Stores stack frame of calling func, location of ebp is a reference pt for our stack Parameters
	movq %rsp, %rbp // Taking current stack ptr and using it as frame for current func
	subq $32, %rsp //init stack pointer for alloced data
	//store callee %r12-r15
	pushq %r12 
	pushq %r13
	pushq %r14
	pushq %r15
	pushq %rbx

	
  	/*stack frame:
   * -8(%rbp) Image *img
   * -12(%rbp) uint32_t x
   * -16(%rbp) uint32_t y
   * -24(%rbp) uint32_t r
   * -32(%rbp) <-> -24(%rbp) alignment padding
   */
	movq %rdi, -8(%rbp)  //store image
	movl %esi, -12(%rbp) //store top left x 
	movl %edx, -16(%rbp) //store top left y
	movq %rcx, -24(%rbp) //store tilemap


	//store inputs in callee
	movq %r8, %r12 //store rect
	//r15 stores i = x for iterator
	//ebx stores j = y for iterator 

	
	//%r13 also stores x,y,width,weight
	//0(r13) = x, 4(r13) = y, 8(r13) = width, 12(r13) = 
	//set iterator ctrs to top left corner (center - r)
	movl -12(%rbp), %r15d // store i = x top left for the iterator
	movl -16(%rbp), %ebx // store j = y top left


	tile_check_bounds: //validate in_bounds
	//setup bounds checking parameters
	movq -24(%rbp), %rdi // -24(%rbp) = tilemap ptr --> rdi
	movq %r12, %rsi // r12 = rect from r12d
	//Process Bounds Checking
	call rec_in_bounds //if is_in_bounds(img,x,y), will access stack in this helper method
	cmp $1, %eax // is_in_bounds returns 1 or 0 
	jne bad_tile //if value is not 1, jump to .not_in_bounds --> jne is ineqaulity

	tile_loop:
	
	tile_get_pix:
	//move tile map to rdi
	movq -24(%rbp), %rdi //tilemap --> edi
	//adjusted x cord --> esi
	movl %r15d, %esi //esi = i
	addl 0(%r12), %esi //esi = i - x + tile-->x
	subl -12(%rbp), %esi //esi = i - x


	//adjusted y cord --> edx
	movl %ebx, %edx //edx = j
	addl 4(%r12), %edx //edx = j - y + tile-->y
	subl -16(%rbp), %edx //edx = j - y
	call get_pix

	tile_draw_pix:
	// Moving all parameters into correct locations for draw_pixel call
	movq -8(%rbp), %rdi // -8(%rbp), = img ptr --> rdi
	movl %r15d, %esi // r15d = i --> esi
	movl %ebx, %edx // ebx = j --> edx
	movl %eax, %ecx // eax = color from tile --> ecx
	call put_pixel

	
	//correct limit of loop < y + r
	addl $1, %ebx // increment j 
	movl -16(%rbp), %r9d // moving y into r9d
	addl 12(%r12), %r9d // adding height to y --> limit = y + rect-->height 
	cmp %r9d, %ebx
	jle tile_loop
	//reset inner loop iter
	movl -16(%rbp), %ebx // store j = y for loop reset
	addl $1, %r15d // increment i
	movl -12(%rbp), %r9d // moving x into r9d
	addl 8(%r12), %r9d // adding height to y --> limit = y + rect-->height 
	cmp %r9d, %r15d
	jle tile_loop
	

	bad_tile:
	//Pop used callees
	popq %rbx
	popq %r15 
	popq %r14
	popq %r13
	popq %r12
	addq $32, %rsp //restore stack ptr
	popq %rbp 
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