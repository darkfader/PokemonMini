;-----------------------------------------------------------------------------
; PMAS - Pika Macro ASsembler by Rafael Vuijk (aka DarkFader)
;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------
; Comments
;-----------------------------------------------------------------------------

;			comment
#			comment

;-----------------------------------------------------------------------------
; Includes
;-----------------------------------------------------------------------------

	;.incbin	file.bin				; include binary file
	.include	cpu/pm.s				; include assembly file. PMAS executable path is tried as last.

;-----------------------------------------------------------------------------
; Options
;-----------------------------------------------------------------------------

	.option	directive	0				; invalid directive error (default = 1)
	.option	range		0				; range checking of immediate values (default = 0)
	.option	base		0				; start address of output file (default = 0)
	.option localjump	0				; jumps to local labels default to short jumps? (default = 1)	
	.option farjump		0				; jumps to non-local labels and without suffix default to far jumps? (default = 1)
	;.option word		1				; *NOT WORKING YET* non-jumps without B or W suffix default to word? (default = 0)
	.option fill		0xAA			; byte to fill uninitialized data with (default = 0xFF)

;-----------------------------------------------------------------------------
; Address
;-----------------------------------------------------------------------------

	.org	0x2100						; set current assemble address.
	.align	8							; align memory (for tile graphics etc.)

addr_example:
	jmpw	.							; loop to current address

;-----------------------------------------------------------------------------
; Relocation
;
; The __addr pointer will temporarily change while retaining output address.
; In combination with .ds you can easily describe memory structures.
;-----------------------------------------------------------------------------

data_src:
	.reloc	0x1800						; relocate to half of RAM
data_dest:

temp:	.db		123						; an initialization loop can be used to copy variables to RAM

	.endreloc
data_end:

; example of a data record...

	.reloc
Record_name_len:	.ds		1
Record_name:		.ds		20
Record_size:
	.endreloc

	mov		x1, 0x1800
	mov		b, [x1+Record_name_len]
	mov		l, Record_name
	mov		a, [x1+l]

;-----------------------------------------------------------------------------
; C PreProcessor example. The extension .S (uppercase) is often used.
;
; Further documentation @ http://gcc.gnu.org/onlinedocs/cpp/
;-----------------------------------------------------------------------------

#if 1
	nop
#endif

;-----------------------------------------------------------------------------
; Labels
;
; Local labels can be created by using '_' prefix.
;-----------------------------------------------------------------------------

DoSomething:
	nop
_loop:
	jmpb	_loop						; force short jump with 'B' suffix

DoSomething2:
_loop:
	jmpw	_loop						; force long jump with 'W' suffix

;-----------------------------------------------------------------------------
; Variables
;-----------------------------------------------------------------------------

	.equ	wow1	0xAA				; equ's are handy for register definitions and precalculations
	.equ	wow2,	wow1
	.db		wow2						; variables should be defined before using
	.equ	cool	"ascii string"		; it can even handle strings!
	.equ	cool0	cool+"\0"			; string concatenation
	.equ	cool1	"blah"+1			; results in "blah1"
	.set	cool2	123					; equivalent of set
	.unset	cool2						; unsets a symbol

;-----------------------------------------------------------------------------
; Conditions
;-----------------------------------------------------------------------------

	.if		1==1
		mov		x1, -123
	.elsif	(2==2)
		this assembler rules !
	.else
		this is a stupid way to use comments :)
	.endif

;-----------------------------------------------------------------------------
; Dynamic parsing
;
; Strings can be passed to the assembler parser
;-----------------------------------------------------------------------------

	.parse	"label"+1+":"				; parse label
	.parse	" nop"						; parse instruction

;-----------------------------------------------------------------------------
; Macro's
;
; Macro parameters are global variables, so use the names with care.
; Variables can only be number for now.
; The '_' prefix character  is replaced with a unique macro ID and can be used for labels.
;-----------------------------------------------------------------------------

	.macro	kewl	$a_string, a_string2, a_number		; first parameter is forced to string
		jmp		_local_label

		.equ	a_number2	a_number * 2
		.db		a_string, a_number
		.dw		a_number2

		.if (a_string == "hello" && a_string2 == "world" && a_number == 123 && a_number2 == 246)
			.printf "It works :)\n"
		.else
			.printf "NOT WORKING!\n"	
		.endif
	
_local_label:
	.endm

	kewl	hello, "world", 123			; macro call

	.rept	3							; repeat loop
	nop
	.endr

;-----------------------------------------------------------------------------
; Operands
;-----------------------------------------------------------------------------

	mov		a	,170					; decimal
 mov		a 	, 0b10101010			; binary (% is no more)
 	 mov	a,	  $AA					; hex
	 mov	a,   0xAA					; hex
	mov		a, @AA						; hex (deprecated)
	mov		a, 'A'						; ascii value
	mov		x1, 'x1'					; two ascii values
	mov		a, '\x40'					; hex

;-----------------------------------------------------------------------------
; Data
;-----------------------------------------------------------------------------

	.db		cool0, 0, "\"\r\n\t\x55\\\0",		# strings are byte arrays
.db			1,2,3,4
	.dw		0x0201,0x0403
.dd			0x04030201
	.ds		100							; reserve 100 bytes, fill with .option fill
	.ds		100, 0xAA					; reserve 100 bytes, fill with 0xAA

;-----------------------------------------------------------------------------
; Other
;-----------------------------------------------------------------------------

	.printf "I'm at %06X.\n", .			; formatted output
	;.exit								; end processing of all files (results in an error)
	.end								; end processing current file
