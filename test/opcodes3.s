	; the OLD instruction names

	;;.include	cpu/pm.s
	addb a, a
	addb a, b
	addb a, [hl]
	addb a, [nn+$00]
	addb a, [x1]
	addb a, [x2]
	addb a, [$0000]
	addb a, $00
	addcb a, a
	addcb a, b
	addcb a, [hl]
	addcb a, [nn+$00]
	addcb a, [x1]
	addcb a, [x2]
	addcb a, [$0000]
	addcb a, $00
	addw ba, $0000
	addw hl, ba
	addw hl, hl
	addw hl, x1
	addw hl, x2
	addw hl, $0000
	addw sp, ba
	addw sp, hl
	addw sp, $0000
	addw x1, ba
	addw x1, hl
	addw x1, $0000
	addw x2, ba
	addw x2, hl
	addw x2, $0000
	andb a, a
	andb a, b
	andb a, [hl]
	andb a, [nn+$00]
	andb a, [x1]
	andb a, [x2]
	andb a, [$0000]
	andb a, $00
	andb b, $00
	andb flags, $00
	andb h, $00
	andb l, $00
	andb [hl], a
	andb [hl], [x1]
	andb [hl], [x2]
	andb [hl], $00
	andb [nn+$00], $00
	bcdd 
	bcde 
	bcdx a
	bcdx [hl]
	callb @00000064
	call @00000167
	cmpb a, a
	cmpb a, b
	cmpb a, [hl]
	cmpb a, [nn+$00]
	cmpb a, [x1]
	cmpb a, [x2]
	cmpb a, [$0000]
	cmpb a, $00
	cmpb b, $00
	cmpb h, $00
	cmpb l, $00
	cmpb n, $00
	cmpw ba, $0000
	cmpw hl, $0000
	cmpw sp, $0000
	cmpw x1, $0000
	cmpw x2, $0000
	decb a
	decb b
	decb h
	decb l
	decb n
	decb [hl]
	decb [nn+$00]
	decw ba
	decw hl
	decw sp
	decw x1
	decw x2
	div hl, a
	incb a
	incb b
	incb h
	incb l
	incb n
	incb [hl]
	incb [nn+$00]
	incw ba
	incw hl
	incw sp
	incw x1
	incw x2
	int $00
	jcb @000000AF
	jc @000001B2
	jdbnz @000000B4
	jint $00
	jmp hl
	jmpb @000000B9
	jmp @000001BC
	jncb @000000BE
	jnc @000001C1
	jnzb @000000C3
	jnz @000001C6
	jzb @000000C8
	jz @000001CB
	movb a, a
	movb a, b
	movb a, flags
	movb a, h
	movb a, l
	movb a, n
	movb a, [hl]
	movb a, [nn+$00]
	movb a, [x1+l]
	movb a, [x1 + $00]
	movb a, [x1]
	movb a, [x2+l]
	movb a, [x2+$00]
	movb a, [x2]
	movb a, [$0000]
	movb a, $00
	movb b, a
	movb b, b
	movb b, h
	movb b, l
	movb b, [hl]
	movb b, [nn+$00]
	movb b, [x1+l]
	movb b, [x1+$00]
	movb b, [x1]
	movb b, [x2+l]
	movb b, [x2+$00]
	movb b, [x2]
	movb b, [$0000]
	movb b, $00
	movb flags, a
	movb flags, $00
	movb h, a
	movb h, b
	movb h, h
	movb h, l
	movb h, [hl]
	movb h, [nn+$00]
	movb h, [x1]
	movb h, [x2]
	movb h, [$0000]
	movb h, $00
	movb l, a
	movb l, b
	movb l, h
	movb l, l
	movb l, [hl]
	movb l, [nn+$00]
	movb l, [x1]
	movb l, [x2]
	movb l, [$0000]
	movb l, $00
	movb n, a
	movb [hl], a
	movb [hl], b
	movb [hl], h
	movb [hl], l
	movb [hl], [hl]
	movb [hl], [nn+$00]
	movb [hl], [x1]
	movb [hl], [x2]
	movb [hl], $00
	movb [nn+$00], a
	movb [nn+$00], b
	movb [nn+$00], h
	movb [nn+$00], l
	movb [nn+$00], [hl]
	movb [nn+$00], [x1]
	movb [nn+$00], [x2]
	movb [nn+$00], $00
	movb [x1+l], a
	movb [x1+l], b
	movb [x1+$00], a
	movb [x1+$00], b
	movb [x1], a
	movb [x1], b
	movb [x1], h
	movb [x1], l
	movb [x1], [hl]
	movb [x1], [nn+$00]
	movb [x1], [x1]
	movb [x1], [x2]
	movb [x1], $00
	movb [x2+l], a
	movb [x2+l], b
	movb [x2+$00], a
	movb [x2+$00], b
	movb [x2], a
	movb [x2], b
	movb [x2], h
	movb [x2], l
	movb [x2], [hl]
	movb [x2], [nn+$00]
	movb [x2], [x1]
	movb [x2], [x2]
	movb [x2], $00
	movb [$0000], a
	movb [$0000], b
	movb [$0000], h
	movb [$0000], l
	movw ba, ba
	movw ba, hl
	movw ba, x1
	movw ba, x2
	movw ba, [$0000]
	movw ba, $0000
	movw hl, ba
	movw hl, hl
	movw hl, x1
	movw hl, x2
	movw hl, [$0000]
	movw hl, $0000
	movw nn, $0000
	movw sp, $0000
	movw x1, ba
	movw x1, hl
	movw x1, x1
	movw x1, x2
	movw x1, [$0000]
	movw x1, $0000
	movw x2, ba
	movw x2, hl
	movw x2, x1
	movw x2, x2
	movw x2, [$0000]
	movw x2, $0000
	movw [$0000], ba
	movw [$0000], hl
	movw [$0000], x1
	movw [$0000], x2
	movx a, hl
	movx a, x1
	movx a, x2
	movx hl, a
	movx hl, $00
	movx x1, a
	movx x1, $00
	movx x2, a
	movx x2, $00
	mul l, a
	nop 
	notb a
	notb b
	notb [hl]
	notb [nn+$00]
	orb a, a
	orb a, b
	orb a, [hl]
	orb a, [nn+$00]
	orb a, [x1]
	orb a, [x2]
	orb a, [$0000]
	orb a, $00
	orb b, $00
	orb flags, $00
	orb h, $00
	orb l, $00
	orb [hl], a
	orb [hl], [x1]
	orb [hl], [x2]
	orb [hl], $00
	orb [nn+$00], $00
	popb a
	popb b
	popb h
	popb l
	popa 
	popax 
	popb flags
	popb n
	popw ba
	popw hl
	popw x1
	popw x2
	popx hl
	popxxx 
	pusha 
	pushax 
	pushb a
	pushb b
	pushb flags
	pushb h
	pushb l
	pushb n
	pushw ba
	pushw hl
	pushw x1
	pushw x2
	pushx hl
	pushxxx 
	ret 
	reti 
	rolb a
	rolb b
	rolcb a
	rolcb b
	rorb a
	rorb b
	rorcb a
	rorcb b
	shlb a
	shlb b
	shrb a
	shrb b
	subb a, a
	subb a, b
	subb a, [hl]
	subb a, [nn+$00]
	subb a, [x1]
	subb a, [x2]
	subb a, [$0000]
	subb a, $00
	subcb a, a
	subcb a, b
	subcb a, [hl]
	subcb a, [nn+$00]
	subcb a, [x1]
	subcb a, [x2]
	subcb a, [$0000]
	subcb a, $00
	subw ba, $0000
	subw hl, ba
	subw hl, hl
	subw hl, x1
	subw hl, x2
	subw hl, $0000
	subw sp, ba
	subw sp, hl
	subw sp, $0000
	subw x1, ba
	subw x1, hl
	subw x1, $0000
	subw x2, ba
	subw x2, hl
	subw x2, $0000
	testb a, b
	testb a, $00
	testb b, $00
	testb [hl], $00
	testb [nn+$00], $00
	xchgb a, b
	xchgb a, [hl]
	xchgw ba, hl
	xchgw ba, sp
	xchgw ba, x1
	xchgw ba, x2
	xorb a, a
	xorb a, b
	xorb a, [hl]
	xorb a, [nn+$00]
	xorb a, [x1]
	xorb a, [x2]
	xorb a, [$0000]
	xorb a, $00
	xorb b, $00
	xorb flags, $00
	xorb h, $00
	xorb l, $00
	xorb [nn+$00], $00
test:	jle		test
; TODO: generate the above opcodes
