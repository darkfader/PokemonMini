	;;.include	cpu/pm.s
	add a, a
	add a, b
	add a, [hl]
	add a, [n+$00]
	add a, [x]
	add a, [y]
	add a, [$0000]
	add a, $00
	adc a, a
	adc a, b
	adc a, [hl]
	adc a, [n+$00]
	adc a, [x]
	adc a, [y]
	adc a, [$0000]
	adc a, $00
	add ba, $0000
	add hl, ba
	add hl, hl
	add hl, x
	add hl, y
	add hl, $0000
	add sp, ba
	add sp, hl
	add sp, $0000
	add x, ba
	add x, hl
	add x, $0000
	add y, ba
	add y, hl
	add y, $0000
	and a, a
	and a, b
	and a, [hl]
	and a, [n+$00]
	and a, [x]
	and a, [y]
	and a, [$0000]
	and a, $00
	and b, $00
	and f, $00
	and h, $00
	and l, $00
	and [hl], a
	and [hl], [x]
	and [hl], [y]
	and [hl], $00
	and [n+$00], $00
	unpack
	pack
	swap a
	swap [hl]
	callb @00000064
	call @00000167
	cmp a, a
	cmp a, b
	cmp a, [hl]
	cmp a, [n+$00]
	cmp a, [x]
	cmp a, [y]
	cmp a, [$0000]
	cmp a, $00
	cmp b, $00
	cmp h, $00
	cmp l, $00
	cmp n, $00
	cmp ba, $0000
	cmp hl, $0000
	cmp sp, $0000
	cmp x, $0000
	cmp y, $0000
	dec a
	dec b
	dec h
	dec l
	dec n
	dec [hl]
	dec [n+$00]
	dec ba
	dec hl
	dec sp
	dec x
	dec y
	div hl, a
	inc a
	inc b
	inc h
	inc l
	inc n
	inc [hl]
	inc [n+$00]
	inc ba
	inc hl
	inc sp
	inc x
	inc y
	cint $00
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
	mov a, a
	mov a, b
	mov a, f
	mov a, h
	mov a, l
	mov a, n
	mov a, [hl]
	mov a, [n+$00]
	mov a, [x+l]
	mov a, [x + $00]
	mov a, [x]
	mov a, [y+l]
	mov a, [y+$00]
	mov a, [y]
	mov a, [$0000]
	mov a, $00
	mov b, a
	mov b, b
	mov b, h
	mov b, l
	mov b, [hl]
	mov b, [n+$00]
	mov b, [x+l]
	mov b, [x+$00]
	mov b, [x]
	mov b, [y+l]
	mov b, [y+$00]
	mov b, [y]
	mov b, [$0000]
	mov b, $00
	mov f, a
	mov f, $00
	mov h, a
	mov h, b
	mov h, h
	mov h, l
	mov h, [hl]
	mov h, [n+$00]
	mov h, [x]
	mov h, [y]
	mov h, [$0000]
	mov h, $00
	mov l, a
	mov l, b
	mov l, h
	mov l, l
	mov l, [hl]
	mov l, [n+$00]
	mov l, [x]
	mov l, [y]
	mov l, [$0000]
	mov l, $00
	mov n, a
	mov [hl], a
	mov [hl], b
	mov [hl], h
	mov [hl], l
	mov [hl], [hl]
	mov [hl], [n+$00]
	mov [hl], [x]
	mov [hl], [y]
	mov [hl], $00
	mov [n+$00], a
	mov [n+$00], b
	mov [n+$00], h
	mov [n+$00], l
	mov [n+$00], [hl]
	mov [n+$00], [x]
	mov [n+$00], [y]
	mov [n+$00], $00
	mov [x+l], a
	mov [x+l], b
	mov [x+$00], a
	mov [x+$00], b
	mov [x], a
	mov [x], b
	mov [x], h
	mov [x], l
	mov [x], [hl]
	mov [x], [n+$00]
	mov [x], [x]
	mov [x], [y]
	mov [x], $00
	mov [y+l], a
	mov [y+l], b
	mov [y+$00], a
	mov [y+$00], b
	mov [y], a
	mov [y], b
	mov [y], h
	mov [y], l
	mov [y], [hl]
	mov [y], [n+$00]
	mov [y], [x]
	mov [y], [y]
	mov [y], $00
	mov [$0000], a
	mov [$0000], b
	mov [$0000], h
	mov [$0000], l
	mov ba, ba
	mov ba, hl
	mov ba, x
	mov ba, y
	mov ba, [$0000]
	mov ba, $0000
	mov hl, ba
	mov hl, hl
	mov hl, x
	mov hl, y
	mov hl, [$0000]
	mov hl, $0000
	mov n, $0000
	mov sp, $0000
	mov x, ba
	mov x, hl
	mov x, x
	mov x, y
	mov x, [$0000]
	mov x, $0000
	mov y, ba
	mov y, hl
	mov y, x
	mov y, y
	mov y, [$0000]
	mov y, $0000
	mov [$0000], ba
	mov [$0000], hl
	mov [$0000], x
	mov [$0000], y
	mov a, i
	mov a, xi
	mov a, yi
	mov i, a
	mov i, $00
	mov xi, a
	mov xi, $00
	mov yi, a
	mov yi, $00
	mul l, a
	nop 
	not a
	not b
	not [hl]
	not [n+$00]
	or a, a
	or a, b
	or a, [hl]
	or a, [n+$00]
	or a, [x]
	or a, [y]
	or a, [$0000]
	or a, $00
	or b, $00
	or f, $00
	or h, $00
	or l, $00
	or [hl], a
	or [hl], [x]
	or [hl], [y]
	or [hl], $00
	or [n+$00], $00
	pop a
	pop b
	pop h
	pop l
	popa 
	popax 
	pop f
	pop n
	pop ba
	pop hl
	pop x
	pop y
	pop i
	popx
	pusha 
	pushax 
	push a
	push b
	push f
	push h
	push l
	push n
	push ba
	push hl
	push x
	push y
	push i
	pushx
	ret
	reti
	rol a
	rol b
	rolc a
	rolc b
	ror a
	ror b
	rorc a
	rorc b
	shl a
	shl b
	shr a
	shr b
	sub a, a
	sub a, b
	sub a, [hl]
	sub a, [n+$00]
	sub a, [x]
	sub a, [y]
	sub a, [$0000]
	sub a, $00
	sbc a, a
	sbc a, b
	sbc a, [hl]
	sbc a, [n+$00]
	sbc a, [x]
	sbc a, [y]
	sbc a, [$0000]
	sbc a, $00
	sub ba, $0000
	sub hl, ba
	sub hl, hl
	sub hl, x
	sub hl, y
	sub hl, $0000
	sub sp, ba
	sub sp, hl
	sub sp, $0000
	sub x, ba
	sub x, hl
	sub x, $0000
	sub y, ba
	sub y, hl
	sub y, $0000
	tst a, b
	tst a, $00
	tst b, $00
	tst [hl], $00
	tst [n+$00], $00
	xchg a, b
	xchg a, [hl]
	xchg ba, hl
	xchg ba, sp
	xchg ba, x
	xchg ba, y
	xor a, a
	xor a, b
	xor a, [hl]
	xor a, [n+$00]
	xor a, [x]
	xor a, [y]
	xor a, [$0000]
	xor a, $00
	xor b, $00
	xor f, $00
	xor h, $00
	xor l, $00
	xor [n+$00], $00
test:	jle		test
; TODO: generate the above opcodes
