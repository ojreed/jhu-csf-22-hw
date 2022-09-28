	.file	"c_drawing_funcs.c"
	.text
	.globl	blur_colors
	.type	blur_colors, @function
blur_colors:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -36(%rbp)
	movl	%esi, -40(%rbp)
	movl	-36(%rbp), %eax
	movb	%al, -9(%rbp)
	movl	$255, -4(%rbp)
	movl	$1, -8(%rbp)
	jmp	.L2
.L3:
	movl	-8(%rbp), %eax
	sall	$3, %eax
	movl	$255, %edx
	movl	%eax, %ecx
	sall	%cl, %edx
	movl	%edx, %eax
	andl	-36(%rbp), %eax
	movl	%eax, %edx
	movl	-8(%rbp), %eax
	sall	$3, %eax
	movl	%eax, %ecx
	shrl	%cl, %edx
	movl	%edx, %eax
	movl	%eax, -16(%rbp)
	movl	-8(%rbp), %eax
	sall	$3, %eax
	movl	$255, %edx
	movl	%eax, %ecx
	sall	%cl, %edx
	movl	%edx, %eax
	andl	-40(%rbp), %eax
	movl	%eax, %edx
	movl	-8(%rbp), %eax
	sall	$3, %eax
	movl	%eax, %ecx
	shrl	%cl, %edx
	movl	%edx, %eax
	movl	%eax, -20(%rbp)
	movzbl	-9(%rbp), %eax
	imull	-16(%rbp), %eax
	movl	%eax, %edx
	movzbl	-9(%rbp), %eax
	movl	$255, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	imull	-20(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, %edx
	movl	$2155905153, %eax
	imulq	%rdx, %rax
	shrq	$32, %rax
	movl	%eax, %edx
	shrl	$7, %edx
	movl	-8(%rbp), %eax
	sall	$3, %eax
	movl	%eax, %ecx
	sall	%cl, %edx
	movl	%edx, %eax
	addl	%eax, -4(%rbp)
	addl	$1, -8(%rbp)
.L2:
	cmpl	$3, -8(%rbp)
	jle	.L3
	movl	-4(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	blur_colors, .-blur_colors
	.globl	is_in_bounds
	.type	is_in_bounds, @function
is_in_bounds:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	%edx, -32(%rbp)
	movq	-24(%rbp), %rax
	movl	4(%rax), %eax
	movl	%eax, -4(%rbp)
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -8(%rbp)
	cmpl	$0, -28(%rbp)
	js	.L6
	movl	-28(%rbp), %eax
	cmpl	-8(%rbp), %eax
	jge	.L6
	cmpl	$0, -32(%rbp)
	js	.L6
	movl	-32(%rbp), %eax
	cmpl	-4(%rbp), %eax
	jge	.L6
	movl	$1, %eax
	jmp	.L7
.L6:
	movl	$0, %eax
.L7:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	is_in_bounds, .-is_in_bounds
	.globl	get_pix
	.type	get_pix, @function
get_pix:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	%edx, -32(%rbp)
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	movl	-32(%rbp), %eax
	imull	-4(%rbp), %eax
	movl	%eax, %edx
	movl	-28(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, -8(%rbp)
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	movl	-8(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rdx, %rax
	movl	(%rax), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	get_pix, .-get_pix
	.globl	set_pix
	.type	set_pix, @function
set_pix:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	%edx, -32(%rbp)
	movl	%ecx, -36(%rbp)
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	movl	-32(%rbp), %eax
	imull	-4(%rbp), %eax
	movl	%eax, %edx
	movl	-28(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, -8(%rbp)
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	movl	-8(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rax, %rdx
	movl	-36(%rbp), %eax
	movl	%eax, (%rdx)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	set_pix, .-set_pix
	.globl	put_pixel
	.type	put_pixel, @function
put_pixel:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$24, %rsp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	%edx, -16(%rbp)
	movl	%ecx, -20(%rbp)
	movl	-16(%rbp), %edx
	movl	-12(%rbp), %ecx
	movq	-8(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	is_in_bounds
	testl	%eax, %eax
	je	.L13
	movl	-20(%rbp), %eax
	orb	$-1, %al
	movl	%eax, %ecx
	movl	-16(%rbp), %edx
	movl	-12(%rbp), %esi
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	set_pix
.L13:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	put_pixel, .-put_pixel
	.globl	rec_in_bounds
	.type	rec_in_bounds, @function
rec_in_bounds:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movq	-48(%rbp), %rax
	movl	8(%rax), %eax
	movl	%eax, -4(%rbp)
	movq	-48(%rbp), %rax
	movl	12(%rax), %eax
	movl	%eax, -8(%rbp)
	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -12(%rbp)
	movq	-40(%rbp), %rax
	movl	4(%rax), %eax
	movl	%eax, -16(%rbp)
	movq	-48(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -20(%rbp)
	movq	-48(%rbp), %rax
	movl	4(%rax), %eax
	movl	%eax, -24(%rbp)
	cmpl	$0, -20(%rbp)
	js	.L15
	movl	-4(%rbp), %edx
	movl	-20(%rbp), %eax
	addl	%edx, %eax
	cmpl	%eax, -12(%rbp)
	jl	.L15
	cmpl	$0, -24(%rbp)
	js	.L15
	movl	-8(%rbp), %edx
	movl	-24(%rbp), %eax
	addl	%edx, %eax
	cmpl	%eax, -16(%rbp)
	jl	.L15
	movl	$1, %eax
	jmp	.L16
.L15:
	movl	$0, %eax
.L16:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	rec_in_bounds, .-rec_in_bounds
	.globl	draw_pixel
	.type	draw_pixel, @function
draw_pixel:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$40, %rsp
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	%edx, -32(%rbp)
	movl	%ecx, -36(%rbp)
	movl	-32(%rbp), %edx
	movl	-28(%rbp), %ecx
	movq	-24(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	is_in_bounds
	testl	%eax, %eax
	je	.L19
	movl	-32(%rbp), %edx
	movl	-28(%rbp), %ecx
	movq	-24(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	get_pix
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %edx
	movl	-36(%rbp), %eax
	movl	%edx, %esi
	movl	%eax, %edi
	call	blur_colors
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %ecx
	movl	-32(%rbp), %edx
	movl	-28(%rbp), %esi
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	set_pix
.L19:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	draw_pixel, .-draw_pixel
	.globl	draw_rect
	.type	draw_rect, @function
draw_rect:
.LFB13:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$56, %rsp
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movl	%edx, -52(%rbp)
	movq	-48(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -12(%rbp)
	movq	-48(%rbp), %rax
	movl	4(%rax), %eax
	movl	%eax, -16(%rbp)
	movq	-48(%rbp), %rax
	movl	8(%rax), %eax
	movl	%eax, -20(%rbp)
	movq	-48(%rbp), %rax
	movl	12(%rax), %eax
	movl	%eax, -24(%rbp)
	movl	-12(%rbp), %eax
	movl	%eax, -4(%rbp)
	jmp	.L21
.L24:
	movl	-16(%rbp), %eax
	movl	%eax, -8(%rbp)
	jmp	.L22
.L23:
	movl	-52(%rbp), %ecx
	movl	-8(%rbp), %edx
	movl	-4(%rbp), %esi
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	draw_pixel
	addl	$1, -8(%rbp)
.L22:
	movl	-16(%rbp), %edx
	movl	-24(%rbp), %eax
	addl	%edx, %eax
	cmpl	%eax, -8(%rbp)
	jl	.L23
	addl	$1, -4(%rbp)
.L21:
	movl	-12(%rbp), %edx
	movl	-20(%rbp), %eax
	addl	%edx, %eax
	cmpl	%eax, -4(%rbp)
	jl	.L24
	nop
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	draw_rect, .-draw_rect
	.globl	draw_circle
	.type	draw_circle, @function
draw_circle:
.LFB14:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$56, %rsp
	movq	%rdi, -40(%rbp)
	movl	%esi, -44(%rbp)
	movl	%edx, -48(%rbp)
	movl	%ecx, -52(%rbp)
	movl	%r8d, -56(%rbp)
	movl	-44(%rbp), %eax
	subl	-52(%rbp), %eax
	movl	%eax, -12(%rbp)
	movl	-48(%rbp), %eax
	subl	-52(%rbp), %eax
	movl	%eax, -16(%rbp)
	movl	-12(%rbp), %eax
	movl	%eax, -4(%rbp)
	jmp	.L26
.L30:
	movl	-16(%rbp), %eax
	movl	%eax, -8(%rbp)
	jmp	.L27
.L29:
	movl	-8(%rbp), %edx
	movl	-4(%rbp), %ecx
	movq	-40(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	is_in_bounds
	cmpl	$1, %eax
	jne	.L28
	movl	-44(%rbp), %eax
	subl	-4(%rbp), %eax
	movl	%eax, %edx
	movl	-44(%rbp), %eax
	subl	-4(%rbp), %eax
	movl	%edx, %ecx
	imull	%eax, %ecx
	movl	-48(%rbp), %eax
	subl	-8(%rbp), %eax
	movl	%eax, %edx
	movl	-48(%rbp), %eax
	subl	-8(%rbp), %eax
	imull	%edx, %eax
	addl	%ecx, %eax
	movl	%eax, -20(%rbp)
	movl	-52(%rbp), %eax
	imull	%eax, %eax
	cmpl	%eax, -20(%rbp)
	jg	.L28
	movl	-56(%rbp), %ecx
	movl	-8(%rbp), %edx
	movl	-4(%rbp), %esi
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	draw_pixel
.L28:
	addl	$1, -8(%rbp)
.L27:
	movl	-52(%rbp), %eax
	leal	(%rax,%rax), %edx
	movl	-16(%rbp), %eax
	addl	%edx, %eax
	cmpl	%eax, -8(%rbp)
	jle	.L29
	addl	$1, -4(%rbp)
.L26:
	movl	-52(%rbp), %eax
	leal	(%rax,%rax), %edx
	movl	-12(%rbp), %eax
	addl	%edx, %eax
	cmpl	%eax, -4(%rbp)
	jle	.L30
	nop
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	draw_circle, .-draw_circle
	.globl	draw_tile
	.type	draw_tile, @function
draw_tile:
.LFB15:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -40(%rbp)
	movl	%esi, -44(%rbp)
	movl	%edx, -48(%rbp)
	movq	%rcx, -56(%rbp)
	movq	%r8, -64(%rbp)
	movq	-64(%rbp), %rax
	movl	8(%rax), %eax
	movl	%eax, -12(%rbp)
	movq	-64(%rbp), %rax
	movl	12(%rax), %eax
	movl	%eax, -16(%rbp)
	movq	-64(%rbp), %rdx
	movq	-56(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	rec_in_bounds
	cmpl	$1, %eax
	jne	.L37
	movl	-44(%rbp), %eax
	movl	%eax, -4(%rbp)
	jmp	.L33
.L36:
	movl	-48(%rbp), %eax
	movl	%eax, -8(%rbp)
	jmp	.L34
.L35:
	movl	-8(%rbp), %eax
	subl	-48(%rbp), %eax
	movl	%eax, %edx
	movq	-64(%rbp), %rax
	movl	4(%rax), %eax
	addl	%eax, %edx
	movl	-4(%rbp), %eax
	subl	-44(%rbp), %eax
	movl	%eax, %ecx
	movq	-64(%rbp), %rax
	movl	(%rax), %eax
	addl	%eax, %ecx
	movq	-56(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	get_pix
	movl	%eax, -20(%rbp)
	movl	-20(%rbp), %ecx
	movl	-8(%rbp), %edx
	movl	-4(%rbp), %esi
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	put_pixel
	addl	$1, -8(%rbp)
.L34:
	movl	-48(%rbp), %edx
	movl	-16(%rbp), %eax
	addl	%edx, %eax
	cmpl	%eax, -8(%rbp)
	jl	.L35
	addl	$1, -4(%rbp)
.L33:
	movl	-44(%rbp), %edx
	movl	-12(%rbp), %eax
	addl	%edx, %eax
	cmpl	%eax, -4(%rbp)
	jl	.L36
.L37:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	draw_tile, .-draw_tile
	.globl	draw_sprite
	.type	draw_sprite, @function
draw_sprite:
.LFB16:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -40(%rbp)
	movl	%esi, -44(%rbp)
	movl	%edx, -48(%rbp)
	movq	%rcx, -56(%rbp)
	movq	%r8, -64(%rbp)
	movq	-64(%rbp), %rax
	movl	8(%rax), %eax
	movl	%eax, -12(%rbp)
	movq	-64(%rbp), %rax
	movl	12(%rax), %eax
	movl	%eax, -16(%rbp)
	movq	-64(%rbp), %rdx
	movq	-56(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	rec_in_bounds
	cmpl	$1, %eax
	jne	.L44
	movl	-44(%rbp), %eax
	movl	%eax, -4(%rbp)
	jmp	.L40
.L43:
	movl	-48(%rbp), %eax
	movl	%eax, -8(%rbp)
	jmp	.L41
.L42:
	movl	-8(%rbp), %eax
	subl	-48(%rbp), %eax
	movl	%eax, %edx
	movq	-64(%rbp), %rax
	movl	4(%rax), %eax
	addl	%eax, %edx
	movl	-4(%rbp), %eax
	subl	-44(%rbp), %eax
	movl	%eax, %ecx
	movq	-64(%rbp), %rax
	movl	(%rax), %eax
	addl	%eax, %ecx
	movq	-56(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	get_pix
	movl	%eax, -20(%rbp)
	movl	-20(%rbp), %ecx
	movl	-8(%rbp), %edx
	movl	-4(%rbp), %esi
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	draw_pixel
	addl	$1, -8(%rbp)
.L41:
	movl	-48(%rbp), %edx
	movl	-16(%rbp), %eax
	addl	%edx, %eax
	cmpl	%eax, -8(%rbp)
	jl	.L42
	addl	$1, -4(%rbp)
.L40:
	movl	-44(%rbp), %edx
	movl	-12(%rbp), %eax
	addl	%edx, %eax
	cmpl	%eax, -4(%rbp)
	jl	.L43
.L44:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	draw_sprite, .-draw_sprite
	.ident	"GCC: (GNU) 12.1.1 20220507 (Red Hat 12.1.1-1)"
	.section	.note.GNU-stack,"",@progbits
