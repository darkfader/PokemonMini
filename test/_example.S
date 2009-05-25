;-----------------------------------------------------------------------------
; PMAS example code and documentation
; by Rafael Vuijk (aka DarkFader)
;-----------------------------------------------------------------------------

//#define USE CPP !

	; assemble with: ./pmas example.s example.min
	; See mindx.txt on http://pokeme.shizzle.it/ for opcode descriptions
	; or check the sourcecode of the assembler

	; TODO: allow ";" in strings
	; TODO: check for undefined variables
	; TODO: allow other data types for variables

	;.include	pm.inc			; you can include assembler files :)

	;.base	0x2100						; skip output up to this address
	.incbin	../bios.min					; you can include binary files :)
	;.org	__addr + RAM_SIZE			; you can skip a certain bytes
	;.org	__addr + IO_SIZE

;-----------------------------------------------------------------------------

	.org	0x2100						; start of user space
	.db		"MN"

	.org	0x2102						; entry point
	jmp		start

	.org	0x2108	; 0x01CF			; interrupt vectors...
	jmp		unknown
	.org	0x210E	; 0x01E0
	jmp		unknown
	.org	0x2114	; 0x01F1
	jmp		unknown
	.org	0x211A	; 0x0202
	jmp		unknown
	.org	0x2120	; 0x0213
	jmp		unknown
	.org	0x2126	; 0x0224
	jmp		unknown
	.org	0x212C	; 0x0235
	jmp		unknown
	.org	0x2132	; 0x0246
	jmp		unknown
	.org	0x2138	; 0x025A
	jmp		unknown
	.org	0x213E	; 0x026D
	jmp		unknown
	.org	0x2144	; 0x027C
	jmp		unknown
	.org	0x214A	; 0x028D
	jmp		unknown
	.org	0x2150	; 0x029E
	jmp		unknown
	.org	0x2156	; 0x02AF
	jmp		unknown
	.org	0x215C	; 0x02C0
	jmp		unknown
	.org	0x2162	; 0x02D1
	jmp		unknown
	.org	0x2168	; 0x02E2
	jmp		unknown
	.org	0x216E	; 0x02F3
	jmp		unknown
	.org	0x2174	; 0x0304
	jmp		unknown
	.org	0x217A	; 0x0315
	jmp		unknown
	.org	0x2180	; 0x0326
	jmp		unknown
	.org	0x2186	; 0x0337
	jmp		unknown
	.org	0x218C	; 0x0348
	jmp		unknown
	.org	0x2192	; 0x035C
	jmp		unknown
	.org	0x2198	; 0x036D
	jmp		unknown

	.org	0x21A4
	.db		"NINTENDO"
	.db		"MUTE"						; Mini UnTitled English
	.db		"Untitled    "
	.db		"2P"
	
	.org	0x21D0


	;.equ	saved_addr	__addr			; save current address (not much now)
	;.org	ROM_SIZE					; go to end of ROM to fix the output size
	;.org	saved_addr					; go back where we where


	; labels must end with a colon (":") and can have an instruction behind it
start:	nop
	call	subroutine
	jmp		halt


unknown:
halt:
	jmp		halt


subroutine:
	nop
	ret

message:
	.db		"Hello world!, comma's are allowed :)",0,"\n\r\t\0" 
			; some special characters too
	.dw		0x1234,0x5678,ROM_SIZE
	.dd		0x12345678



	.end		; any code following this will be ignor
