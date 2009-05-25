	;.include	cpu/pm.s

	.if		0
	nop
	.endif

	.equ	A			"A"
	.equ	B			"B"
	.equ	C			"C"
	.equ	ABC			"A" + ("B"+"C")

	.db		ABC

	.ds		1, 0		; =0 !
	.db		1,
	.db		2;
	.db		3#    hmm
	.db		2*2 #
	.db		$5	;
	.db		0b110 #
	.equ	string		"\x07"
	.db		string		;"\x07"
	.db		8;	ah :)
	.db		"\t";
	.db		@A,11
	.db		12,"",13
	.dw		$0000F0E

	.dd		256*256

label:	# label :)
	.equ	label2		label		# same :)
	jmp		label2

	mov		a, 'A'
	mov		a, 'A'
	mov		a, '\x41'
	movw	x1, 'x1'

	.macro	kewl	param1, param2
	.db		param1, param2, param1 + param2
_label:
	jmp		_label		# yeah !
	.endm

Test1:
_hmm:
	kewl	1,2

	.option	localjump	0

Test2:
_hmm:
	kewl	1,2

	.equ	nop		" no" + "p ; this is nothing"

	.rept	3
	.parse	nop		; wow !
	.endr

	.macro	kewl2	p1, p2, p3
	.db		p1 + p2, p3
	.endm
	
	kewl2	"A","B",""

	.end

	nop
