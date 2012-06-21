	.include	cpu/r16.s

	.equ rofl	0x1234
	.equ r0fl	0x1234

	;mov		r0, r0fl

;Loop:
	nop
;	bral	Hello
	
	;bra		Loop

;	MOV r0, 0x1234
;	MOV r1, 0x5678
;	ADD r0, r1
;	BRA .+0x1000
;	LW r2, 0x42
;	LB r3, 0x13

;Hello:
	ret
	

	;.printf		"%%hello world! %d [%s]\n", 123, "blub"


	.macro	testmacro	$a_string, a_number

	;.equ	blub	a_string + a_string
	;.printf blub + "\n"
	;.printf "%d\n", (a_string == a_string + "x")
	
	.if (a_string == "blaat!" && a_number == 1234)
	.printf "BLAAT!!\n"
	.endif
	;.printf	"a_string='%s'\n", a_string
	;.printf	"a_number=%d\n", a_number
	.endm


	;testmacro	blaat!, 1234


	.macro	push	$register
		.parse	" sw [r13], " + register
		.parse	" add r13, 2"
	.endm
	
	push	r15


Test:
	.printf		"address of Test is 0x%06X :)\n", Test
	;.exit

	.align	4096

	;.if (test == 0)

	;.equ	test	1
	;.include	"test.s"	# wow


	;.endif
