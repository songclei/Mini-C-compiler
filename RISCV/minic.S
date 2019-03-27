	.file   "test.c"
	.option nopic
	.comm   v0,420,8
	.text
	.align  2
	.globl  main
	.type   main, @function
main:
	addi    sp,sp,-16
	sd      ra,8(sp)
	addiw   s1,x0,32
	addiw   s2,x0,10
	call    getint
	addiw   s0,a0,0
	addiw   s3,x0,0
.l0:
	slt     s4,s3,s0
	bnez    s4,.l1
	call    getint
	addiw   s4,a0,0
	addiw   s8,x0,4
	mulw    s5,s8,s3
	lui     s8,%hi(v0)
	lw      s8,%lo(v0)(s8)
	addw    s8,s8,s5
	sw      s4,0(s8)
	addiw   s3,s3,1
	j       .l0
.l1:
	addiw   s3,s0,-1
.l2:
	bnez    s4,.l3
	addiw   s4,x0,0
.l4:
	slt     s5,s4,s3
	bnez    s5,.l5
	addiw   s8,x0,4
	mulw    s5,s8,s4
	lui     s8,%hi(v0)
	lw      s8,%lo(v0)(s8)
	addw    s8,s8,s5
	lw      s5,0(s8)
	addiw   s6,s4,1
	addiw   s8,x0,4
	mulw    s6,s8,s6
	lui     s8,%hi(v0)
	lw      s8,%lo(v0)(s8)
	addw    s8,s8,s6
	lw      s6,0(s8)
	sgt     s5,s5,s6
	bnez    s5,.l6
	addiw   s8,x0,4
	mulw    s5,s8,s4
	lui     s8,%hi(v0)
	lw      s8,%lo(v0)(s8)
	addw    s8,s8,s5
	lw      s5,0(s8)
	addiw   s6,s4,1
	addiw   s8,x0,4
	mulw    s6,s8,s6
	lui     s8,%hi(v0)
	lw      s8,%lo(v0)(s8)
	addw    s8,s8,s6
	lw      s6,0(s8)
	addiw   s8,x0,4
	mulw    s7,s8,s4
	lui     s8,%hi(v0)
	lw      s8,%lo(v0)(s8)
	addw    s8,s8,s7
	sw      s6,0(s8)
	addiw   s6,s4,1
	addiw   s8,x0,4
	mulw    s6,s8,s6
	lui     s8,%hi(v0)
	lw      s8,%lo(v0)(s8)
	addw    s8,s8,s6
	sw      s5,0(s8)
.l6:
	addiw   s4,s4,1
	j       .l4
.l5:
	addiw   s3,s3,-1
	j       .l2
.l3:
	addiw   s3,x0,0
.l7:
	slt     s4,s3,s0
	bnez    s4,.l8
	addiw   s8,x0,4
	mulw    s4,s8,s3
	lui     s8,%hi(v0)
	lw      s8,%lo(v0)(s8)
	addw    s8,s8,s4
	lw      s5,0(s8)
	addiw   a0,s5,0
	call    putint
	addiw   s5,a0,0
	addiw   a0,s2,0
	call    putchar
	addiw   s5,a0,0
	addiw   s3,s3,1
	j       .l7
.l8:
	addiw   a0,x0,0
	ld      ra,8(sp)
	addi    sp,sp,16
	jr      ra
	.size   main, .-main
	.ident  "GCC: (GNU) 7.2.0"
