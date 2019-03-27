	.file   "test.c"
	.option nopic
	.text
	.align  2
	.globl  main
	.type   main, @function
main:
	addi    sp,sp,-16
	sd      ra,8(sp)
	call    getint
	addiw   s0,a0,0
	call    getint
	addiw   s1,a0,0
	addiw   s2,x0,1
	addiw   s3,x0,0
	bnez    s0,.Label0
	beqz    s1,.Label1
.Label0:
	li      s4,1
	j       .Label2
.Label1:
	li      s4,0
.Label2:
	beqz    s4,.l0
	addiw   a0,s2,0
	call    putint
	addiw   a0,x0,1
	ld      ra,8(sp)
	addi    sp,sp,16
	jr      ra
	j       .l1
.l0:
	addiw   a0,s3,0
	call    putint
	addiw   a0,x0,0
	ld      ra,8(sp)
	addi    sp,sp,16
	jr      ra
.l1:
	.size   main, .-main
	.ident  "GCC: (GNU) 7.2.0"
